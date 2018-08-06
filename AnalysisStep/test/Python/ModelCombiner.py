import os, sys
import shutil
import numpy as np
import subprocess as sp

from trainlib.ConfigFileHandler import ConfigFileHandler
from trainlib.ConfigFileUtils import ConfigFileUtils
from trainlib.ModelCollectionConfigFileHandler import ModelCollectionConfigFileHandler
from DistributeTrainingSettings import distribute_training_settings

# this looks through several training runs (e.g. as they originate from a hyperparameter sweep) and takes for each 
# model the best one that was encountered

def get_loss(run, mcoll, model):
    confhandler = ConfigFileHandler()
    confhandler.load_configuration(os.path.join(run, "training", mcoll, "model_benchmark.txt"))
    return float(confhandler.get_field(model, 'val_loss'))


def main():
    # runs to check for (good) models (the first one passed is taken as reference run from which the available models
    # are taken - it is expected that all others runs also follow this structure):
    input_runs = []

    print "==================================================================="
    print "looking for models in the following runs:"
    
    for campaign_dir in sys.argv[1:-2]:
        for run_dir in next(os.walk(campaign_dir))[1]:
            if not "bin" in run_dir:
                run_path = os.path.join(campaign_dir, run_dir)
                print run_path
                input_runs.append(run_path)

    print "==================================================================="

    # output training campaign, this will consist of a combination of the models found in the campaigns listed above, in such a way that the overall performance is optimized
    output_run = os.path.join(sys.argv[-1], "optimized")

    # where the configuration file for the hyperparameter settings should be stored
    hyperparam_output = os.path.join(output_run, "../hyperparameters.conf")

    os.makedirs(output_run)

    # load the available model names
    reference_run = input_runs[0]
    available_mcolls = os.walk(os.path.join(reference_run, "training")).next()[1]

    mcolls_winning = []

    for mcoll in available_mcolls:
        models = os.walk(os.path.join(reference_run, "training", mcoll)).next()[1]

        # load a representative version of the current model collection...
        mconfhandler = ModelCollectionConfigFileHandler()
        mconfhandler.load_configuration(os.path.join(reference_run, "settings_training", mcoll, "settings.conf"))
        mcoll_template = mconfhandler.GetModelCollection()[0]
    
        # ... but strip away all the actual model components
        mcoll_template.model_dict = {}
        mcoll_template.preprocessor_dict = {}
        mcoll_template.settings_dict = {}
    
        for model in models:
            # compare this model across the different runs
            losses = [get_loss(run, mcoll, model) for run in input_runs]

            winner = np.argmin(losses)
        
            winning_run = input_runs[winner]
            
            # copy the winning model into the output run
            shutil.copytree(os.path.join(winning_run, "training", mcoll, model),
                            os.path.join(output_run, "training", mcoll, model))

            print "--------------------------------------------"
            print " take " + model + " from " + winning_run
            print "--------------------------------------------"
        
            # load the winning model to keep track of its settings
            mconfhandler = ModelCollectionConfigFileHandler()
            mconfhandler.load_configuration(os.path.join(winning_run, "settings_training", mcoll, "settings.conf"))
            mcoll_winning = mconfhandler.GetModelCollection()[0]
            
            # then pull the winning model over into the template
            winning_model = mcoll_winning.model_dict[model]
            winning_preprocessor = mcoll_winning.preprocessor_dict[model]
            winning_settings = mcoll_winning.settings_dict[model]
        
            mcoll_template.add_model(winning_preprocessor, winning_model, winning_settings)
        
        mcolls_winning.append(mcoll_template)
    
    # now save the put-together config file also into the output run
    mconfhandler = ModelCollectionConfigFileHandler()
    mconfhandler.ToConfiguration(mcolls_winning)
    mconfhandler.save_configuration(os.path.join(output_run, "settings.conf"))

    # now distriute again the training settings, as usual:
    distribute_training_settings(output_run + '/')

    # now create the hyperparameter config file for each model, taken from the winners
    hp_confhandler = ConfigFileHandler()
    for mcoll in mcolls_winning:
        for model_name, model in mcoll.model_dict.iteritems():
            hp_confhandler.new_section(model_name)
            hp_confhandler.set_field(model_name, "hyperparameters", ConfigFileUtils.serialize_dict(model.hyperparameters, lambda x: str(x)))
        
    hp_confhandler.save_configuration(hyperparam_output)

    print "==================================================================="
    print "hyperparameter configuration file written to " + hyperparam_output
    print "==================================================================="

if __name__ == "__main__":
    main()
