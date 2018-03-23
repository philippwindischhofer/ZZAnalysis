import configparser
import inspect

import utils as utils
import cuts as cuts
from SimpleModel import SimpleModel
from ModelFactory import ModelFactory
from ModelCollection import ModelCollection
from PCAWhiteningPreprocessor import PCAWhiteningPreprocessor
from config import TrainingConfig, ConfigFileConfig

class ConfigFileHandler:

    def __init__(self):
        self.cuts_translation = ConfigFileConfig.cuts_translation
        self.model_translation = ConfigFileConfig.model_translation

    def process_dict(self, rawstring, translation_dict):
        retdict = {}
        for source in rawstring.split(','):
            key, cmd = source.split(':')
            retdict[key.strip()] = translation_dict.get(cmd.strip())
        
        return retdict    

    def process_list(self, rawstring):
        retlist = []
        for entry in rawstring.split(','):
            retlist.append(entry.strip())
            
        return retlist

    def process_model(self, config, model):
        model_section = config['[' + model]
    
        model_name = model
        base_model = self.model_translation[model_section['base_model']]
        input_columns = self.process_list(model_section['input_columns'])
        preprocessor_cuts = eval(model_section['preprocessor_cuts'])
    
        pre = PCAWhiteningPreprocessor(processed_columns = input_columns, cuts = preprocessor_cuts)
    
        mod = base_model(model_name, input_columns)
        mod.build()
    
        return mod, pre

    def process_model_collection(self, config, section, weight_path):
        cur_sect = config[section]
    
        mcoll_name = section
        H1_stream = self.process_dict(cur_sect['H1_stream'], self.cuts_translation)
        H0_stream = self.process_dict(cur_sect['H0_stream'], self.cuts_translation)
        model_names = self.process_list(cur_sect['models'])
    
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        for model in model_names:
            mod, pre = self.process_model(config, model)
            sett = TrainingConfig()
            mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        
        return mcoll

    def assemble_lambda_string(self, func):
        func_string = str(inspect.getsourcelines(func)[0])
        func_string = func_string.strip("['\\n']").split(" = ")[1]

        return func_string

    def assemble_dict(self, in_dict, translation_dict):
        inverse_translation = {val: key for key, val in translation_dict.iteritems()}
    
        out_string = ""
    
        for key, val in in_dict.iteritems():
            out_string += key + ": "
            out_string += inverse_translation.get(val) + ", "
    
        # cut away the last two (superfluous) characters
        return out_string[:-2]

    def assemble_list(self, in_list):
        out_string = ""
    
        for entry in in_list:
            out_string += entry + ", "
        
        return out_string[:-2]

    def make_config_model(self, config, model, preprocessor):    
        model_name = model.name
        section_name = '[' + model_name + ']'
        base_model = model.__class__.__name__
        input_columns = self.assemble_list(model.input_columns)
        preprocessor_cuts = self.assemble_lambda_string(preprocessor.cuts)
        
        # make a new section for the model in the config file
        config[section_name] = {}
        cur_sect = config[section_name]
    
        cur_sect['base_model'] = base_model
        cur_sect['input_columns'] = input_columns
        cur_sect['preprocessor_cuts'] = preprocessor_cuts

    def make_config_model_collection(self, config, mcoll):
        mcoll_name = mcoll.name
        H1_stream = mcoll.H1_stream
        H0_stream = mcoll.H0_stream
        model_names = mcoll.model_dict.keys()
    
        # make a new section in the config file
        config[mcoll_name] = {}
        cur_sect = config[mcoll_name]
    
        cur_sect['H1_stream'] = self.assemble_dict(H1_stream, self.cuts_translation)
        cur_sect['H0_stream'] = self.assemble_dict(H0_stream, self.cuts_translation)
        cur_sect['models'] = self.assemble_list(model_names)
    
        models = mcoll.model_dict.values()
        preprocessors = mcoll.preprocessor_dict.values()
        for model, preprocessor in zip(models, preprocessors):
            self.make_config_model(config, model, preprocessor)

    def ModelCollectionsToConfigFile(self, mcoll, configpath):
        config = configparser.ConfigParser()
    
        for model in mcoll:
            self.make_config_model_collection(config, model)
        
        with open(configpath, 'w') as configfile:
            config.write(configfile)

    def ModelCollectionsFromConfigFile(self, configpath, weightpath = None):
        print "attempting to load model collections from " + configpath
        config = configparser.ConfigParser()
        config.read(configpath)
    
        raw_sections = config.sections()
        main_sections = [name for name in raw_sections if not name.startswith('[')]
    
        mcolls = []
    
        for section in main_sections:
            mcolls.append(self.process_model_collection(config, section, weightpath))
        
        return mcolls
