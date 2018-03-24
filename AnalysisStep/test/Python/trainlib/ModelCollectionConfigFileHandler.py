import configparser

import utils as utils
import cuts as cuts
from ConfigFileHandler import ConfigFileHandler
from SimpleModel import SimpleModel
from ModelFactory import ModelFactory
from ModelCollection import ModelCollection
from PCAWhiteningPreprocessor import PCAWhiteningPreprocessor
from config import TrainingConfig, ConfigFileConfig

class ModelCollectionConfigFileHandler(ConfigFileHandler):

    def __init__(self):
        ConfigFileHandler.__init__(self)
        self.cuts_translation = ConfigFileConfig.cuts_translation
        self.model_translation = ConfigFileConfig.model_translation

    def _process_model(self, model):
        model_section = self._get_section('[' + model)
    
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
        cur_sect = self._get_section(section)
    
        mcoll_name = section
        H1_stream = self._process_dict(cur_sect['H1_stream'], lambda x: self._dict_translator(x, self.cuts_translation))
        H0_stream = self._process_dict(cur_sect['H0_stream'], lambda x: self._dict_translator(x, self.cuts_translation))
        model_names = self._process_list(cur_sect['models'], lambda x: x)
    
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        for model in model_names:
            mod, pre = self._process_model(model)
            sett = TrainingConfig()
            mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        
        return mcoll

    def _make_config_model(self, model, preprocessor):    
        model_name = model.name
        section_name = '[' + model_name
        base_model = model.__class__.__name__
        input_columns = self._assemble_list(model.input_columns, lambda x: x)
        preprocessor_cuts = self._assemble_lambda_string(preprocessor.cuts)
        hyperparameters = self._assemble_dict(model.hyperparameters, lambda x: str(x))

        # make a new section for the model in the config file
        self._new_section(section_name)
        cur_sect = self._get_section(section_name)
    
        cur_sect['base_model'] = base_model
        cur_sect['input_columns'] = input_columns
        cur_sect['preprocessor_cuts'] = preprocessor_cuts
        cur_sect['hyperparameters'] = hyperparameters

    def _make_config_model_collection(self, mcoll):
        mcoll_name = mcoll.name
        H1_stream = mcoll.H1_stream
        H0_stream = mcoll.H0_stream
        model_names = mcoll.model_dict.keys()

        # make a new section
        self._new_section(mcoll_name)
        cur_sect = self._get_section(mcoll_name)
    
        cur_sect['H1_stream'] = self._assemble_dict(H1_stream, lambda x: self._idict_translator(x, self.cuts_translation))
        cur_sect['H0_stream'] = self._assemble_dict(H0_stream, lambda x: self._idict_translator(x, self.cuts_translation))
        cur_sect['models'] = self._assemble_list(model_names, lambda x: x)
    
        models = mcoll.model_dict.values()
        preprocessors = mcoll.preprocessor_dict.values()
        for model, preprocessor in zip(models, preprocessors):
            self._make_config_model(model, preprocessor)

    def _get_model_collection_list(self):
        raw_sections = self._get_sections()
        main_sections = [name for name in raw_sections if not name.startswith('[')]
        return main_sections

    def _get_model_list(self, model_collection):
        field = self._get_field(model_collection, 'models')
        return self._process_list(field, lambda x: x)

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
        field = self._get_field(model_collection, 'models')
        return self.process_list(field, lambda x: x)

    def ModelCollectionDiff(self, model_collection, parameter, new_value):
        self._set_field(model_collection, parameter, new_value)

    def ModelDiff(self, model, parameter, new_value):
        self._set_field('[' + model, parameter, new_value)

    def ModelGlobalDiff(self, parameter, new_value):
        for model_collection in self._get_model_collection_list():
            for model in self._get_model_list(model_collection):
                self.ModelDiff(model, parameter, new_value)

    def ModelCollectionGlobalDiff(self, parameter, new_value):
        for model_collection in self._get_model_collection_list():
            self.ModelCollectionDiff(model_collection, parameter, new_value)

    def GetModelCollectionParameter(self, model_collection, parameter):
        return self._get_field(model_collection, parameter)

    def GetModelParameter(self, model, parameter):
        return self._get_field('[' + model, parameter)

    def AddInputParameterLocally(self, model, parameter):
        current = self.GetModelParameter(model, 'input_columns')
        self.ModelDiff(model, 'input_columns', current + ', ' + parameter)

    def AddInputParameterGlobally(self, parameter):
        for model_collection in self._get_model_collection_list():
            for model in self._get_model_list(model_collection):
                self.AddInputParameterLocally(model, parameter)

    def SetHyperparametersLocally(self, model, hyperparameters):
        self.ModelDiff(model, 'hyperparameters', self._assemble_dict(hyperparameters, lambda x: str(x)))

    def GetHyperparameters(self, model):
        return self._process_dict(self.GetModelParameter(model, 'hyperparameters'), lambda x: float(x))
        
    def SetHyperparametersGlobally(self, hyperparameters):
        for model_collection in self._get_model_collection_list():
            for model in self._get_model_list(model_collection):
                self.SetHyperparametersLocally(model, hyperparameters)
        
