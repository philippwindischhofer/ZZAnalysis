import configparser
import numpy as np
import os
import re
import copy
import sys

from trainlib.config import Config
from trainlib.SimpleModel import SimpleModel
from trainlib.CombinedModel import CombinedModel

# fix the naming!
from trainlib.ModelFactory import ModelFactory
from trainlib.ModelFactoryFullCategorySetOptimizedInputs import ModelFactoryFullCategorySetOptimizedInputs
from trainlib.ModelFactoryFullCategorySetOptimizedInputsInclusive import ModelFactoryFullCategorySetOptimizedInputsInclusive
from trainlib.ModelFactoryFullCategorySetOptimizedInputsCombined import ModelFactoryFullCategorySetOptimizedInputsCombined

from trainlib.ModelFactoryFullMassRangeDynamicInclusive import ModelFactoryFullMassRangeDynamicInclusive
from trainlib.ModelFactoryFullMassRangeDynamic import ModelFactoryFullMassRangeDynamic

from trainlib.ModelFactoryFullCategorySetDynamic import ModelFactoryFullCategorySetDynamic
from trainlib.ModelFactoryFullCategorySetDynamicInclusive import ModelFactoryFullCategorySetDynamicInclusive

from trainlib.ConfigFileHandler import ConfigFileHandler
from trainlib.ModelCollectionConfigFileHandler import ModelCollectionConfigFileHandler
from trainlib.ConfigFileUtils import ConfigFileUtils

def main():
    
    if len(sys.argv) != 4:
        print "Error: exactly 3 arguments is required"

    campaign_dir = sys.argv[1]
    input_config_file = sys.argv[2]
    hyperparam_config_file = sys.argv[3]
    workdir = sys.argv[4]

    mass_point = 125.0

    run_dir = os.path.join(campaign_dir, "run")

    if not os.path.exists(run_dir):
        os.makedirs(run_dir)
    
    #    MC_path = "/data_CMS/cms/wind/CJLST_NTuples_with_systematics/trainval/"
    MC_path = os.path.join(workdir, "trainval/")
    mcoll = ModelFactoryFullMassRangeDynamicInclusive.GenerateSimpleModelCollections(MC_path, input_config_file = input_config_file, hyperparam_config_file = hyperparam_config_file, mass_point = mass_point)

    mconfhandler = ModelCollectionConfigFileHandler()
    mconfhandler.ToConfiguration(mcoll)

    mconfhandler.save_configuration(os.path.join(run_dir, "settings.conf"))

if __name__ == "__main__":
    main()
