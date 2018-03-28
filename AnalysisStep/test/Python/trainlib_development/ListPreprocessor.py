from Preprocessor import Preprocessor
from keras.preprocessing.sequence import pad_sequences
import pandas as pd
import numpy as np

# this is a wrapper class around a given preprocessor of type "preprocessor_base_type" that makes it also work on lists
class ListPreprocessor(Preprocessor):
   # "processed_columns" should only refer to those which contain numpy arrays as their content!
    def __init__(self, name, processed_columns, cuts, preprocessor_base_type):
        self.name = name
        self.processed_columns = processed_columns
        self.cuts = cuts

        # create the underlying preprocessor object that is going to handle the unpacked lists
        self.base_preprocessor = preprocessor_base_type("unpacked", processed_columns, cuts)
        
    def setup_generator(self, datagen, len_setupdata):
        self.len_setupdata = len_setupdata
        extracted_data = []
        extracted_rows = 0
        
        for data in datagen:
            extracted_data.append(data[0])
            extracted_rows += len(data[0])
            
            if extracted_rows > self.len_setupdata:
                break
                
        print "setting up list preprocessor on " + str(extracted_rows) + " events"
        
        input_data = pd.concat(extracted_data)
        self.setup(input_data)
    
    def setup(self, data):
        # first reduce the passed dataframe to the needed columns (these must be the ones having lists as entries!)
        data = self._rowcol_cut(data)
        
        print str(len(data)) + " remaining after the cuts"
        
        # unpack the list data ...
        unpacked_data = self._unpack_series(data)
        
        # ... and set up the base preprocessor on this unpacked data
        self.base_preprocessor.setup(unpacked_data)
        
    def process(self, data):        
        # again apply the cuts first (this leaves the series unpacked, of course)
        cut_data = self._rowcol_cut(data)

        # determine the maximum length of the stored lists (defines the zero-padding length as well)
        representative_column = self.processed_columns[0]
        number_jets = [len(entry) for index, entry in cut_data[representative_column].iteritems()]
        maxlen = max(np.max(number_jets), 1)
        
        data_out = []

        for index, row in cut_data.iterrows():
            rowframe = pd.DataFrame([row.values], columns = cut_data.columns)

            unpacked_row = self._unpack_series(rowframe)
        
            # push it into the processor, if there are jets available, otherwise make a default answer that then gets padded
            if len(unpacked_row) != 0:
                processed_row = self.base_preprocessor.process(unpacked_row).get("unpacked")
            else:
                processed_row = np.zeros((1, len(cut_data.columns)))
                                                
            processed_row = np.swapaxes(processed_row, 0, 1)
            
            padded_row = pad_sequences(processed_row, maxlen = 4, dtype = 'float32', padding = 'post', truncating = 'post', value = 0.0)
            data_out.append(padded_row)
        
        data_out = np.array(data_out)
        data_out = np.swapaxes(data_out, 1, 2)
        
        return {self.name: data_out}
    
    def save(self, folder, filename):
        self.base_preprocessor.save(folder, filename)
        
    def load(self, folder, filename):
        self.base_preprocessor.load(folder, filename)
    
    # can this be put into the abstract class??
    def _rowcol_cut(self, data):
        # apply the row selection
        data = data.loc[data.apply(self.cuts, axis = 1)]
        
        # apply the column selection
        output_data = data.loc[:, self.processed_columns]
        
        return output_data

    # unpacks the columns containing numpy arrays as entries
    def _unpack_series(self, df):
        cols = df.columns
        df_out = pd.DataFrame(columns = df.columns)

        for index, row in df.iterrows():
            # check that the arrays in the columns all have a common length
            lengths = [len(row[col]) for col in cols]
            assert(min(lengths) == max(lengths))
            length = lengths[0]
    
            for i in range(length):
                df_out = df_out.append(pd.DataFrame([[row[col][i] for col in cols]], columns = cols))
        
        df_out = df_out.reset_index(drop = True)   
    
        return df_out
