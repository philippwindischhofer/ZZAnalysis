import configparser
import inspect

import utils as utils
import cuts as cuts
from SimpleModel import SimpleModel
from ModelFactory import ModelFactory
from ModelCollection import ModelCollection
from PCAWhiteningPreprocessor import PCAWhiteningPreprocessor
from config import TrainingConfig, ConfigFileConfig

def _cut_processor(inval):
    inverse_translation = {val: key for key, val in ConfigFileConfig.cuts_translation.iteritems()}
    return inverse_translation.get(inval)

def _icut_processor(inval):
    return ConfigFileConfig.cuts_translation[inval]

class ConfigFileHandler:

    def __init__(self):
        self.cuts_translation = ConfigFileConfig.cuts_translation
        self.model_translation = ConfigFileConfig.model_translation
        self.config = configparser.ConfigParser()

    def _process_dict(self, rawstring, processor):
        retdict = {}
        for source in rawstring.split(','):
            key, cmd = source.split(':')
            retdict[key.strip()] = processor(cmd.strip())
        
        return retdict    

    def _process_list(self, rawstring, processor):
        retlist = []
        for entry in rawstring.split(','):
            retlist.append(processor(entry.strip()))
            
        return retlist

    def _process_model(self, model):
        model_section = self.config['[' + model]
    
        model_name = model
        base_model = self.model_translation[model_section['base_model']]
        input_columns = self._process_list(model_section['input_columns'], lambda x: x)
        hyperparameters = self._process_dict(model_section['hyperparameters'], lambda x: float(x))
        preprocessor_cuts = eval(model_section['preprocessor_cuts'])
    
        pre = PCAWhiteningPreprocessor(processed_columns = input_columns, cuts = preprocessor_cuts)
    
        mod = base_model(model_name, input_columns)
        mod.build(hyperparameters)
    
        return mod, pre

    def _process_model_collection(self, section, weight_path):
        cur_sect = self.config[section]
    
        mcoll_name = section
        H1_stream = self._process_dict(cur_sect['H1_stream'], _icut_processor)
        H0_stream = self._process_dict(cur_sect['H0_stream'], _icut_processor)
        model_names = self._process_list(cur_sect['models'], lambda x: x)
    
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        for model in model_names:
            mod, pre = self._process_model(model)
            sett = TrainingConfig()
            mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        
        return mcoll

    def _assemble_lambda_string(self, func):
        func_string = str(inspect.getsourcelines(func)[0])
        func_string = func_string.strip("['\\n']").split(" = ")[1]

        return func_string

    def _assemble_dict(self, in_dict, processor):
    
        out_string = ""
    
        for key, val in in_dict.iteritems():
            out_string += key + ": "
            out_string += processor(val) + ", "
    
        # cut away the last two (superfluous) characters
        return out_string[:-2]

    def _assemble_list(self, in_list, processor):
        out_string = ""
    
        for entry in in_list:
            out_string += processor(entry) + ", "
        
        return out_string[:-2]

    def _make_config_model(self, model, preprocessor):    
        model_name = model.name
        section_name = '[' + model_name + ']'
        base_model = model.__class__.__name__
        input_columns = self._assemble_list(model.input_columns, lambda x: x)
        preprocessor_cuts = self._assemble_lambda_string(preprocessor.cuts)
        hyperparameters = self._assemble_dict(model.hyperparameters, lambda x: str(x))

        # make a new section for the model in the config file
        self.config[section_name] = {}
        cur_sect = self.config[section_name]
    
        cur_sect['base_model'] = base_model
        cur_sect['input_columns'] = input_columns
        cur_sect['preprocessor_cuts'] = preprocessor_cuts
        cur_sect['hyperparameters'] = hyperparameters

    def _make_config_model_collection(self, mcoll):
        mcoll_name = mcoll.name
        H1_stream = mcoll.H1_stream
        H0_stream = mcoll.H0_stream
        model_names = mcoll.model_dict.keys()
    
        # make a new section in the config file
        self.config[mcoll_name] = {}
        cur_sect = self.config[mcoll_name]
    
        cur_sect['H1_stream'] = self._assemble_dict(H1_stream, _cut_processor)
        cur_sect['H0_stream'] = self._assemble_dict(H0_stream, _cut_processor)
        cur_sect['models'] = self._assemble_list(model_names, lambda x: x)
    
        models = mcoll.model_dict.values()
        preprocessors = mcoll.preprocessor_dict.values()
        for model, preprocessor in zip(models, preprocessors):
            self._make_config_model(model, preprocessor)

    def _get_model_collection_list(self):
        raw_sections = self.config.sections()
        main_sections = [name for name in raw_sections if not name.startswith('[')]
        return main_sections

    def _get_model_list(self, model_collection):
        return self._process_list(self.config[model_collection]['models'], lambda x: x)

    def LoadConfiguration(self, configpath):
        print "attempting to load model collections from " + configpath
        self.config.read(configpath)

    def SaveConfiguration(self, configpath):
        with open(configpath, 'w') as configfile:
            self.config.write(configfile)

    def ToConfiguration(self, mcoll):   
        for model in mcoll:
            self._make_config_model_collection(model)

    def GetModelCollection(self, weightpath = None):
        main_sections = self._get_model_collection_list()
        mcolls = []
    
        for section in main_sections:
            mcolls.append(self._process_model_collection(section, weightpath))
        
        return mcolls

    def _get_models(self, model_collection):
        return self.process_list(self.config[model_collection]['models'], lambda x: x)

    def GlobalDiff(self, parameter, new_value):
        self.config[model_collection][parameter] = new_value

    def ModelCollectionDiff(self, model_collection, parameter, new_value):
        self.config[model_collection][parameter] = new_value

    def ModelDiff(self, model, parameter, new_value):
        self.config['[' + model][parameter] = new_value

    def ModelGlobalDiff(self, parameter, new_value):
        for model_collection in self._get_model_collection_list():
            for model in self._get_model_list(model_collection):
                self.ModelDiff(model, parameter, new_value)

    def ModelCollectionGlobalDiff(self, parameter, new_value):
        for model_collection in self._get_model_collection_list():
            self.ModelCollectionDiff(model_collection, parameter, new_value)

    def GetModelCollectionParameter(self, model_collection, parameter):
        return self.config[model_collection][parameter]

    def GetModelParameter(self, model, parameter):
        return self.config['[' + model][parameter]

    def AddInputParameterLocally(self, model, parameter):
        current = self.GetModelParameter(model, 'input_columns')
        self.ModelDiff(model, 'input_columns', current + ', ' + parameter)

    def AddInputParameterGlobally(self, parameter):
        for model_collection in self._get_model_collection_list():
            for model in self._get_model_list(model_collection):
                self.AddInputParameterLocally(model, parameter)

    def SetHyperparametersLocally(self, model, hyperparameters):
        self.ModelDiff(model, 'hyperparameters', self._assemble_dict(hyperparameters, lambda x: str(x)))

    def SetHyperparametersGlobally(self, hyperparameters):
        for model_collection in self._get_model_collection_list():
            for model in self._get_model_list(model_collection):
                self.SetHyperparametersLocally(model, hyperparameters)
        
