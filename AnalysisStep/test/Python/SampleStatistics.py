import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt

import os
import sys
from trainlib.ConfigFileHandler import ConfigFileHandler
from trainlib.ModelCollectionConfigFileHandler import ModelCollectionConfigFileHandler
import numpy as np
import pandas as pd

def main():

    if len(sys.argv) != 2:
        print "Error: exactly 1 argument is required"

    campaign_dir = sys.argv[1]

    bin_dir = {"Untagged": 0,
            "VBF1j": 1,
            "VBF2j": 2,
            "VHhadr": 3}


    df = pd.DataFrame()

    for subdir in next(os.walk(campaign_dir))[1]:
        if "statistics" not in subdir:
            punzi_path = campaign_dir + subdir + "/comp/Mor18_punzi_comp.conf"
            settings_path = campaign_dir + subdir + "/settings.conf"
    
            # first, read back the configuration file for this run
            conf = ModelCollectionConfigFileHandler()
            conf.LoadConfiguration(settings_path)
    
            # now select a typical model and read its hyperparameters
            typical_model = conf._get_model_list(conf._get_model_collection_list()[0])[0]
            hyperparams = conf.GetHyperparameters(typical_model)
            hyperparam_dict = {key: [val] for key, val in hyperparams.iteritems()}
            
            # then read in the results in terms of relative Punzi improvement for each category
            conf = ConfigFileHandler()
            conf.LoadConfiguration(punzi_path)
    
            # load the Punzi values for each category
            values = {}
            for category, bin_number in bin_dir.iteritems():
                values[category] = float(conf._get_field("Punzi", category))
        
            # merge the two dictionaries
            values.update(hyperparam_dict)
            df = df.append(pd.DataFrame.from_dict(values))

    # all different values of the number of neurons that were used in the sweep
    number_neurons = set(df['number_neurons'])

    statistics_dir = campaign_dir + "statistics/"
    if not os.path.exists(statistics_dir):
        os.makedirs(statistics_dir)

    # sort the pandas dataframe ascending by number of hidden layers
    df = df.sort_values("number_layers")

    for num in number_neurons:
        number_layers = df.loc[df["number_neurons"] == num, ["number_layers"]].as_matrix().flatten().astype(int)
        punzi_data = df.loc[df["number_neurons"] == num, bin_dir.keys()].as_matrix()
    
        plt.figure()
        plt.imshow(punzi_data, interpolation = 'none', cmap = 'RdYlGn', aspect = 0.6, vmin = 0.8, vmax = 1.2)
        plt.xticks(range(len(bin_dir)), bin_dir.keys())
        plt.yticks(range(len(number_layers)), number_layers)
        plt.ylabel("number hidden layers")
        plt.colorbar()
        plt.title("Punzi purity ratio [" + str(int(num)) + " hidden neurons]")
        plt.savefig(statistics_dir + "punzi_" + str(int(num)) + "_hidden_neurons.pdf")

if __name__ == "__main__":
    main()
