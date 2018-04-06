from trainlib.ModelCollection import ModelCollection
from trainlib.ModelCollectionConfigFileHandler import ModelCollectionConfigFileHandler
from trainlib.ModelFactoryFullCategorySet import ModelFactoryFullCategorySet
import os
import sys

def main():
    
    if len(sys.argv) != 2:
        print "Error: exactly 1 argument is required"

    # the path of the current run
    run_path = sys.argv[1]

    # load the configuration that is sitting there
    confhandler = ModelCollectionConfigFileHandler()
    confhandler.load_configuration(run_path + "settings.conf")

    # these are all the model collections that need to be trained
    mcolls = confhandler.GetModelCollection()

    # create the folder holding the settings for the individual models and their training
    settings_dir = run_path + "settings_training/"
    if not os.path.exists(settings_dir):
        os.makedirs(settings_dir)

    # iterate over these models and make a separate config file for each of them
    for mcoll in mcolls:
        training_settings_dir = settings_dir + mcoll.name + "/"
    
        if not os.path.exists(training_settings_dir):
            os.makedirs(training_settings_dir)
    
        outconf = ModelCollectionConfigFileHandler()
        outconf.ToConfiguration([mcoll])
        outconf.save_configuration(training_settings_dir + "settings.conf")

if __name__ == "__main__":
    main()
