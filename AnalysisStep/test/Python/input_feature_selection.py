from bdttools.bdtscores import BDTscorer
from trainlib.ConfigFileHandler import ConfigFileHandler
from trainlib.ConfigFileUtils import ConfigFileUtils
from trainlib.SimpleModelFactoryDynamic import SimpleModelFactoryDynamic
import trainlib.cuts
import pandas as pd
import os
import sys

def main():

    def append_variables_raw(confhandler, impdict):
        section_name = impdict["discriminant"]
        confhandler.new_section(section_name)

        periodic_inputs = []
        nonperiodic_inputs = []
        for key, val in impdict.iteritems():
            if key is not "discriminant":
                if "phi" in key or "Phi" in key or "xi" in key or "xistar" in key:
                    periodic_inputs.append(key)
                else:
                    nonperiodic_inputs.append(key)

        confhandler.set_field(section_name, "nonperiodic_columns", ConfigFileUtils.serialize_list(nonperiodic_inputs, lambda x: x))
        confhandler.set_field(section_name, "periodic_columns", ConfigFileUtils.serialize_list(periodic_inputs, lambda x: x))

    def convert_varname(raw):
        raw = raw.replace('(', '[')
        raw = raw.replace(')', ']')
        return raw

    def select_input_features_cumulative(H1_stream, H0_stream, discriminant_name, scalar_branches, list_branches, list_pt_limits, confhandler, df_scores, cumulative_threshold = 0.99):
        print "using cumulative threshold = " + str(cumulative_threshold)

        # temporary fix: for any discriminant that involves Z+X, do not use PF-MET (for the data / MC comparison would otherwise be potentially biased)
        if "ZX" in model.name:
            print "blocking PFMET for model " + model.name
            scalar_branches.remove("PFMET")

        print "will select input features from: " + str(scalar_branches + list_branches)

        implist = scorer.get_sorted_feature_importance_list(H1_stream, H0_stream, scalar_branches, list_branches, list_pt_limits)

        print "implist: " + str(implist)

        # now iterate through the sorted list that has been returned and put only the highest-ranked variables, up to the threshold
        running_sum = 0
        impdict = {}
        for key, val in implist:
            if running_sum < cumulative_threshold:
                impdict[convert_varname(key)] = [val]
                running_sum += val

        impdict["discriminant"] = discriminant_name

        print "impdict: " + str(impdict)

        append_variables_raw(confhandler, impdict)

        df = df_scores.append(pd.DataFrame.from_dict(impdict))

        print str(implist)
        return df

    if len(sys.argv) != 6:
        print "Error: exactly 5 arguments are required"
        return

    out_dir = sys.argv[1]
    campaign_name = sys.argv[2]
    MC_path = sys.argv[3]
    usemela = sys.argv[4]
    threshold = float(sys.argv[5])

    # input variables that are stored as lists
    list_branches = ["Jet", "ExtraLep"]

    # limit pt values for these lists: here, jets are only used if their pt > 30GeV, no restrictions are placed on leptons
    list_pt_limits = [30, 0]

    # scalar (i.e. non-list) input variables
    production_branches = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", "nExtraLep", 
                           "ZZMass_masked", "nExtraZ", "Z1Mass", "Z2Mass", "Z1Pt", "Z2Pt", "ZZMassErr", "ZZPt", "ZZEta", "ZZPhi", "Z1Flav", "Z2Flav"]
    decay_branches = ["costhetastar", "helphi", "helcosthetaZ1", "helcosthetaZ2", "phistarZ1", "xi", "xistar"]

    print "using cumulative_threshold = " + str(threshold)

    if "y" in usemela:
        MELA_branches = ["D_bkg_ME", "D_VBF2j_ggH_ME", "D_VBF1j_ggH_ME", "D_WHh_ggH_ME", "D_ZHh_ggH_ME", "D_WHh_ZHh_ME", "D_VBF2j_WHh_ME", "D_VBF2j_ZHh_ME"]
    else:
        MELA_branches = []

    scorer = BDTscorer(MC_path)
    confhandler = ConfigFileHandler()
    df_fscores = pd.DataFrame()

    # create a model collection with default input variables and default hyperparameters, just to get a list of all the models and their training data files
    mcolls = SimpleModelFactoryDynamic.GenerateSimpleModelCollections(MC_path, input_config_file = None, hyperparam_config_file = None)

    # iterate over all models that are contained in the list of model collections
    for mcoll in mcolls:
        for model_name in mcoll.model_dict.keys():
            
            model = mcoll.model_dict[model_name]
            pre = mcoll.preprocessor_dict[model_name]

            H1_stream = {}
            H0_stream = {}

            print "=========================================================================="
            print "selecting input variables for model " + model_name

            for key, val in mcoll.H1_stream.iteritems():
                H1_stream[key] = lambda row, val = val, pre = pre: val(row) and pre.cuts(row)
                print "adding preprocessor cuts on top for " + key + ": " + pre.cuts_s

            for key, val in mcoll.H0_stream.iteritems():
                H0_stream[key] = lambda row, val = val, pre = pre: val(row) and pre.cuts(row)
                print "adding preprocessor cuts on top for " + key + ": " + pre.cuts_s
            
            df_fscores = select_input_features_cumulative(H1_stream, H0_stream, model_name, production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
            print "=========================================================================="

    confhandler.save_configuration(os.path.join(out_dir, campaign_name + "_inputs.conf"))
    df_fscores.to_csv(os.path.join(out_dir, campaign_name + "_fscore_table_bkg.csv"))

if __name__ == "__main__":
    main()
