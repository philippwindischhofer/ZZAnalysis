import configparser
import numpy as np
import os
import re
import copy
import sys

from trainlib.config import Config
from trainlib.SimpleModel import SimpleModel
from trainlib.ModelFactory import ModelFactory
from trainlib.ConfigFileHandler import ConfigFileHandler
from trainlib.ModelCollectionConfigFileHandler import ModelCollectionConfigFileHandler
from trainlib.ConfigFileUtils import ConfigFileUtils

# calls 'callback'
def iterate(iterables_running, iterables_fixed, callback):
    if len(iterables_running) == 0:
        return
    
    if len(iterables_running) > 1:
        # do the iteration along the next dimension, and keep the others for later in the recursion
        explicit_iterable = iterables_running.pop()
        iterables_fixed.append(explicit_iterable)
        
        for pos in explicit_iterable:
            iterate(copy.deepcopy(iterables_running), copy.deepcopy(iterables_fixed), callback)
    else:
        # this is the last dimension along which need to iterate
        explicit_iterable = copy.deepcopy(iterables_running[0])
        iterables_fixed.append(explicit_iterable)
        for pos in explicit_iterable:
            callback(iterables_fixed)
            
class SweepDimensionDict:
    
    def __init__(self, name, parameter, start_dict, end_dict, step_dict):
        self.name = name
        self.parameter = parameter
        self.cur_dict = start_dict
        self.end_dict = end_dict
        self.step_dict = step_dict
        self.first_call = True
        
    def __iter__(self):
        return self
    
    def next(self):
        if not self.first_call:
            for key, step in self.step_dict.iteritems():
                if self.cur_dict[key] + step < self.end_dict[key]:
                    self.cur_dict[key] += step
                else:
                    raise StopIteration
        self.first_call = False
        return self.cur_dict
    
    def cur(self):
        return self.cur_dict
    
    def to_string(self):
        retval = ""
        for key, val in self.cur_dict.iteritems():
            retval += str(key) + "_" + str(val) + "_"
            
        return retval[:-1]
    
    def set_auxiliary(self, aux):
        self.aux = aux
        
    def get_auxiliary(self):
        return aux
    
    
class SweepDimensionList:
    
    def __init__(self, name, parameter, start_list, end_list):
        self.name = name
        self.parameter = parameter
        self.cur_list = start_list
        self.to_add_list = [entry for entry in end_list if entry not in start_list]
        self.first_call = True
        
    def __iter__(self):
        return self
    
    def next(self):
        if not self.first_call:
            if len(self.to_add_list) > 0:
                self.cur_list.append(self.to_add_list.pop())
            else:
                raise StopIteration
        self.first_call = False
        return self.cur_list
    
    def cur(self):
        return self.cur_list
    
    def to_string(self):
        return self.parameter + "_" + str(len(self.cur_list))
        
    def set_auxiliary(self, aux):
        self.aux = aux
        
    def get_auxiliary(self):
        return aux
    
def augment_config(mcoll, parent_dir, iterables):
    mconfhandler = ModelCollectionConfigFileHandler()
    mconfhandler.ToConfiguration(mcoll)
    
    outname = ""
    
    # augment the config object, given the values in the iterable dict
    for it in iterables:
        value = it.cur()
        behaviour = it.aux
        outname += it.to_string()
        section_name = it.name
        parameter_name = it.parameter
        
        if isinstance(value, dict):
            if behaviour == 'replace':
                mconfhandler.SetDict(section_name, parameter_name, value, lambda x: str(x))
            elif behaviour == 'append':
                mconfhandler.AddDictEntry(section_name, parameter_name, value, lambda x: float(x), lambda x: str(x))
        elif isinstance(value, list):
            if behaviour == 'replace':
                mconfhandler.SetList(section_name, parameter_name, value, lambda x: x)
            elif behaviour == 'append':
                mconfhandler.AddListEntry(section_name, parameter_name, value, lambda x: x, lambda x: x)

    outpath = parent_dir + outname + "/"
    
    if not os.path.exists(outpath):
        print "creating " + outpath
        os.makedirs(outpath)
        
    mconfhandler.save_configuration(outpath + "settings.conf")
    

def main():
    
    if len(sys.argv) != 2:
        print "Error: exactly 1 argument is required"

    campaign_dir = sys.argv[1]

    # make sure that the given directory ends with a /
    if not campaign_dir.endswith('/'):
        campaign_dir += "/"
        
    confhandler = ConfigFileHandler()
    confhandler.load_configuration(campaign_dir + "campaign.conf")
    
    iterables = []
    
    for section in confhandler.get_sections():
        if section != 'global':
            # this is a section that determines a new sweep direction
            sweep_metadata = section.split(':')
            sweep_name = sweep_metadata[0]
            sweep_parameter = sweep_metadata[1]

            # request more information
            sweep_behaviour = confhandler.get_field(section, 'behaviour')

            if ConfigFileUtils.is_dict(confhandler.get_field(section, 'start')):
                # will need a dictionary iterable
                start_dict = ConfigFileUtils.parse_dict(confhandler.get_field(section, 'start'), lambda x: float(x))
                end_dict = ConfigFileUtils.parse_dict(confhandler.get_field(section, 'end'), lambda x: float(x))
                step_dict = ConfigFileUtils.parse_dict(confhandler.get_field(section, 'step'), lambda x: float(x))
                it = SweepDimensionDict(sweep_name, sweep_parameter, start_dict, end_dict, step_dict)
            else:
                # construct a list iterable instead
                start_list = ConfigFileUtils.parse_list(confhandler.get_field(section, 'start'), lambda x: x)    
                end_list = ConfigFileUtils.parse_list(confhandler.get_field(section, 'end'), lambda x: x)
                it = SweepDimensionList(sweep_name, sweep_parameter, start_list, end_list)

            it.set_auxiliary(sweep_behaviour)

            iterables.append(it)
        
    MC_path = "/data_CMS/cms/wind/CJLST_NTuples/"
    model_type = confhandler.get_field('global', 'model_type') 

    if model_type == 'SimpleModel':
        mcoll = ModelFactory.GenerateSimpleModelCollectionsReducedCategorySet(MC_path)
    elif model_type == 'CombinedModel':
        mcoll = ModelFactory.GenerateCombinedModelCollectionsReducedCategorySet(MC_path)
        
    iterate(iterables, [], lambda it: augment_config(mcoll, campaign_dir, it))

if __name__ == "__main__":
    main()
