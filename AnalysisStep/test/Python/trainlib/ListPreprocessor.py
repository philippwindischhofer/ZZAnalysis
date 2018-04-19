from Preprocessor import Preprocessor
from keras.preprocessing.sequence import pad_sequences
import pandas as pd
import numpy as np
import pickle

# this is a wrapper class around a given preprocessor of type "preprocessor_base_type" that makes it also work on lists
class ListPreprocessor(Preprocessor):
   # "processed_columns" should only refer to those which contain numpy arrays as their content!
    def __init__(self, name, processed_columns, cuts, preprocessor_base_type):
        self.name = name
        self.processed_columns = processed_columns
        self.cuts = cuts

        # create the underlying preprocessor object that is going to handle the unpacked lists
        self.base_preprocessor = preprocessor_base_type(self.name + "_base", processed_columns, cuts)
        
        self.last_indices = None

        print "ListPreprocessor for stream '" + self.name + "'"

    @classmethod
    def from_config(cls, config_section):
        raise NotImplementedError("Still to be done!")

    def to_config(self, confhandler):
        raise NotImplementedError("Still to be done!")        
        
    def setup_generator(self, datagen, len_setupdata):
        self.len_setupdata = len_setupdata
        extracted_data = []
        extracted_rows = 0

        print "drawing data from generator"
        
        for data in datagen:
            extracted_data.append(data)
            extracted_rows += len(data)
            
            if extracted_rows > self.len_setupdata:
                break
                
        print "setting up list preprocessor on " + str(extracted_rows) + " events"
        
        input_data = pd.concat(extracted_data)
        input_data = input_data.reset_index(drop = True)

        self.setup(input_data)
    
    def setup(self, data):
        # first reduce the passed dataframe to the needed columns (these must be the ones having lists as entries!)
        cut_data = self._rowcol_cut(data)
        
        print "List: " + str(data.columns)

        print str(len(cut_data)) + " remaining after the cuts"

        # unpack the list data ...
        unpacked_data = self._unpack_series(cut_data)
        
        # ... and set up the base preprocessor on this unpacked data
        self.base_preprocessor.setup(unpacked_data)

        # determine also the maximum length of the stored lists (defines the zero-padding length as well)
        representative_column = self.processed_columns[0]
        list_lengths = [len(entry) for index, entry in cut_data[representative_column].iteritems()]
        self.maxlen = max(np.max(list_lengths), 1)

        print "found a maximum list length in the setup data of " + str(self.maxlen) + ": will pad or truncate to this length from now on"
        
    def process(self, data):        
        # again apply the cuts first (this leaves the series unpacked, of course)
        cut_data = self._rowcol_cut(data)
        self.last_indices = cut_data.index

        data_out = []

        for index, row in cut_data.iterrows():
            rowframe = pd.DataFrame([row.values], columns = cut_data.columns)

            unpacked_row = self._unpack_series(rowframe)
        
            # push it into the processor, if there are jets available, otherwise make a default answer that then gets padded
            if len(unpacked_row) != 0:
                processed_row = self.base_preprocessor.process(unpacked_row).get(self.name+ "_base")
            else:
                processed_row = np.zeros((1, len(cut_data.columns)))
                                                
            processed_row = np.swapaxes(processed_row, 0, 1)
            
            padded_row = pad_sequences(processed_row, maxlen = self.maxlen, dtype = 'float32', padding = 'post', truncating = 'post', value = 0.0)

            data_out.append(padded_row)
        
        data_out = np.array(data_out)
        data_out = np.swapaxes(data_out, 1, 2)
        
        return {self.name: data_out}
    
    def save(self, folder, filename):
        self.base_preprocessor.save(folder, filename)

        path = folder + "padding_" + filename
        outfile = open(path, "wb")
        pickle.dump(self.maxlen, outfile)
        outfile.close()
        
    def load(self, folder, filename):
        self.base_preprocessor.load(folder, filename)

        path = folder + "padding_" + filename        
        print "now attempting to load padding settings from file " + path

        infile = open(path, "rb")
        self.maxlen = pickle.load(infile)
        infile.close()
    
    def get_last_indices(self):
        return self.last_indices

    # can this be put into the abstract class??
    def _rowcol_cut(self, data):
        # apply the row selection
        data = data.loc[data.apply(self.cuts, axis = 1)]
        
        # apply the column selection
        output_data = data.loc[:, self.processed_columns]
        
        return output_data

    # unpacks the columns containing numpy arrays as entries
    def _unpack_series(self, df):
        def flatten_column(col):
            test = np.array(col).tolist()
            return np.concatenate(test)

        df_dict = {col: flatten_column(df[col]) for col in df.columns}
        df_out = pd.DataFrame(df_dict, columns = df.columns)

        #print "df_out before pt-cut: "
        #print df_out

        if any("Jet" in col for col in df_out.columns):
            # apply the jet-pt cut
            mask_column = df_out["JetPt"]
            mask = mask_column < 30.0
            df_out[mask.as_matrix()] = 0.0
            
            # now drop the lines in the dataframe that are all zero
            df_out = df_out.loc[~(df_out == 0).all(axis = 1)]

        #print "df_out after pt-cut: "
        #print df_out
        
        return df_out
