from Preprocessor import Preprocessor
from ListPreprocessor import ListPreprocessor
from keras.preprocessing.sequence import pad_sequences
import pandas as pd
import numpy as np

class RNNPreprocessor(Preprocessor):
    
    def __init__(self, nonperiodic_columns, periodic_columns, sorted_column, cuts, preprocessor_basetype):
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
        self.list_preprocessor = ListPreprocessor(self.processed_columns, self.cuts, preprocessor_basetype)
        
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
        prepared_data = self._prepare_data(data, self.sorted_column)
        self.list_preprocessor.setup(prepared_data)
        
    def process(self, data):
                
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
    
        # and sort it in p_t
        prepared_data = self._sort_dataframe(prepared_data, sorted_column, True)
    
        return prepared_data

    # encode an angle phi as the tuple (sin(phi), cos(phi))
    def _encode_angles(self, df, col):
        df_out = pd.DataFrame()
        for index, row in df.iterrows():
            row_frame = pd.DataFrame({col + "_sin": [np.sin(row[col])], col + "_cos": [np.cos(row[col])]})
            df_out = df_out.append(row_frame)

        df_out = df_out.reset_index(drop = True)
        return df_out


    def _sort_dataframe(self, df, col_name, inverted = False):
        df_out = pd.DataFrame()
    
        # make sure to use always the correct column for sorting (namely the one containing the Pt)
        sort_index = df.columns.get_loc(col_name)
        
        for index, row in df.iterrows():
            inrow = row.as_matrix()
            rowperm = np.argsort(inrow[sort_index])
        
            # if want it sorted inversely in col_name:
            if inverted:
                rowperm = rowperm[::-1]
        
            inrow_sorted = [inrow[cur_field][rowperm] for cur_field in range(len(inrow))]
                
            df_row = pd.DataFrame([inrow_sorted], columns = df.columns)
            df_out = df_out.append(df_row)
        
        df_out = df_out.reset_index(drop = True)
        return df_out
