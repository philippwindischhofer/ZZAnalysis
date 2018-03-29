from Preprocessor import Preprocessor
from ListPreprocessor import ListPreprocessor
from keras.preprocessing.sequence import pad_sequences
import pandas as pd
import numpy as np

class RNNPreprocessor(Preprocessor):
    
    def __init__(self, name, nonperiodic_columns, periodic_columns, sorted_column, cuts, preprocessor_basetype):
        self.name = name
        self.nonperiodic_columns = nonperiodic_columns
        self.periodic_columns = periodic_columns
        self.sorted_column = sorted_column
        self.cuts = cuts
        
        # prepare the periodic column names after encoding
        self.periodic_columns_encoded = []
        for periodic_column in periodic_columns:
            self.periodic_columns_encoded.append(periodic_column + "_sin")
            self.periodic_columns_encoded.append(periodic_column + "_cos")
            
        self.processed_columns = self.nonperiodic_columns + self.periodic_columns_encoded

        # now create the list preprocessor that is going to be used
        self.list_preprocessor = ListPreprocessor(name, self.processed_columns, self.cuts, preprocessor_basetype)

        print "RNN preprocessor for stream '" + self.name + "'"

    @classmethod
    def from_config(cls, config_section):
        raise NotImplementedError("Still to be done!")

    def to_config(self, confhandler):
        raise NotImplementedError("Still to be done!")        
        
    def setup_generator(self, datagen, len_setupdata):
        self.len_setupdata = len_setupdata
        extracted_data = []
        extracted_rows = 0
        
        for data in datagen:
            extracted_data.append(data)
            extracted_rows += len(data)
            
            if extracted_rows > self.len_setupdata:
                break
                
        print "setting up RNN preprocessor on " + str(extracted_rows) + " events"
                
        input_data = pd.concat(extracted_data)
        input_data = input_data.reset_index(drop = True)

        self.setup(input_data)
    
    def setup(self, data):
        prepared_data = self._prepare_data(data, self.sorted_column)
        self.list_preprocessor.setup(prepared_data)

    def get_last_indices(self):
        return self.list_preprocessor.get_last_indices()
        
    def process(self, data):
        # prepare the data by adding the encoded angle columns
        prepared_data = self._prepare_data(data, self.sorted_column)
        
        # run it through the list preprocessor
        processed_data = self.list_preprocessor.process(prepared_data)
        
        return processed_data
    
    def save(self, folder, filename):
        self.list_preprocessor.save(folder, filename)
        
    def load(self, folder, filename):
        self.list_preprocessor.load(folder, filename)
    
    def _prepare_data(self, data, sorted_column):
        # any column that contains a (2 pi-periodic) angle needs to be encoded as the sine and cosine of this angle
        periodic_data_encoded = []
        for periodic_column in self.periodic_columns:
            periodic_data_encoded.append(self._encode_angles(data, periodic_column))

        # now need to join the list of dataframes holding the encoded angles: take the first one ...
        angles_encoded = periodic_data_encoded[0]
        
        # ... and join it sequentially with all the other ones:
        for periodic_column_encoded in periodic_data_encoded[1:]:
            angles_encoded = angles_encoded.join(periodic_column_encoded)

        # now, add back the non-periodic (i.e. non-angle) columns
        prepared_data = data[self.nonperiodic_columns].join(angles_encoded)
    
        def sort_row(row, sorted_col):
            sorted_ind = row[sorted_col].argsort()
            sorted_ind = sorted_ind[::-1]

            for i in range(len(row)):
                row[i] = row[i][sorted_ind]
            
            return row

        sort_index = prepared_data.columns.get_loc(sorted_column)

        prepared_data = prepared_data.apply(lambda row: sort_row(row, sort_index), raw = True, axis = 1)

        # and sort it in p_t (highest p_t comes first)
        # prepared_data = self._sort_dataframe(prepared_data, sorted_column, True)
    
        return prepared_data

    # encode an angle phi as the tuple (sin(phi), cos(phi))
    def _encode_angles(self, df, col):
        df_out = pd.DataFrame()

        sin_encoding = df[col].apply(lambda x: np.sin(x))
        cos_encoding = df[col].apply(lambda x: np.cos(x))

        df_out[col + "_sin"] = sin_encoding
        df_out[col + "_cos"] = cos_encoding

        return df_out

    # can this be put into the abstract class??
    def _rowcol_cut(self, data):
        # apply the row selection
        data = data.loc[data.apply(self.cuts, axis = 1)]
        
        # apply the column selection
        output_data = data.loc[:, self.processed_columns]
        
        return output_data
