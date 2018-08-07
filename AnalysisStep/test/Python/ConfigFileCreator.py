import configparser
import numpy as np
import os
import re
import copy
import sys

from trainlib.config import Config
from trainlib.SimpleModel import SimpleModel

# fix the naming!
from trainlib.SimpleModelFactoryDynamic import SimpleModelFactoryDynamic

from trainlib.ConfigFileHandler import ConfigFileHandler
from trainlib.ModelCollectionConfigFileHandler import ModelCollectionConfigFileHandler
from trainlib.ConfigFileUtils import ConfigFileUtils

def main():
    
    if len(sys.argv) != 5:
        print "Error: exactly 4 arguments is required"

    campaign_dir = sys.argv[1]
    input_config_file = sys.argv[2]
    hyperparam_config_file = sys.argv[3]
    workdir = sys.argv[4]

    mass_point = 125.0

    run_dir = os.path.join(campaign_dir, "run")

    if not os.path.exists(run_dir):
        os.makedirs(run_dir)
    
    MC_path = os.path.join(workdir, "trainval/")

    # this always uses SimpleModel by default (more complicated models are accessible through ConfigFileSweeper when performing a sweep of network hyperparameters etc.)
    mcoll = SimpleModelFactoryDynamic.GenerateSimpleModelCollections(MC_path, input_config_file = input_config_file, hyperparam_config_file = hyperparam_config_file, mass_point = mass_point)

    mconfhandler = ModelCollectionConfigFileHandler()
    mconfhandler.ToConfiguration(mcoll)

    mconfhandler.save_configuration(os.path.join(run_dir, "settings.conf"))

if __name__ == "__main__":
    main()
