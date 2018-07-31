import trainlib.cuts
import itertools
import os

from ModelCollection import ModelCollection
from ConfigFileHandler import ConfigFileHandler
from Category import Category
from DiscriminantEndpiece import DiscriminantEndpiece
from DiscriminantEndpieceComponent import DiscriminantEndpieceComponent
from FlexiblePCAWhiteningPreprocessor import FlexiblePCAWhiteningPreprocessor
from PCAWhiteningPreprocessor import PCAWhiteningPreprocessor
from SimpleModel import SimpleModel
from config import TrainingConfig
from ConfigFileUtils import ConfigFileUtils

class SimpleModelFactoryDynamic:

    @staticmethod
    def GenerateSimpleModelCollections(MC_path, weight_path = None, input_config_file = None, hyperparam_config_file = None, mass_point = 125.0):
        confhandler = ConfigFileHandler()

        zzroot = os.environ["CMSSW_BASE"]        
        config_file_path = os.path.join(zzroot, "src/ZZAnalysis/AnalysisStep/test/Python/trainlib/categories.conf")
        confhandler.load_configuration(config_file_path)
        default_section = confhandler.get_section("default")
        
        categories = []
        defined_category_names = ConfigFileUtils.parse_list(confhandler.get_field("categories", "category_names"), lambda x: x)

        print "found definitions for the following event categories: " + str(defined_category_names)
        
        for current_category_name in defined_category_names:
            print "------------------------------------------------"
            print "now building category " + current_category_name

            cur_sect = confhandler.get_section(current_category_name)
            current_category = Category.from_config(cur_sect, current_category_name, MC_path)

            # now create the discriminant endpieces defined for this category
            defined_endpieces = ConfigFileUtils.parse_list(cur_sect["endpieces"], lambda x: x)
            
            for endpiece_id in defined_endpieces:
                cur_sect = confhandler.get_section(endpiece_id)
                current_endpiece = DiscriminantEndpiece.from_config(cur_sect)
                
                defined_endpiece_components = ConfigFileUtils.parse_list(cur_sect["components"], lambda x: x)
                for endpiece_component in defined_endpiece_components:
                    cur_sect = confhandler.get_section(endpiece_component)
                    current_endpiece_component = DiscriminantEndpieceComponent.from_config(cur_sect, variable_section = default_section, model_basetype = SimpleModel, preprocessor_basetype = FlexiblePCAWhiteningPreprocessor)
                    
                    current_endpiece.add_component(current_endpiece_component)

                current_category.add_endpiece(current_endpiece)

            categories.append(current_category)
    
        # have all category objects defined, now generate all the needed pairs
        mcolls = []
        
        for pair in itertools.combinations(categories, 2):
            mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(*pair), input_config_file = input_config_file, hyperparam_config_file = hyperparam_config_file))

        return mcolls
