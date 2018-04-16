import pandas as pd
import numpy as np
from sklearn.decomposition import PCA
import pickle
import re
import copy

import cuts
from Preprocessor import Preprocessor
from ConfigFileUtils import ConfigFileUtils
from PCAWhiteningPreprocessor import PCAWhiteningPreprocessor

class FlexiblePCAWhiteningPreprocessor(Preprocessor):
    def __init__(self, name, nonperiodic_columns, periodic_columns, cuts, cuts_s = None):
        self.name = name
        self.nonperiodic_columns = nonperiodic_columns
        self.periodic_columns = periodic_columns
        self.cuts = cuts
        self.cuts_s = cuts_s
        self.base_preprocessor = None
        self.processed_columns = None
        self.last_indices = None

        print "FlexiblePCAPreprocessor set up for " + str(self.number_processed_columns()) + " inputs"

    def number_processed_columns(self):
        return len(self.nonperiodic_columns) + 2 * len(self.periodic_columns)

    @classmethod
    def from_config(cls, config_section):
        preprocessor_name = re.sub('[<>]', '', config_section.name)
        nonperiodic_columns = ConfigFileUtils.parse_list(config_section['nonperiodic_columns'], lambda x: x)
        periodic_columns = ConfigFileUtils.parse_list(config_section['periodic_columns'], lambda x: x)

        # filter away empty entries
        nonperiodic_columns = filter(None, nonperiodic_columns)
        periodic_columns = filter(None, periodic_columns)

        preprocessor_cuts, cuts_s = ConfigFileUtils.parse_lambda_s(config_section['preprocessor_cuts'])
        
        obj = cls(name = preprocessor_name, nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts, cuts_s = cuts_s)
        
        return obj

    def to_config(self, confhandler):
        section_name = '<' + self.name
        confhandler.new_section(section_name)
        
        confhandler.set_field(section_name, 'preprocessor_type', 'FlexiblePCAWhiteningPreprocessor')
        confhandler.set_field(section_name, 'nonperiodic_columns', ConfigFileUtils.serialize_list(self.nonperiodic_columns, lambda x: x))
        confhandler.set_field(section_name, 'periodic_columns', ConfigFileUtils.serialize_list(self.periodic_columns, lambda x: x))
        confhandler.set_field(section_name, 'preprocessor_cuts', ConfigFileUtils.serialize_lambda_s(self.cuts, self.cuts_s))
    
    def setup_generator(self, datagen, len_setupdata):
        self.len_setupdata = len_setupdata
        
        # draw some data from the generator
        extracted_data = []
        extracted_rows = 0
        
        for data in datagen:
            extracted_data.append(data)
            extracted_rows += len(data)
            
            if extracted_rows > self.len_setupdata:
                break
                
        input_data = pd.concat(extracted_data)
        input_data = input_data.reset_index(drop = True)
        
        self.setup(input_data)
    
    def setup(self, data):
        # first perform the row selection
        cut_data = self._row_cut(data)
        
        # then prepare the data and the base preprocessor
        prepared_data = self.prepare_data(cut_data)
        self.processed_columns = prepared_data.columns
        
        self.base_preprocessor = PCAWhiteningPreprocessor(self.name, self.processed_columns, cuts.no_cut, None)
        self.base_preprocessor.setup(prepared_data)
        
    def get_last_indices(self):
        return self.last_indices
    
    def process(self, data):
        cut_data = self._row_cut(data)        
        self.last_indices = cut_data.index
        
        prepared_data = self.prepare_data(cut_data)
                
        return self.base_preprocessor.process(prepared_data)
    
    def save(self, folder, filename):
        self.base_preprocessor.save(folder, filename)
        
    def load(self, folder, filename):
        self.base_preprocessor.load(folder, filename)
    
    # puts the data in a format such that PCAWhiteningPreprocessor can act on them
    def prepare_data(self, data):
        prepared_data = pd.DataFrame()

        for column in self.nonperiodic_columns:
            #print "nonperiodic column: " + column
            cf = self.extract_column(data, column)
            prepared_data = pd.concat([prepared_data, cf], axis = 1)

        for column in self.periodic_columns:
            #print "periodic column: " + column
            cf = self.extract_column(data, column)
            cf = self.encode_periodic_column(cf)
            prepared_data = pd.concat([prepared_data, cf], axis = 1)

        return prepared_data

    def extract_order(self, df, sorted_column, columns, order):
        def get_index(row, order, sorted_column):
            sorted_column = row[sorted_column]
            if order >= len(sorted_column):
                return -1
            else:
                return np.flipud(np.argsort(sorted_column))[order]

        index_column = pd.DataFrame(df.transform(lambda row: get_index(row, order, sorted_column), axis = 1, raw = True))
        index_column.columns = ["index"]
        df_temp = pd.concat([index_column, df], axis = 1)

        def get_element(row, column_name):
            if row["index"] == -1:
                return 0
            else:
                return row[column_name][row["index"]]

        extracted_cols = pd.DataFrame()
        for column in columns:
            extracted_col = pd.DataFrame(df_temp.transform(lambda row: get_element(row, column), axis = 1, raw = True))
            extracted_col.columns = [column + "_" + str(order)]
            extracted_cols = pd.concat([extracted_cols, extracted_col], axis = 1)

        return extracted_cols
    
    def extract_order_filtered(self, df, sorted_column, columns, order):
        extracted_raw = self.extract_order(df, sorted_column, columns, order)

        if any("Jet" in col for col in extracted_raw.columns):
            # apply the jet-pt cut
            mask_column = self.extract_order(df, "JetPt", ["JetPt"], order)
            mask = mask_column < 30.0
            extracted_raw[mask.as_matrix()] = 0.0

        return extracted_raw
    
    def extract_column(self, df, colstring):
        if '[' in colstring and ']' in colstring:
            col, sorted_col_order, _ = re.split('[\[\]]', colstring)
            sorted_col, order = re.split('\|', sorted_col_order)
            cf = self.extract_order_filtered(df, sorted_col, [col], int(order))
        else:
            cf = df[[colstring]]
        return cf
    
    def encode_periodic_column(self, df):
        of = pd.DataFrame()
        sincol = df.apply(np.sin, raw = True)
        sincol.columns = [col + "_sin" for col in df.columns]
        coscol = df.apply(np.cos, raw = True)
        coscol.columns = [col + "_cos" for col in df.columns]
        of = pd.concat([sincol, coscol], axis = 1)

        return of
    
    def _row_cut(self, data):
        output_data = data.loc[data.apply(self.cuts, axis = 1)]
        return output_data
