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

    def format_parameter_list(inlist):
        outstring = ""
        linewidth = 0
    
        for parameter in inlist:
            if "D_" in parameter:
                newstring = "MELA, "
            else:
                newstring = parameter + ", "
            
            outstring += newstring
            linewidth += len(newstring)
        
            if linewidth > 20:
                outstring += "\n"
                linewidth = 0
            
        return outstring[:-2]

    df = pd.DataFrame()

    for subdir in next(os.walk(campaign_dir))[1]:
        if "statistics" not in subdir:
            values = {}

            punzi_path = campaign_dir + subdir + "/comp/Mor18_punzi_comp.conf"
            settings_path = campaign_dir + subdir + "/settings.conf"
    
            # first, read back the configuration file for this run
            conf = ModelCollectionConfigFileHandler()
            conf.LoadConfiguration(settings_path)
    
            # now select a typical model and read its hyperparameters
            typical_model = conf._get_model_list(conf._get_model_collection_list()[0])[0]
            hyperparams = conf.GetHyperparameters(typical_model)
            hyperparam_dict = {key: [val] for key, val in hyperparams.iteritems()}
        
            # also read the list of input parameters that have been fed into the network
            param_list = conf.GetInputParameterList(typical_model)
            values['input_columns'] = [format_parameter_list(param_list)]
            values['number_inputs'] = len(param_list)
            
            # then read in the results in terms of relative Punzi improvement for each category
            conf = ConfigFileHandler()
            conf.LoadConfiguration(punzi_path)
    
            # load the Punzi values for each category
            for category, bin_number in bin_dir.iteritems():
                values[category] = float(conf._get_field("Punzi", category))
        
            # merge the two dictionaries
            values.update(hyperparam_dict)
            df = df.append(pd.DataFrame.from_dict(values))

    statistics_dir = campaign_dir + "statistics/"
    if not os.path.exists(statistics_dir):
        os.makedirs(statistics_dir)

    punzi_data = df[bin_dir.keys()].as_matrix()
    punzi_data = np.transpose(punzi_data)

    inparam_labels = df['input_columns'].as_matrix()

    plt.figure(figsize = (8, 9))
    plt.imshow(punzi_data, interpolation = 'none', cmap = 'RdYlGn', aspect = 0.6, vmin = 0.8, vmax = 1.2)
    plt.colorbar()
    plt.yticks(range(len(bin_dir)), bin_dir.keys())
    plt.xticks(range(len(df)), inparam_labels, rotation = 'vertical')
    plt.title("Punzi purity ratio")
    #plt.tight_layout()
    plt.savefig(statistics_dir + "punzi.pdf", bbox_inches = 'tight')

if __name__ == "__main__":
    main()
