import configparser
import numpy as np
import os
import copy
import sys

from trainlib.config import Config
from trainlib.SimpleModel import SimpleModel
from trainlib.ModelFactory import ModelFactory
from trainlib.ConfigFileHandler import ConfigFileHandler
from trainlib.ModelCollectionConfigFileHandler import ModelCollectionConfigFileHandler

def dict_to_dirname(in_dict):
    dirname = ""
    for key, val in in_dict.iteritems():
        dirname += str(key) + "_" + str(val) + "_"
        
    return dirname[:-1]

def augment_config(mcoll, hyperparams, inputs_to_add, parent_dir):
    mconfhandler = ModelCollectionConfigFileHandler()
    mconfhandler.ToConfiguration(mcoll)
    
    # augment the original config
    mconfhandler.SetHyperparametersGlobally(hyperparams)
    
    added_params = 0
    # are there any input variables to add?
    for input_to_add in inputs_to_add:
        if input_to_add:
            added_params += 1
            mconfhandler.AddInputParameterGlobally(input_to_add)
        
    # convert to directory name
    dirname = dict_to_dirname(hyperparams) + "_extraparams_" + str(added_params)
     
    # make directory of that name
    confdir = parent_dir + dirname + "/"

    if not os.path.exists(confdir):
        print "creating " + confdir
        os.makedirs(confdir)
    
    # save the config file there
    mconfhandler.SaveConfiguration(confdir + "settings.conf")

# performs a dynamic number of nested iterations
def iterate(start, end, step, fixed, callback):
    if len(start) == 0:
        return
    
    if len(start) > 1:
        # truncate the dict and keep going on the smaller one
        new_keys = start.keys()[1:]
        new_start = {key: start[key] for key in new_keys}
        new_end = {key: end[key] for key in new_keys}
        new_step = {key: step[key] for key in new_keys}
        
        new_fixed_key = start.keys()[0]
        
        for x in np.arange(start[new_fixed_key], end[new_fixed_key], step[new_fixed_key]):
            new_fixed = fixed
            new_fixed[new_fixed_key] = x
        
            iterate(new_start, new_end, new_step, new_fixed, callback)
    else:
        # only have one dimension left along which to iterate
        dimension = start.keys()[0]
        
        for x in np.arange(start[dimension], end[dimension], step[dimension]):
            allvars = fixed
            allvars[dimension] = x
            callback(allvars)

def main():
    
    if len(sys.argv) != 2:
        print "Error: exactly 1 argument is required"

    campaign_dir = sys.argv[1]

    # make sure that the given directory ends with a /
    if not campaign_dir.endswith('/'):
        campaign_dir += "/"

    confhandler = ConfigFileHandler()
    confhandler.LoadConfiguration(campaign_dir + "campaign.conf")

    # build the standard configuration from which to start
    mcoll = ModelFactory.GenerateStandardModelCollections(SimpleModel, "/data_CMS/cms/wind/CJLST_NTuples/")

    # load the sweep parameters for the hyperparameters
    start_dict = confhandler._process_dict(confhandler._get_field('hyperparameters', 'start'), lambda x: float(x))
    end_dict = confhandler._process_dict(confhandler._get_field('hyperparameters', 'end'), lambda x: float(x))
    step_dict = confhandler._process_dict(confhandler._get_field('hyperparameters', 'step'), lambda x: float(x))

    # load the information about which input parameters to add
    start_params = confhandler._process_list(confhandler._get_field('input_columns', 'start'), lambda x: x)
    end_params = confhandler._process_list(confhandler._get_field('input_columns', 'end'), lambda x: x)

    params_to_add = [param for param in end_params if param not in start_params]

    params = start_params
    iterate(start_dict, end_dict, step_dict, {}, lambda hyperparams: augment_config(mcoll, hyperparams, params, campaign_dir))
    for param_to_add in params_to_add:
        params.append(param_to_add)
        iterate(start_dict, end_dict, step_dict, {}, lambda hyperparams: augment_config(mcoll, hyperparams, params, campaign_dir))

if __name__ == "__main__":
    main()
