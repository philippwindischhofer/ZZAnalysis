from Preprocessor import Preprocessor
from PCAWhiteningPreprocessor import PCAWhiteningPreprocessor
import pandas as pd
import numpy as np
import re

class PNPPreprocessor(Preprocessor):
    def __init__(self, name, nonperiodic_columns, periodic_columns, preprocessor_cuts, preprocessor_basetype):
        self.name = name
        self.nonperiodic_columns = nonperiodic_columns
        self.periodic_columns = periodic_columns
        self.preprocessor_cuts = preprocessor_cuts
        
        # each periodic column will be encoded with two values:
        self.periodic_columns_encoded = []
        for periodic_column in self.periodic_columns:
            self.periodic_columns_encoded.append(periodic_column + "_sin")
            self.periodic_columns_encoded.append(periodic_column + "_cos")
            
        self.processed_columns = self.nonperiodic_columns + self.periodic_columns_encoded
         
        print self.processed_columns    
        
        # all cuts are already handled at this level (the topmost level)
        self.pre = preprocessor_basetype(self.name, self.processed_columns, cuts.no_cut)
    
    @classmethod
    def from_config(cls, config_section):
        preprocessor_name = re.sub('[<>]', '', config_section.name)
        nonperiodic_inputs = ConfigFileUtils.parse_list(config_section['processed_nonperiodic_columns'], lambda x: x.encode("ascii"))
        periodic_inputs = ConfigFileUtils.parse_list(config_section['processed_periodic_columns'], lambda x: x.encode("ascii"))
        preprocessor_cuts = ConfigFileUtils.parse_lambda(config_section['preprocessor_cuts'])
        preprocessor_type = eval(config_section['scalar_preprocessor_type'])
        
        obj = cls(name = preprocessor_name, nonperiodic_columns = nonperiodic_inputs, periodic_columns = periodic_inputs, 
                      preprocessor_cuts = preprocessor_cuts, preprocessor_basetype = preprocessor_type)
    
    def to_config(self, confhandler):
        section_name = '<' + self.name
        confhandler.new_section(section_name)
        
        confhandler.set_field(section_name, 'preprocessor_type', 'PNPPreprocessor')
        confhandler.set_field(section_name, 'processed_nonperiodic_columns', ConfigFileUtils.serialize_list(self.nonperiodic_columns, lambda x: x))
        confhandler.set_field(section_name, 'processed_periodic_columns', ConfigFileUtils.serialize_list(self.periodic_columns))
        confhandler.set_field(section_name, 'preprocessor_cuts', ConfigFileUtils.serialize_lambda(self.preprocessor_cuts))
        confhandler.set_field(section_name, 'scalar_preprocessor_type', self.pre.__class__.__name__)
        
    def _prepare_data(self, data):
        periodic_data_encoded = []
        
        for periodic_column in self.periodic_columns:
            periodic_data_encoded.append(self._encode_angles(data, periodic_column))
            
        prepared_data = pd.concat(periodic_data_encoded + [data[self.nonperiodic_columns]], axis = 1)
        
        return prepared_data
        
    def _encode_angles(self, df, col):
        df_out = pd.DataFrame()
        
        sin_encoding = df[col].apply(lambda x: np.sin(x))
        cos_encoding = df[col].apply(lambda x: np.cos(x))
        
        df_out[col + "_sin"] = sin_encoding
        df_out[col + "_cos"] = cos_encoding
        
        return df_out
    
    def setup_generator(self, datagen, len_setupdata):
        self.len_setupdata = len_setupdata
        extracted_data = []
        extracted_rows = 0
        
        for data in datagen:
            extracted_data.append(data)
            extracted_rows += len(data)
            
            if extracted_rows > self.len_setupdata:
                break
                
        print "setting up PNPPreprocessor on " + str(extracted_rows) + " events"
        
        input_data = pd.concat(extracted_data)
        input_data = input_data.reset_index(drop = True)
        
        self.setup(input_data)
    
    def setup(self, data):
        prepared_data = self._prepare_data(data)
        cut_data = self._rowcol_cut(prepared_data)
        
        self.pre.setup(cut_data)
    
    def process(self, data):
        prepared_data = self._prepare_data(data)
        cut_data = se
