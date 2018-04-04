import configparser
import numpy as np
import os
import re
import copy
import sys

from trainlib.config import Config
from trainlib.SimpleModel import SimpleModel

# fix the naming!
from trainlib.ModelFactory import ModelFactory
from trainlib.ModelFactoryFullCategorySet import ModelFactoryFullCategorySet

from trainlib.ConfigFileHandler import ConfigFileHandler
from trainlib.ModelCollectionConfigFileHandler import ModelCollectionConfigFileHandler
from trainlib.ConfigFileUtils import ConfigFileUtils

# calls 'callback'
def iterate(iterables_running, iterables_fixed, callback):
    if len(iterables_running) == 0:
        return
    
    if len(iterables_running) > 1:
        # do the iteration along the next dimension, and keep the others for later in the recursion
        explicit_key = iterables_running.keys()[0]
        explicit_iterable = iterables_running.pop(explicit_key)
        iterables_fixed[explicit_key] = explicit_iterable
        
        for pos in explicit_iterable:
            iterate(copy.deepcopy(iterables_running), copy.deepcopy(iterables_fixed), callback)
    else:
        # this is the last dimension along which need to iterate
        explicit_iterable = copy.deepcopy(iterables_running)
        iterables_fixed.update(explicit_iterable)
        for pos in explicit_iterable.values()[0]:
            callback(iterables_fixed)
            
class SweepDimensionDict:
    
    def __init__(self, name, parameter, start_dict, end_dict, step_dict, aux):
        self.names = [name]
        self.parameters = [parameter]
        self.cur_dicts = [start_dict]
        self.end_dicts = [end_dict]
        self.step_dicts = [step_dict]
        self.auxs = [aux]
        self.first_call = True
        
    def add(self, name, parameter, start_dict, end_dict, step_dict, aux):
        self.names.append(name)
        self.parameters.append(parameter)
        self.cur_dicts.append(start_dict)
        self.end_dicts.append(end_dict)
        self.step_dicts.append(step_dict)
        self.auxs.append(aux)
        
    def __iter__(self):
        return self
    
    def next(self):
        if not self.first_call:
            for cur_dict, step_dict, end_dict in zip(self.cur_dicts, self.step_dicts, self.end_dicts):
                for key, step in step_dict.iteritems():
                    if cur_dict[key] + step < end_dict[key]:
                        cur_dict[key] += step
                    else:
                        raise StopIteration
        self.first_call = False
        return self.cur_dicts
    
    def cur(self):
        return self.cur_dicts
    
    def to_strings(self):
        retvals = []
        for cur_dict in self.cur_dicts:
            retval = ""
            for key, val in cur_dict.iteritems():
                retval += str(key) + "_" + str(val) + "_"
            
            retvals.append(retval[:-1])
        return retvals
    
class SweepDimensionList:
    
    def __init__(self, name, parameter, start_list, end_list, aux):
        self.names = [name]
        self.parameters = [parameter]
        self.cur_lists = [start_list]
        self.auxs = [aux]
        self.to_add_lists = [[entry for entry in end_list if entry not in start_list]]
        self.first_call = True
        
    def add(self, name, parameter, start_list, end_list, aux):
        self.names.append(name)
        self.parameters.append(parameter)
        self.cur_lists.append(start_list)
        self.to_add_lists.append([entry for entry in end_list if entry not in start_list])
        self.auxs.append(aux)
        
    def __iter__(self):
        return self
    
    def next(self):
        if not self.first_call:
            for to_add_list, cur_list in zip(self.to_add_lists, self.cur_lists):
                if len(to_add_list) > 0:
                    cur_list.append(to_add_list.pop())
                else:
                    raise StopIteration
        self.first_call = False
        return self.cur_lists
    
    def cur(self):
        return self.cur_lists
    
    def to_strings(self):
        retvals = []
        
        for cur_list, parameter in zip(self.cur_lists, self.parameters):
            retvals.append(parameter + "_" + str(len(cur_list)))
            
        return retvals
    
def augment_config(mcoll, parent_dir, iterables):
    mconfhandler = ModelCollectionConfigFileHandler()
    mconfhandler.ToConfiguration(mcoll)
    
    outname = ""
    
    # augment the config object, given the values in the iterable dict
    for it in iterables.values():
        values = it.cur()
        behaviours = it.auxs
        outname += it.to_strings()[0] # always the first one in a linked sweep determines the actual name of it
        section_names = it.names
        parameter_names = it.parameters
        
        # apply all the changes set forth in a (linked) sweep
        for value, behaviour, section_name, parameter_name in zip(values, behaviours, section_names, parameter_names):
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
    
    iterables = {}
    
    for section in confhandler.get_sections():
        if '!' in section:
            sweep_name = re.sub('!', '', section)
            sweep_sections = ConfigFileUtils.parse_list(confhandler.get_field(section, 'variables'), lambda x: x)

            # now look for the sweep variables that belong to this sweep
            for sweep_section in sweep_sections:
                # this is a section that determines a new sweep direction, possibly linked
                sweep_metadata = confhandler.get_field(sweep_section, 'variable').split(':')
                sweep_scope = sweep_metadata[0]
                sweep_parameter = sweep_metadata[1]

                # request more information
                sweep_behaviour = confhandler.get_field(sweep_section, 'behaviour')

                if ConfigFileUtils.is_dict(confhandler.get_field(sweep_section, 'start')):
                    # will need a dictionary iterable
                    start_dict = ConfigFileUtils.parse_dict(confhandler.get_field(sweep_section, 'start'), lambda x: float(x))
                    end_dict = ConfigFileUtils.parse_dict(confhandler.get_field(sweep_section, 'end'), lambda x: float(x))
                    step_dict = ConfigFileUtils.parse_dict(confhandler.get_field(sweep_section, 'step'), lambda x: float(x))

                    if sweep_name not in iterables:
                        it = SweepDimensionDict(sweep_scope, sweep_parameter, start_dict, end_dict, step_dict, sweep_behaviour)
                        iterables[sweep_name] = it
                    else:
                        iterables[sweep_name].add(sweep_scope, sweep_parameter, start_dict, end_dict, step_dict, sweep_behaviour)
                else:
                    # construct a list iterable instead
                    start_list = ConfigFileUtils.parse_list(confhandler.get_field(sweep_section, 'start'), lambda x: x)    
                    end_list = ConfigFileUtils.parse_list(confhandler.get_field(sweep_section, 'end'), lambda x: x)

                    if sweep_name not in iterables:
                        it = SweepDimensionList(sweep_scope, sweep_parameter, start_list, end_list, sweep_behaviour)
                        iterables[sweep_name] = it
                    else:
                        iterables[sweep_name].add(sweep_scope, sweep_parameter, start_list, end_list, sweep_behaviour)

    #MC_path = "/data_CMS/cms/wind/CJLST_NTuples/"
    MC_path = "/data_CMS/cms/wind/CJLST_NTuples_training_weights/"
    model_type = confhandler.get_field('global', 'model_type') 

    if model_type == 'SimpleModel':
        #mcoll = ModelFactory.GenerateSimpleModelCollectionsReducedCategorySet(MC_path)
        mcoll = ModelFactoryFullCategorySet.GenerateSimpleModelCollections(MC_path)
    elif model_type == 'CombinedModel':
        mcoll = ModelFactory.GenerateCombinedModelCollectionsReducedCategorySet(MC_path)
        
    iterate(iterables, {}, lambda it: augment_config(mcoll, campaign_dir, it))

if __name__ == "__main__":
    main()
