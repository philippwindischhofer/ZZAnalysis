from bdttools.bdtscores import BDTscorer
from trainlib.ConfigFileHandler import ConfigFileHandler
from trainlib.ConfigFileUtils import ConfigFileUtils
import pandas as pd
import os
import sys

def main():

    def append_variables(confhandler, impdict, threshold_fscore):
        section_name = impdict["discriminant"]
        confhandler.new_section(section_name)

        periodic_inputs = []
        nonperiodic_inputs = []
        for key, val in impdict.iteritems():
            if val[0] > threshold_fscore and key is not "discriminant":
                if "phi" in key or "Phi" in key or "xi" in key or "xistar" in key:
                    periodic_inputs.append(key)
                else:
                    nonperiodic_inputs.append(key)

        confhandler.set_field(section_name, "nonperiodic_columns", ConfigFileUtils.serialize_list(nonperiodic_inputs, lambda x: x))
        confhandler.set_field(section_name, "periodic_columns", ConfigFileUtils.serialize_list(periodic_inputs, lambda x: x))

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

    def select_input_features(category_pair, discriminant_name, scalar_branches, list_branches, list_pt_limits, confhandler, df_scores, threshold_fscore = 0.05):
        # temporary fix: for any discriminant that involves Z+X, do not use PF-MET (for the data / MC comparison would otherwise be potentially biased)
        if "ZX" in category_pair[0] or "ZX" in category_pair[1]:
            print "blocking PFMET for category pair " + category_pair[0] + " / " + category_pair[1]
            scalar_branches.remove("PFMET")

        print "will select input features from: " + str(scalar_branches + list_branches)

        implist = scorer.get_sorted_feature_importance_list(category_pair, scalar_branches, list_branches, list_pt_limits)

        print implist

        impdict = {convert_varname(entry[0]): [entry[1]] for entry in implist}
        impdict["discriminant"] = discriminant_name

        print "impdict: " + str(impdict)

        append_variables(confhandler, impdict, threshold_fscore)

        df = df_scores.append(pd.DataFrame.from_dict(impdict))

        print str(implist)
        return df

    def select_input_features_cumulative(category_pair, discriminant_name, scalar_branches, list_branches, list_pt_limits, confhandler, df_scores, cumulative_threshold = 0.99):
        print "using threshold = " + str(cumulative_threshold)

        # temporary fix: for any discriminant that involves Z+X, do not use PF-MET (for the data / MC comparison would otherwise be potentially biased)
        if "ZX" in category_pair[0] or "ZX" in category_pair[1]:
            print "blocking PFMET for category pair " + category_pair[0] + " / " + category_pair[1]
            scalar_branches.remove("PFMET")

        print "will select input features from: " + str(scalar_branches + list_branches)

        implist = scorer.get_sorted_feature_importance_list(category_pair, scalar_branches, list_branches, list_pt_limits)

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


    #out_dir = "/data_CMS/cms/wind/InputConfigurations2.0_cumulative/"
    out_dir = "/data_CMS/cms/wind/InputConfigurations2.0_leading_jets/"
    MC_path = "/data_CMS/cms/wind/CJLST_NTuples_prepared_systematics/"

    list_branches = ["Jet", "ExtraLep"]
    list_pt_limits = [30, 0]
    production_branches = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", "nExtraLep", 
                           "ZZMass_masked", "nExtraZ", "Z1Mass", "Z2Mass", "Z1Pt", "Z2Pt", "ZZMassErr", "ZZPt", "ZZEta", "ZZPhi", "Z1Flav", "Z2Flav"]
    decay_branches = ["costhetastar", "helphi", "helcosthetaZ1", "helcosthetaZ2", "phistarZ1", "xi", "xistar"]

    #campaign_name = raw_input("name of this configuration: ")
    #campaign_name = sys.argv[1]
    campaign_name = 'combined_99_bkg'

    #usemela = raw_input("use MELA (y/n)? ")
    #usemela = sys.argv[2]
    usemela = 'y'

    #threshold = float(sys.argv[3])
    threshold = 1.00
    print "using cumulative_threshold = " + str(threshold)

    if "y" in usemela:
        MELA_branches = ["D_bkg_ME", "D_VBF2j_ggH_ME", "D_VBF1j_ggH_ME", "D_WHh_ggH_ME", "D_ZHh_ggH_ME", "D_WHh_ZHh_ME", "D_VBF2j_WHh_ME", "D_VBF2j_ZHh_ME"]
    else:
        MELA_branches = []

    scorer = BDTscorer(MC_path)

    confhandler = ConfigFileHandler()
    df_fscores = pd.DataFrame()

    # the pairings with the ZX background category
    df_fscores = select_input_features(("ggH", "ZX"), "D_ggH_ZX_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    df_fscores = select_input_features(("VBF", "ZX"), "D_VBF_ZX_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    df_fscores = select_input_features(("ZHl", "ZX"), "D_ZHl_ZX_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    df_fscores = select_input_features(("ZHh", "ZX"), "D_ZHh_ZX_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    df_fscores = select_input_features(("WHl", "ZX"), "D_WHl_ZX_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    df_fscores = select_input_features(("WHh", "ZX"), "D_WHh_ZX_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    df_fscores = select_input_features(("ZHMET", "ZX"), "D_ZHMET_ZX_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    df_fscores = select_input_features(("ttHh", "ZX"), "D_ttHh_ZX_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    df_fscores = select_input_features(("ttHl", "ZX"), "D_ttHl_ZX_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)

    # the pairings with the qq background category
    df_fscores = select_input_features(("ggH", "qq"), "D_ggH_qq_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    df_fscores = select_input_features(("ggH", "qq"), "D_ggH_qq_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    df_fscores = select_input_features(("VBF", "qq"), "D_VBF_qq_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    df_fscores = select_input_features(("ZHl", "qq"), "D_ZHl_qq_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    df_fscores = select_input_features(("ZHh", "qq"), "D_ZHh_qq_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    df_fscores = select_input_features(("WHl", "qq"), "D_WHl_qq_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    df_fscores = select_input_features(("WHh", "qq"), "D_WHh_qq_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    df_fscores = select_input_features(("ZHMET", "qq"), "D_ZHMET_qq_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    df_fscores = select_input_features(("ttHh", "qq"), "D_ttHh_qq_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    df_fscores = select_input_features(("ttHl", "qq"), "D_ttHl_qq_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    df_fscores = select_input_features(("ZX", "qq"), "D_ZX_qq_ML", production_branches + decay_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)

    # the pairings of the signal sources among themselves
    # df_fscores = select_input_features_cumulative(("VBF2j", "ggH2j"), "D_VBF_ggH_2j_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("VBF1j", "ggH1j"), "D_VBF_ggH_1j_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("VBF0j", "ggH0j"), "D_VBF_ggH_0j_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("WHh", "ggH"), "D_WHh_ggH_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ZHh", "ggH"), "D_ZHh_ggH_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("WHh", "ZHh"), "D_WHh_ZHh_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("VBF", "WHh"), "D_VBF_WHh_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("VBF", "ZHh"), "D_VBF_ZHh_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("WHl", "ggH"), "D_WHl_ggH_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("WHl", "VBF"), "D_WHl_VBF_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("WHl", "WHh"), "D_WHl_WHh_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("WHl", "ZHh"), "D_WHl_ZHh_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("WHl", "ZHl"), "D_WHl_ZHl_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("WHl", "ZHMET"), "D_WHl_ZHMET_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("WHl", "ttHh"), "D_WHl_ttHh_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("WHl", "ttHl"), "D_WHl_ttHl_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ZHh", "ZHl"), "D_ZHh_ZHl_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ZHh", "ZHMET"), "D_ZHh_ZHMET_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ZHh", "ttHh"), "D_ZHh_ttHh_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ZHh", "ttHl"), "D_ZHh_ttHl_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ZHl", "ggH"), "D_ZHl_ggH_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ZHl", "VBF"), "D_ZHl_VBF_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ZHl", "WHh"), "D_ZHl_WHh_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ZHl", "ZHMET"), "D_ZHl_ZHMET_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ZHl", "ttHh"), "D_ZHl_ttHh_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ZHl", "ttHl"), "D_ZHl_ttHl_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ZHMET", "ggH"), "D_ZHMET_ggH_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ZHMET", "VBF"), "D_ZHMET_VBF_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ZHMET", "WHh"), "D_ZHMET_WHh_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ZHMET", "ttHh"), "D_ZHMET_ttHh_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ZHMET", "ttHl"), "D_ZHMET_ttHl_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ttHh", "ggH"), "D_ttHh_ggH_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ttHh", "VBF"), "D_ttHh_VBF_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ttHh", "WHh"), "D_ttHh_WHh_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ttHh", "ttHl"), "D_ttHh_ttHl_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ttHl", "ggH"), "D_ttHl_ggH_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ttHl", "VBF"), "D_ttHl_VBF_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)
    # df_fscores = select_input_features_cumulative(("ttHl", "WHh"), "D_ttHl_WHh_ML", production_branches + MELA_branches, list_branches, list_pt_limits, confhandler, df_fscores, threshold)

    # confhandler.save_configuration(os.path.join(out_dir, campaign_name + "_inputs.conf"))
    df_fscores.to_csv(os.path.join(out_dir, campaign_name + "_fscore_table_bkg.csv"))

if __name__ == "__main__":
    main()
