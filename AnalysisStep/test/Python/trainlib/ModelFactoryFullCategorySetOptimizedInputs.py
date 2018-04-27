from ModelCollection import ModelCollection
from FlexiblePCAWhiteningPreprocessor import FlexiblePCAWhiteningPreprocessor
from PCAWhiteningPreprocessor import PCAWhiteningPreprocessor
from RNNPreprocessor import RNNPreprocessor
from CombinedPreprocessor import CombinedPreprocessor
from SimpleModel import SimpleModel
from CombinedModel import CombinedModel
from config import TrainingConfig
import cuts

class ModelFactoryFullCategorySetOptimizedInputs:

    # this will generate Model collections for the full set of 9 categories, so 36 different discriminants are needed (some with multiple models)
    @staticmethod
    def GenerateSimpleModelCollections(MC_path, weight_path = None):
        mcolls = []
        global_max_epochs = 100
        global_hyperparams = {'number_layers': 2}

        # ---------------------------------------------

        H1_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_VBF_ggH_ML"
        H1_name = "VBF"
        H0_name = "ggH"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_VBF_ggH_2j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetPt[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|0]", "JetEta[JetPt|1]", 
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "D_VBF2j_ggH_ME", "D_ZHh_ggH_ME", "D_WHh_ZHh_ME", "D_WHh_ggH_ME", "D_VBF2j_WHh_ME", "D_VBF2j_ZHh_ME",
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ggH_1j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", 
                               "D_VBF1j_ggH_ME",
                               "ZZPt", "PFMET", "ZZMassErr", "Z1Mass", "Z2Mass", "ZZMass", "Z1Pt", "Z2Pt", "ZZEta", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ggH_0j_ML"
        nonperiodic_columns = ["ZZPt", "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMass"]
        periodic_columns = []
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 0 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_WHh_ggH_ML"
        H1_name = "WHh"
        H0_name = "ggH"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHh_ggH_2j_ML"

        nonperiodic_columns = ["JetPt[JetPt|0]", "JetPt[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|0]", "JetEta[JetPt|1]", "JetEta[JetPt|2]",  
                               "D_VBF2j_ggH_ME", "D_ZHh_ggH_ME", "D_WHh_ZHh_ME", "D_WHh_ggH_ME", "D_VBF2j_ZHh_ME", "D_VBF2j_WHh_ME",
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMass", "ZZMassErr"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "JetPhi[JetPt|2]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_WHh_ggH_1j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]",
                               "D_VBF1j_ggH_ME",
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF",
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMass", "ZZMassErr"]
        periodic_columns = ["JetPhi[JetPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_WHh_ggH_0j_ML"
        nonperiodic_columns = ["ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMass", "ZZMassErr"]
        periodic_columns = []
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 0 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ZHh_ggH_ML"
        H1_name = "ZHh"
        H0_name = "ggH"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHh_ggH_2j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetPt[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|0]", "JetEta[JetPt|1]", "JetEta[JetPt|2]", 
                               "D_VBF2j_ggH_ME", "D_ZHh_ggH_ME", "D_WHh_ZHh_ME", "D_WHh_ggH_ME", "D_VBF2j_WHh_ME", "D_VBF2j_ZHh_ME",
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "JetPhi[JetPt|2]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_ZHh_ggH_1j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", 
                               "D_VBF1j_ggH_ME",
                               "ZZPt", "PFMET", "ZZMassErr", "Z1Mass", "Z2Mass", "ZZMass", "Z1Pt", "Z2Pt", "ZZEta",
                               "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_ZHh_ggH_0j_ML"
        nonperiodic_columns = ["ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "ZZPt", "PFMET", "ZZMassErr", "ZZMass", "Z1Mass", "Z2Mass", "Z1Pt", "Z2Pt", "ZZEta"]
        periodic_columns = ["ExtraLepPhi[ExtraLepPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 0 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}

        mcoll_name = "D_WHh_ZHh_ML"
        H1_name = "WHh"
        H0_name = "ZHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHh_ZHh_2j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetPt[JetPt|1]", "JetPt[JetPt|2]", "JetPt[JetPt|3]", "JetEta[JetPt|0]", "JetEta[JetPt|1]", "JetEta[JetPt|2]", "JetEta[JetPt|3]", 
                               "D_VBF2j_ggH_ME", "D_VBF2j_ZHh_ME", "D_VBF2j_WHh_ME", "D_ZHh_ggH_ME", "D_WHh_ZHh_ME", "D_WHh_ggH_ME",
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMass", "ZZMassErr"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "JetPhi[JetPt|2]", "JetPhi[JetPt|3]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_WHh_ZHh_1j_ML"
        nonperiodic_columns = ["ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]", "JetPt[JetPt|0]", "JetEta[JetPt|0]",
                               "PFMET", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZMass", "ZZPt", "ZZEta", "Z2Mass", "Z1Mass", "Z2Pt", "Z1Pt", "ZZMassErr"]
        periodic_columns = ["JetPhi[JetPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_WHh_ZHh_0j_ML"
        nonperiodic_columns = ["ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", 
                               "ZZMass", "ZZPt", "ZZEta", "Z2Mass", "Z1Mass", "Z2Pt", "Z1Pt", "ZZMassErr"]
        periodic_columns = []
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 0 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)


        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}
        H0_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}

        mcoll_name = "D_VBF_WHh_ML"
        H1_name = "VBF"
        H0_name = "WHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_VBF_WHh_2j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetPt[JetPt|1]", "JetPt[JetPt|2]", "JetPt[JetPt|3]", "JetEta[JetPt|0]", "JetEta[JetPt|1]", "JetEta[JetPt|2]", "JetEta[JetPt|3]", 
                               "D_VBF2j_ggH_ME", "D_ZHh_ggH_ME", "D_WHh_ZHh_ME", "D_WHh_ggH_ME", "D_VBF2j_ZHh_ME", "D_VBF2j_WHh_ME",
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_WHh_1j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", 
                               "D_VBF1j_ggH_ME",
                               "ZZPt", "PFMET", "ZZMassErr", "Z1Mass", "Z2Mass", "Z1Pt", "Z2Pt", "ZZEta", "ZZMass"]
        periodic_columns = ["JetPhi[JetPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_WHh_0j_ML"
        nonperiodic_columns = ["ExtraLepPt[ExtraLepPt|0]",
                               "ZZPt", "PFMET", "ZZMassErr", "Z1Mass", "Z2Mass", "Z1Pt", "Z2Pt", "ZZEta", "ZZMass"]
        periodic_columns = []
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 0 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}

        mcoll_name = "D_VBF_ZHh_ML"
        H1_name = "VBF"
        H0_name = "ZHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_VBF_ZHh_2j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetPt[JetPt|1]", "JetPt[JetPt|2]", "JetPt[JetPt|3]", "JetEta[JetPt|0]", "JetEta[JetPt|1]", "JetEta[JetPt|2]", "JetEta[JetPt|3]",
                               "D_VBF2j_ggH_ME", "D_ZHh_ggH_ME", "D_WHh_ggH_ME", "D_WHh_ZHh_ME", "D_VBF2j_ZHh_ME", "D_VBF2j_WHh_ME",
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "JetPhi[JetPt|2]", "JetPhi[JetPt|3]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ZHh_1j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]",
                               "D_VBF1j_ggH_ME",
                               "PFMET", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass"]
        periodic_columns = ["JetPhi[JetPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ZHh_0j_ML"
        nonperiodic_columns = ["ExtraLepPt[ExtraLepPt|0]",
                               "PFMET", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass"]
        periodic_columns = []
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 0 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------        

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHlept_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHlept_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_WHl_ggH_ML"
        H1_name = "WHl"
        H0_name = "ggH"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHl_ggH_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass", 
                               "nExtraLep"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ExtraLepPhi[ExtraLepPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------        

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHlept_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHlept_cut}
        H0_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_WHl_VBF_ML"
        H1_name = "WHl"
        H0_name = "VBF"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHl_VBF_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetEta[JetPt|2]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ExtraLepPhi[ExtraLepPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------        

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHlept_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHlept_cut}
        H0_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}

        mcoll_name = "D_WHl_WHh_ML"
        H1_name = "WHl"
        H0_name = "WHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHl_WHh_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ExtraLepPhi[ExtraLepPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------        

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHlept_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHlept_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}

        mcoll_name = "D_WHl_ZHh_ML"
        H1_name = "WHl"
        H0_name = "ZHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHl_ZHh_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ExtraLepPhi[ExtraLepPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------        

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHlept_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHlept_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}

        mcoll_name = "D_WHl_ZHl_ML"
        H1_name = "WHl"
        H0_name = "ZHl"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHl_ZHl_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ExtraLepPhi[ExtraLepPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------        

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHlept_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHlept_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}

        mcoll_name = "D_WHl_ZHMET_ML"
        H1_name = "WHl"
        H0_name = "ZHMET"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHl_ZHMET_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ExtraLepPhi[ExtraLepPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------        

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHlept_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHlept_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}

        mcoll_name = "D_WHl_ttHh_ML"
        H1_name = "WHl"
        H0_name = "ttHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHl_ttHh_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|2]", "JetPt[JetPt|3]", "JetEta[JetPt|3]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "JetPhi[JetPt|2]", "JetPhi[JetPt|3]", "ExtraLepPhi[ExtraLepPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------        

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHlept_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHlept_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}

        mcoll_name = "D_WHl_ttHl_ML"
        H1_name = "WHl"
        H0_name = "ttHl"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHl_ttHl_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|2]", "JetPt[JetPt|3]", "JetEta[JetPt|3]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ExtraLepPhi[ExtraLepPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}

        mcoll_name = "D_ZHh_ZHl_ML"
        H1_name = "ZHh"
        H0_name = "ZHl"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHh_ZHl_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ExtraLepPhi[ExtraLepPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}

        mcoll_name = "D_ZHh_ZHMET_ML"
        H1_name = "ZHh"
        H0_name = "ZHMET"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHh_ZHMET_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|2]", "ExtraLepPt[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}

        mcoll_name = "D_ZHh_ttHh_ML"
        H1_name = "ZHh"
        H0_name = "ttHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHh_ttHh_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|2]", "JetPt[JetPt|3]", "JetEta[JetPt|3]", "ExtraLepPt[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)
        
        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}

        mcoll_name = "D_ZHh_ttHl_ML"
        H1_name = "ZHh"
        H0_name = "ttHl"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHh_ttHl_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|2]", "JetPt[JetPt|3]", "JetEta[JetPt|3]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "JetPhi[JetPt|2]", "JetPhi[JetPt|3]", "ExtraLepPhi[ExtraLepPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ZHl_ggH_ML"
        H1_name = "ZHl"
        H0_name = "ggH"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHl_ggH_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}
        H0_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ZHl_VBF_ML"
        H1_name = "ZHl"
        H0_name = "VBF"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHl_VBF_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|2]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}
        H0_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}

        mcoll_name = "D_ZHl_WHh_ML"
        H1_name = "ZHl"
        H0_name = "WHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHl_WHh_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|2]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}

        mcoll_name = "D_ZHl_ZHMET_ML"
        H1_name = "ZHl"
        H0_name = "ZHMET"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHl_ZHMET_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}

        mcoll_name = "D_ZHl_ttHh_ML"
        H1_name = "ZHl"
        H0_name = "ttHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHl_ttHh_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|2]", "JetPt[JetPt|3]", "JetEta[JetPt|3]", "ExtraLepPt[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}

        mcoll_name = "D_ZHl_ttHl_ML"
        H1_name = "ZHl"
        H0_name = "ttHl"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHl_ttHl_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|2]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepPt[ExtraLepPt|1]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "JetPhi[JetPt|2]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ZHMET_ggH_ML"
        H1_name = "ZHMET"
        H0_name = "ggH"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHMET_ggH_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}
        H0_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ZHMET_VBF_ML"
        H1_name = "ZHMET"
        H0_name = "VBF"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHMET_VBF_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}
        H0_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}

        mcoll_name = "D_ZHMET_WHh_ML"
        H1_name = "ZHMET"
        H0_name = "WHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHMET_WHh_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}

        mcoll_name = "D_ZHMET_ttHh_ML"
        H1_name = "ZHMET"
        H0_name = "ttHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHMET_ttHh_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|2]", "JetPt[JetPt|3]", "JetEta[JetPt|3]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}

        mcoll_name = "D_ZHMET_ttHl_ML"
        H1_name = "ZHMET"
        H0_name = "ttHl"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHMET_ttHl_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|2]", "JetPt[JetPt|3]", "JetEta[JetPt|3]", "ExtraLepPt[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "JetPhi[JetPt|2]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ttHh_ggH_ML"
        H1_name = "ttHh"
        H0_name = "ggH"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ttHh_ggH_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|2]", "JetPt[JetPt|3]", "JetEta[JetPt|3]", "ExtraLepPt[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "JetPhi[JetPt|2]", "JetPhi[JetPt|3]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}
        H0_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ttHh_VBF_ML"
        H1_name = "ttHh"
        H0_name = "VBF"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ttHh_VBF_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|2]", "JetPt[JetPt|3]", "JetEta[JetPt|3]", "ExtraLepPt[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "JetPhi[JetPt|2]", "JetPhi[JetPt|3]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}
        H0_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}

        mcoll_name = "D_ttHh_WHh_ML"
        H1_name = "ttHh"
        H0_name = "WHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ttHh_WHh_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|2]", "JetPt[JetPt|3]", "JetEta[JetPt|3]", "ExtraLepPt[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "JetPhi[JetPt|2]", "JetPhi[JetPt|3]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}

        mcoll_name = "D_ttHh_ttHl_ML"
        H1_name = "ttHh"
        H0_name = "ttHl"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ttHh_ttHl_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|2]", "JetPt[JetPt|3]", "JetEta[JetPt|3]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "JetPhi[JetPt|2]", "JetPhi[JetPt|3]", "ExtraLepPhi[ExtraLepPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ttHl_ggH_ML"
        H1_name = "ttHl"
        H0_name = "ggH"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ttHl_ggH_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|2]", "JetPt[JetPt|3]", "JetEta[JetPt|3]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}
        H0_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ttHl_VBF_ML"
        H1_name = "ttHl"
        H0_name = "VBF"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ttHl_VBF_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|2]", "JetPt[JetPt|3]", "JetEta[JetPt|3]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "JetPhi[JetPt|2]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut} 
        H0_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}

        mcoll_name = "D_ttHl_WHh_ML"
        H1_name = "ttHl"
        H0_name = "WHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ttHl_WHh_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|1]", "JetPt[JetPt|2]", "JetEta[JetPt|2]", "ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "JetPhi[JetPt|2]", "ExtraLepPhi[ExtraLepPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        return mcolls

    # ----------------------------------------------------------------------

    @staticmethod
    def GenerateCombinedModelCollections(MC_path, weight_path = None):
        mcolls = []
        global_max_epochs = 100
        global_hyperparams = {'LSTM_units': 16, 'LSTM_output_size': 4, 'number_dense_layers': 2, 'number_dense_neurons': 128}

        # the list inputs are common to all models
        list_input_columns = {"Jet": ["JetPt", "JetEta", "JetPhi"], "Lep": ["LepPt", "LepEta", "LepPhi"], "ExtraLep": ["ExtraLepPt", "ExtraLepEta", "ExtraLepPhi"]}
        nojet_list_input_columns = {"Lep": ["LepPt", "LepEta", "LepPhi"], "ExtraLep": ["ExtraLepPt", "ExtraLepEta", "ExtraLepPhi"]}

        # ---------------------------------------------

        H1_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_VBF_ggH_ML"
        H1_name = "VBF"
        H0_name = "ggH"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_VBF_ggH_2j_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "D_VBF2j_ggH_ME", "D_ZHh_ggH_ME", "D_WHh_ZHh_ME", "D_WHh_ggH_ME", "D_VBF2j_WHh_ME", "D_VBF2j_ZHh_ME",
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ggH_1j_ML"
        scalar_input_columns = ["D_VBF1j_ggH_ME",
                               "ZZPt", "PFMET", "ZZMassErr", "Z1Mass", "Z2Mass", "ZZMass", "Z1Pt", "Z2Pt", "ZZEta", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ggH_0j_ML"
        scalar_input_columns = ["ZZPt", "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMass"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 0 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, nojet_list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, nojet_list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_WHh_ggH_ML"
        H1_name = "WHh"
        H0_name = "ggH"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHh_ggH_2j_ML"
        scalar_input_columns = ["D_VBF2j_ggH_ME", "D_ZHh_ggH_ME", "D_WHh_ZHh_ME", "D_WHh_ggH_ME", "D_VBF2j_ZHh_ME", "D_VBF2j_WHh_ME",
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMass", "ZZMassErr"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_WHh_ggH_1j_ML"
        scalar_input_columns = ["D_VBF1j_ggH_ME",
                               "PFMET", "nCleanedJetsPt30BTagged_bTagSF",
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMass", "ZZMassErr"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_WHh_ggH_0j_ML"
        scalar_input_columns = ["PFMET",
                                "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMass", "ZZMassErr"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 0 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, nojet_list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, nojet_list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)


        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ZHh_ggH_ML"
        H1_name = "ZHh"
        H0_name = "ggH"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHh_ggH_2j_ML"
        scalar_input_columns = ["D_VBF2j_ggH_ME", "D_ZHh_ggH_ME", "D_WHh_ZHh_ME", "D_WHh_ggH_ME", "D_VBF2j_WHh_ME", "D_VBF2j_ZHh_ME",
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_ZHh_ggH_1j_ML"
        scalar_input_columns = ["D_VBF1j_ggH_ME",
                               "ZZPt", "PFMET", "ZZMassErr", "Z1Mass", "Z2Mass", "ZZMass", "Z1Pt", "Z2Pt", "ZZEta",
                               "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_ZHh_ggH_0j_ML"
        scalar_input_columns = ["ZZPt", "PFMET", "ZZMassErr", "ZZMass", "Z1Mass", "Z2Mass", "Z1Pt", "Z2Pt", "ZZEta"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 0 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, nojet_list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, nojet_list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}

        mcoll_name = "D_WHh_ZHh_ML"
        H1_name = "WHh"
        H0_name = "ZHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHh_ZHh_2j_ML"
        scalar_input_columns = ["D_VBF2j_ggH_ME", "D_VBF2j_ZHh_ME", "D_VBF2j_WHh_ME", "D_ZHh_ggH_ME", "D_WHh_ZHh_ME", "D_WHh_ggH_ME",
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMass", "ZZMassErr"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_WHh_ZHh_1j_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZMass", "ZZPt", "ZZEta", "Z2Mass", "Z1Mass", "Z2Pt", "Z1Pt", "ZZMassErr"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_WHh_ZHh_0j_ML"
        scalar_input_columns = ["PFMET",
                               "ZZMass", "ZZPt", "ZZEta", "Z2Mass", "Z1Mass", "Z2Pt", "Z1Pt", "ZZMassErr"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 0 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}
        H0_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}

        mcoll_name = "D_VBF_WHh_ML"
        H1_name = "VBF"
        H0_name = "WHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_VBF_WHh_2j_ML"
        scalar_input_columns = ["D_VBF2j_ggH_ME", "D_ZHh_ggH_ME", "D_WHh_ZHh_ME", "D_WHh_ggH_ME", "D_VBF2j_ZHh_ME", "D_VBF2j_WHh_ME",
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_WHh_1j_ML"
        scalar_input_columns = ["D_VBF1j_ggH_ME",
                               "ZZPt", "PFMET", "ZZMassErr", "Z1Mass", "Z2Mass", "Z1Pt", "Z2Pt", "ZZEta", "ZZMass"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_WHh_0j_ML"
        scalar_input_columns = ["ZZPt", "PFMET", "ZZMassErr", "Z1Mass", "Z2Mass", "Z1Pt", "Z2Pt", "ZZEta", "ZZMass"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 0 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, nojet_list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, nojet_list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}

        mcoll_name = "D_VBF_ZHh_ML"
        H1_name = "VBF"
        H0_name = "ZHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_VBF_ZHh_2j_ML"
        scalar_input_columns = ["D_VBF2j_ggH_ME", "D_ZHh_ggH_ME", "D_WHh_ggH_ME", "D_WHh_ZHh_ME", "D_VBF2j_ZHh_ME", "D_VBF2j_WHh_ME",
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ZHh_1j_ML"
        scalar_input_columns = ["D_VBF1j_ggH_ME",
                               "PFMET", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ZHh_0j_ML"
        scalar_input_columns = ["PFMET", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 0 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, nojet_list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, nojet_list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)


        # # ---------------------------------------------        

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHlept_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHlept_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_WHl_ggH_ML"
        H1_name = "WHl"
        H0_name = "ggH"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHl_ggH_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass", 
                               "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------        

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHlept_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHlept_cut}
        H0_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_WHl_VBF_ML"
        H1_name = "WHl"
        H0_name = "VBF"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHl_VBF_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------        

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHlept_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHlept_cut}
        H0_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}

        mcoll_name = "D_WHl_WHh_ML"
        H1_name = "WHl"
        H0_name = "WHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHl_WHh_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------        

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHlept_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHlept_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}

        mcoll_name = "D_WHl_ZHh_ML"
        H1_name = "WHl"
        H0_name = "ZHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHl_ZHh_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------        

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHlept_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHlept_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}

        mcoll_name = "D_WHl_ZHl_ML"
        H1_name = "WHl"
        H0_name = "ZHl"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHl_ZHl_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------        

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHlept_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHlept_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}

        mcoll_name = "D_WHl_ZHMET_ML"
        H1_name = "WHl"
        H0_name = "ZHMET"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHl_ZHMET_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------        

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHlept_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHlept_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}

        mcoll_name = "D_WHl_ttHh_ML"
        H1_name = "WHl"
        H0_name = "ttHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHl_ttHh_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------        

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHlept_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHlept_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}

        mcoll_name = "D_WHl_ttHl_ML"
        H1_name = "WHl"
        H0_name = "ttHl"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_WHl_ttHl_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}

        mcoll_name = "D_ZHh_ZHl_ML"
        H1_name = "ZHh"
        H0_name = "ZHl"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHh_ZHl_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}

        mcoll_name = "D_ZHh_ZHMET_ML"
        H1_name = "ZHh"
        H0_name = "ZHMET"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHh_ZHMET_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}

        mcoll_name = "D_ZHh_ttHh_ML"
        H1_name = "ZHh"
        H0_name = "ttHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHh_ttHh_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)
        
        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}

        mcoll_name = "D_ZHh_ttHl_ML"
        H1_name = "ZHh"
        H0_name = "ttHl"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHh_ttHl_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ZHl_ggH_ML"
        H1_name = "ZHl"
        H0_name = "ggH"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHl_ggH_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}
        H0_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ZHl_VBF_ML"
        H1_name = "ZHl"
        H0_name = "VBF"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHl_VBF_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}
        H0_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}

        mcoll_name = "D_ZHl_WHh_ML"
        H1_name = "ZHl"
        H0_name = "WHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHl_WHh_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}

        mcoll_name = "D_ZHl_ZHMET_ML"
        H1_name = "ZHl"
        H0_name = "ZHMET"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHl_ZHMET_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}

        mcoll_name = "D_ZHl_ttHh_ML"
        H1_name = "ZHl"
        H0_name = "ttHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHl_ttHh_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}

        mcoll_name = "D_ZHl_ttHl_ML"
        H1_name = "ZHl"
        H0_name = "ttHl"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHl_ttHl_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ZHMET_ggH_ML"
        H1_name = "ZHMET"
        H0_name = "ggH"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHMET_ggH_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}
        H0_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ZHMET_VBF_ML"
        H1_name = "ZHMET"
        H0_name = "VBF"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHMET_VBF_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}
        H0_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}

        mcoll_name = "D_ZHMET_WHh_ML"
        H1_name = "ZHMET"
        H0_name = "WHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHMET_WHh_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}

        mcoll_name = "D_ZHMET_ttHh_ML"
        H1_name = "ZHMET"
        H0_name = "ttHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHMET_ttHh_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}

        mcoll_name = "D_ZHMET_ttHl_ML"
        H1_name = "ZHMET"
        H0_name = "ttHl"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ZHMET_ttHl_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ttHh_ggH_ML"
        H1_name = "ttHh"
        H0_name = "ggH"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ttHh_ggH_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}
        H0_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ttHh_VBF_ML"
        H1_name = "ttHh"
        H0_name = "VBF"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ttHh_VBF_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}
        H0_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}

        mcoll_name = "D_ttHh_WHh_ML"
        H1_name = "ttHh"
        H0_name = "WHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ttHh_WHh_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}

        mcoll_name = "D_ttHh_ttHl_ML"
        H1_name = "ttHh"
        H0_name = "ttHl"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ttHh_ttHl_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ttHl_ggH_ML"
        H1_name = "ttHl"
        H0_name = "ggH"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ttHl_ggH_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}
        H0_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ttHl_VBF_ML"
        H1_name = "ttHl"
        H0_name = "VBF"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ttHl_VBF_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut} 
        H0_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}

        mcoll_name = "D_ttHl_WHh_ML"
        H1_name = "ttHl"
        H0_name = "WHh"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream, H1_name = H1_name, H0_name = H0_name)

        model_name = "D_ttHl_WHh_ML"
        scalar_input_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        return mcolls
