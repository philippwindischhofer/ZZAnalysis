import numpy as np
import pandas as pd
import re

from Preprocessor import Preprocessor
from PCAWhiteningPreprocessor import PCAWhiteningPreprocessor
from RNNPreprocessor import RNNPreprocessor
from ConfigFileUtils import ConfigFileUtils
import cuts

class CombinedPreprocessor(Preprocessor):
    def __init__(self, name, scalar_inputs, scalar_preprocessor_basetype, list_inputs, list_preprocessor_basetype, input_cuts):
        self.name = name
        self.scalar_inputs = scalar_inputs
        self.list_inputs = list_inputs # note: list inputs is of the form {input_group_name: ["input_1", ...], ...}
        self.cuts = input_cuts
        self.last_indices = None
        
        self.list_preprocessors = {}
        self.scalar_preprocessor = scalar_preprocessor_basetype(self.name + "_" + 'scalar_inputs', self.scalar_inputs, cuts.no_cut)
        
        self.processed_columns = []
        
        # construct the RNN preprocessors from the passed dictionary of list inputs:
        for input_name, input_columns in self.list_inputs.iteritems():
            periodic_columns = []
            nonperiodic_columns = []
            sorted_column = None
            for input_column in input_columns:
                # listen for the keyword "Phi" to classify input variables as being periodic
                if "Phi" in input_column:
                    periodic_columns.append(input_column)
                else:
                    nonperiodic_columns.append(input_column)
                    
                # listen for the keyword "Pt" to set the column as the one that is sorted
                if "Pt" in input_column:
                    sorted_column = input_column
                    
            print("for list input group '" + input_name + "': assigned periodic inputs " + str(periodic_columns) + 
                " and nonperiodic inputs " + str(nonperiodic_columns) + ", sorting according to " + sorted_column)
            
            list_pre = list_preprocessor_basetype(self.name + "_" + input_name, nonperiodic_columns, periodic_columns, sorted_column, cuts.no_cut, scalar_preprocessor_basetype)
            self.list_preprocessors[input_name] = list_pre
            
            self.processed_columns += nonperiodic_columns
            self.processed_columns += periodic_columns
            
        self.processed_columns += self.scalar_inputs
        print "total processed columns: " + str(self.processed_columns)
            
    @classmethod
    def from_config(cls, config_section):
        #preprocessor_translation = ConfigFileConfig.preprocessor_translation

        preprocessor_name = re.sub('[<>]', '', config_section.name)
        scalar_inputs = ConfigFileUtils.parse_list(config_section['processed_scalar_columns'], lambda x: x.encode("ascii"))
        #scalar_preprocessor_type = preprocessor_translation[config_section['scalar_preprocessor_type']]
        scalar_preprocessor_type = eval(config_section['scalar_preprocessor_type'])
        #list_preprocessor_type = preprocessor_translation[config_section['list_preprocessor_type']]
        list_preprocessor_type = eval(config_section['list_preprocessor_type'])
        list_inputs = ConfigFileUtils.parse_dict(config_section['processed_list_columns'], 
                                                 lambda x: ConfigFileUtils.parse_list(x, lambda y: y.encode("ascii")))
        cuts = ConfigFileUtils.parse_lambda(config_section['preprocessor_cuts'])

        obj = cls(name = preprocessor_name, scalar_inputs = scalar_inputs, scalar_preprocessor_basetype = scalar_preprocessor_type, list_inputs = list_inputs, list_preprocessor_basetype = list_preprocessor_type, input_cuts = cuts)

        return obj

    def to_config(self, confhandler):
        section_name = '<' + self.name
        confhandler.new_section(section_name)

        confhandler.set_field(section_name, 'preprocessor_type', 'CombinedPreprocessor')
        confhandler.set_field(section_name, 'processed_scalar_columns', ConfigFileUtils.serialize_list(self.scalar_inputs, lambda x: x))
        confhandler.set_field(section_name, 'scalar_preprocessor_type', self.scalar_preprocessor.__class__.__name__)
        confhandler.set_field(section_name, 'list_preprocessor_type', self.list_preprocessors.values()[0].__class__.__name__)
        confhandler.set_field(section_name, 'processed_list_columns', ConfigFileUtils.serialize_dict(self.list_inputs, lambda x: ConfigFileUtils.serialize_list(x, lambda y: y)))
        confhandler.set_field(section_name, 'preprocessor_cuts', ConfigFileUtils.serialize_lambda(self.cuts))                              

    def setup_generator(self, datagen, len_setupdata):
        self.len_setupdata = len_setupdata
        extracted_data = []
        extracted_rows = 0
        
        for data in datagen:
            extracted_data.append(data)
            extracted_rows += len(data)
            
            if extracted_rows > self.len_setupdata:
                break
                
        print "setting up preprocessor on " + str(extracted_rows) + " events"
        
        input_data = pd.concat(extracted_data)
        input_data = input_data.reset_index(drop = True)
        
        self.setup(input_data)
    
    def setup(self, data):
        cut_data = self._rowcol_cut(data)
        print cut_data.columns
        # in turn, set up all the list-type preprocessors as well as the separate scalar one
        print "setting up scalar preprocessor"
        self.scalar_preprocessor.setup(cut_data)
        
        for name, pre in self.list_preprocessors.iteritems():
            print "setting up list preprocessor for '" + name + "'"
            pre.setup(cut_data)
    
    def process(self, data):
        cut_data = self._rowcol_cut(data)
        self.last_indices = cut_data.index
        
        # call each preprocessor in turn, and in the end combine all their outputs into the final dictionary object
        retval = self.scalar_preprocessor.process(cut_data)
                
        for name, pre in self.list_preprocessors.iteritems():
            list_output = pre.process(cut_data)
            retval.update(list_output)
            
        return retval
    
    def get_last_indices(self):
        return self.last_indices
    
    def save(self, folder, filename):
        # save separately the scalar preprocessor as well as those for the list inputs
        self.scalar_preprocessor.save(folder, "scalar_" + filename)
        
        for name, pre in self.list_preprocessors.iteritems():
            pre.save(folder, "list_" + name + "_" + filename)
    
    def load(self, folder, filename):
        # load them back separately as well
        self.scalar_preprocessor.load(folder, "scalar_" + filename)
        
        for name, pre in self.list_preprocessor.iteritems():
            pre.load(folder, "list_" + name + "_" + filename)
            
    def _rowcol_cut(self, data):
        data = data.loc[data.apply(self.cuts, axis = 1)]
        
        output_data = data.loc[:, self.processed_columns]
        
        return output_data
