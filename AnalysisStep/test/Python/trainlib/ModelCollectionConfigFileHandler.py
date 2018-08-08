import configparser
import inspect
import utils as utils
import trainlib.cuts
import re

from ConfigFileHandler import ConfigFileHandler
from ConfigFileUtils import ConfigFileUtils
from SimpleModel import SimpleModel
from ModelCollection import ModelCollection
from PCAWhiteningPreprocessor import PCAWhiteningPreprocessor
from FlexiblePCAWhiteningPreprocessor import FlexiblePCAWhiteningPreprocessor
from ConfigFileUtils import ConfigFileUtils
from config import TrainingConfig

class ModelCollectionConfigFileHandler(ConfigFileHandler):

    def __init__(self):
        ConfigFileHandler.__init__(self)

    def _parse_model(self, model_name):
        model_section = self.get_section('[' + model_name)
        base_model = eval(model_section['model_type'])
        preprocessor_name = model_section['preprocessor']
        preprocessor_section = self.get_section('<' + preprocessor_name)
        preprocessor_base = eval(preprocessor_section['preprocessor_type'])

        mod = base_model.from_config(model_section)
        pre = preprocessor_base.from_config(preprocessor_section)

        return mod, pre

    def _parse_model_collection(self, section, weight_path):
        cur_sect = self.get_section(section)
    
        mcoll_name = section

        H1_stream = ConfigFileUtils.parse_dict(cur_sect['H1_stream'], lambda x: eval(x))
        H0_stream = ConfigFileUtils.parse_dict(cur_sect['H0_stream'], lambda x: eval(x))

        H1_stream_unmixed = ConfigFileUtils.parse_dict(cur_sect['H1_stream_unmixed'], lambda x: eval(x))
        H0_stream_unmixed = ConfigFileUtils.parse_dict(cur_sect['H0_stream_unmixed'], lambda x: eval(x))
        
        H1_name = cur_sect['H1_name']
        H0_name = cur_sect['H0_name']

        model_names = ConfigFileUtils.parse_list(cur_sect['models'], lambda x: x)
    
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name, H1_stream_unmixed = H1_stream_unmixed, H0_stream_unmixed = H0_stream_unmixed)

        for model in model_names:
            mod, pre = self._parse_model(model)
            sett = TrainingConfig()
            mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        
        return mcoll

    def _serialize_model(self, model, preprocessor):    
        model_name = model.name
        preprocessor_name = preprocessor.name
        
        model_section = model.to_config(self)
        self.set_field(model_section, 'preprocessor', preprocessor_name)

        preprocessor.to_config(self)

    def _serialize_model_collection(self, mcoll):
        mcoll_name = mcoll.name
        H1_stream = mcoll.H1_stream
        H0_stream = mcoll.H0_stream

        H1_stream_unmixed = mcoll.H1_stream_unmixed
        H0_stream_unmixed = mcoll.H0_stream_unmixed

        H1_name = mcoll.H1_name
        H0_name = mcoll.H0_name

        model_names = mcoll.model_dict.keys()

        # make a new section
        self.new_section(mcoll_name)
        cur_sect = self.get_section(mcoll_name)
    
        cur_sect['H1_stream'] = ConfigFileUtils.serialize_dict(H1_stream, lambda x: inspect.getmodule(x).__name__ + "." + x.__name__)
        cur_sect['H0_stream'] = ConfigFileUtils.serialize_dict(H0_stream, lambda x: inspect.getmodule(x).__name__ + "." + x.__name__)

        cur_sect['H1_stream_unmixed'] = ConfigFileUtils.serialize_dict(H1_stream_unmixed, lambda x: inspect.getmodule(x).__name__ + "." + x.__name__)
        cur_sect['H0_stream_unmixed'] = ConfigFileUtils.serialize_dict(H0_stream_unmixed, lambda x: inspect.getmodule(x).__name__ + "." + x.__name__)
        
        cur_sect['H1_name'] = H1_name
        cur_sect['H0_name'] = H0_name

        cur_sect['models'] = ConfigFileUtils.serialize_list(model_names, lambda x: x)
    
        models = mcoll.model_dict.values()
        preprocessors = mcoll.preprocessor_dict.values()
        for model, preprocessor in zip(models, preprocessors):
            self._serialize_model(model, preprocessor)

    def _get_model_collection_list(self):
        raw_sections = self.get_sections()
        main_sections = [name for name in raw_sections if not (name.startswith('[') or name.startswith('<') or name.startswith('!'))]
        return main_sections

    def _get_model_list(self, model_collection):
        field = self.get_field(model_collection, 'models')
        return ConfigFileUtils.parse_list(field, lambda x: x)

    def ToConfiguration(self, mcoll):   
        for model in mcoll:
            self._serialize_model_collection(model)

    def GetModelCollection(self, weightpath = None):
        main_sections = self._get_model_collection_list()
        mcolls = []
    
        for section in main_sections:
            mcolls.append(self._parse_model_collection(section, weightpath))
        
        return mcolls

    # ------------------------------------
    # these methods are only used by the ConfigFileSweeper
    # ------------------------------------

    def AddListEntry(self, section, field, to_add, processor, reverse_processor):
        if 'all' in section:
            available_sections = self.get_sections()
            if section.startswith('[') or section.startswith('<'):
                r = re.compile('^\\' + section[0])
                available_sections = filter(r.match, available_sections)
            for available_section in available_sections:
                if self.has_field(available_section, field):
                    self.AddListEntry(available_section, field, to_add, processor, reverse_processor)
        else:
            cur_list = ConfigFileUtils.parse_list(self.get_field(section, field), processor)
            cur_list += to_add
            self.set_field(section, field, ConfigFileUtils.serialize_list(cur_list, reverse_processor))

    def AddDictEntry(self, section, field, to_add, processor, reverse_processor):
        if 'all' in section:
            available_sections = self.get_sections()
            if section.startswith('[') or section.startswith('<'):
                r = re.compile('^\\' + section[0])
                available_sections = filter(r.match, available_sections)
            for available_section in available_sections:
                if self.has_field(available_section, field):
                    self.AddDictEntry(available_section, field, to_add, processor, reverse_processor)
        else:
            cur_dict = ConfigFileUtils.parse_dict(self.get_field(section, field), processor)
            cur_dict.update(to_add)
            self.set_field(section, field, ConfigFileUtils.serialize_dict(cur_dict, reverse_processor))

    def SetList(self, section, field, in_list, processor):
        if 'all' in section:
            available_sections = self.get_sections()
            if section.startswith('[') or section.startswith('<'):
                r = re.compile('^\\' + section[0])
                available_sections = filter(r.match, available_sections)
            for available_section in available_sections:
                if self.has_field(available_section, field):
                    self.SetList(available_section, field, in_list, processor)
        else:
            self.set_field(section, field, ConfigFileUtils.serialize_list(in_list, processor))

    def SetDict(self, section, field, in_dict, processor):
        if 'all' in section:
            available_sections = self.get_sections()
            if section.startswith('[') or section.startswith('<'):
                r = re.compile('^\\' + section[0])
                available_sections = filter(r.match, available_sections)
            for available_section in available_sections:
                if self.has_field(available_section, field):
                    self.SetDict(available_section, field, in_dict, processor)
        else:
            self.set_field(section, field, ConfigFileUtils.serialize_dict(in_dict, processor))

    # ------------------------------------

    def ModelCollectionDiff(self, model_collection, parameter, new_value):
        self.set_field(model_collection, parameter, new_value)

    def ModelDiff(self, model, parameter, new_value):
        self.set_field('[' + model, parameter, new_value)

    def ModelGlobalDiff(self, parameter, new_value):
        for model_collection in self._get_model_collection_list():
            for model in self._get_model_list(model_collection):
                self.ModelDiff(model, parameter, new_value)

    def ModelCollectionGlobalDiff(self, parameter, new_value):
        for model_collection in self._get_model_collection_list():
            self.ModelCollectionDiff(model_collection, parameter, new_value)

    def GetModelCollectionParameter(self, model_collection, parameter):
        return self.get_field(model_collection, parameter)

    def GetModelParameter(self, model, parameter):
        return self.get_field('[' + model, parameter)

    def AddInputParameterLocally(self, model, parameter):
        current = ConfigFileUtils.parse_list(self.GetModelParameter(model, 'input_columns'), lambda x: x)
        current += [parameter]
        self.ModelDiff(model, 'input_columns', current + ', ' + parameter)

    def AddInputParameterGlobally(self, parameter):
        for model_collection in self._get_model_collection_list():
            for model in self._get_model_list(model_collection):
                self.AddInputParameterLocally(model, parameter)

    def GetInputParameterList(self, model):
        current = self.GetModelParameter(model, 'input_columns')
        return ConfigFileUtils.parse_list(current, lambda x: x)

    def SetHyperparametersLocally(self, model, hyperparameters):
        self.ModelDiff(model, 'hyperparameters', ConfigFileUtils.serialize_dict(hyperparameters, lambda x: str(x)))

    def GetHyperparameters(self, model):
        return ConfigFileUtils.parse_dict(self.GetModelParameter(model, 'hyperparameters'), lambda x: float(x))
        
    def SetHyperparametersGlobally(self, hyperparameters):
        for model_collection in self._get_model_collection_list():
            for model in self._get_model_list(model_collection):
                self.SetHyperparametersLocally(model, hyperparameters)
        
