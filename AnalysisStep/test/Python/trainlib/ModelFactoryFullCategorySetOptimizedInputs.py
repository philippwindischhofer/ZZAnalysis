from ModelCollection import ModelCollection
from FlexiblePCAWhiteningPreprocessor import FlexiblePCAWhiteningPreprocessor
from SimpleModel import SimpleModel
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_VBF_ggH_2j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|0]", "JetEta[JetPt|1]", 
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "D_VBF2j_ggH_ME", "D_ZHh_ggH_ME", "D_WHh_ZHh_ME", "D_WHh_ggH_ME",
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ggH_1j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", 
                               "ZZPt", "D_VBF1j_ggH_ME", "PFMET", "ZZMassErr", "Z1Mass", "Z2Mass", "Z1Pt", "Z2Pt", "ZZEta"]
        periodic_columns = ["JetPhi[JetPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ggH_0j_ML"
        nonperiodic_columns = ["ZZPt", "PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z2Mass"]
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_WHh_ggH_2j_ML"

        nonperiodic_columns = ["JetPt[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|0]", "JetEta[JetPt|1]", 
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "D_VBF2j_ggH_ME", "D_ZHh_ggH_ME", "D_WHh_ZHh_ME", "D_WHh_ggH_ME", "D_VBF2j_ZHh_ME", "D_VBF2j_WHh_ME" ,
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_WHh_ggH_1j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]",
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "D_VBF1j_ggH_ME",
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr"]
        periodic_columns = ["JetPhi[JetPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_WHh_ggH_0j_ML"
        nonperiodic_columns = ["ExtraLepPt[ExtraLepPt|0]",
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr"]
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHh_ggH_2j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|0]", "JetEta[JetPt|1]", 
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "D_VBF2j_ggH_ME", "D_ZHh_ggH_ME", "D_WHh_ZHh_ME", "D_WHh_ggH_ME", "D_VBF2j_WHh_ME", "D_VBF2j_ZHh_ME",
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_ZHh_ggH_1j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", 
                               "ZZPt", "D_VBF1j_ggH_ME", "PFMET", "ZZMassErr", "Z1Mass", "Z2Mass", "Z1Pt", "Z2Pt", "ZZEta",
                               "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = ["JetPhi[JetPt|0]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_ZHh_ggH_0j_ML"
        nonperiodic_columns = ["ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]",
                               "ZZPt", "PFMET", "ZZMassErr", "Z1Mass", "Z2Mass", "Z1Pt", "Z2Pt", "ZZEta"]
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
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}

        mcoll_name = "D_WHh_ZHh_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_WHh_ZHh_2j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|0]", "JetEta[JetPt|1]", 
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "D_VBF2j_ggH_ME", "D_VBF2j_ZHh_ME", "D_VBF2j_WHh_ME", "D_ZHh_ggH_ME", "D_WHh_ZHh_ME", "D_WHh_ggH_ME",
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_WHh_ZHh_01j_ML"
        nonperiodic_columns = ["ExtraLepPt[ExtraLepPt|0]", "ExtraLepEta[ExtraLepPt|0]", "JetPt[JetPt|0]", "JetEta[JetPt|0]",
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", "ZZMass", "ZZPt", "ZZEta", "Z2Mass", "Z1Mass", "Z2Pt", "Z1Pt", "ZZMassErr"]
        periodic_columns = []
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] < 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_VBF_WHh_2j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|0]", "JetEta[JetPt|1]", 
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "D_VBF2j_ggH_ME", "D_ZHh_ggH_ME", "D_WHh_ZHh_ME", "D_WHh_ggH_ME", "D_VBF2j_ZHh_ME", "D_VBF2j_WHh_ME",
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]", "ZZPhi"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_WHh_1j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]", 
                               "ZZPt", "D_VBF1j_ggH_ME", "PFMET", "ZZMassErr", "Z1Mass", "Z2Mass", "Z1Pt", "Z2Pt", "ZZEta", "ZZMass"]
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_VBF_ZHh_2j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetPt[JetPt|1]", "JetEta[JetPt|0]", "JetEta[JetPt|1]", 
                               "PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "D_VBF2j_ggH_ME", "D_ZHh_ggH_ME", "D_WHh_ggH_ME", "D_WHh_ZHh_ME", "D_VBF2j_ZHh_ME", "D_VBF2j_WHh_ME",
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass"]
        periodic_columns = ["JetPhi[JetPt|0]", "JetPhi[JetPt|1]"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ZHh_1j_ML"
        nonperiodic_columns = ["JetPt[JetPt|0]", "JetEta[JetPt|0]",
                               "PFMET", "nCleanedJetsPt30BTagged_bTagSF", 
                               "D_VBF1j_ggH_ME",
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass"]
        periodic_columns = ["JetPhi[JetPt|0]", "ZZPhi"]        
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = FlexiblePCAWhiteningPreprocessor(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, pre.number_processed_columns(), global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ZHh_0j_ML"
        nonperiodic_columns = ["ExtraLepPt[ExtraLepPt|0]",
                               "PFMET", "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass"]
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_WHl_ggH_ML"
        nonperiodic_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                               "ZZPt", "Z2Pt", "Z1Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZMassErr", "ZZMass", "nExtraLep"]
        periodic_columns = []        
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_WHl_VBF_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_WHl_WHh_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_WHl_ZHh_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_WHl_ZHl_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_WHl_ZHMET_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_WHl_ttHh_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_WHl_ttHl_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHh_ZHl_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []        
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHh_ZHMET_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHh_ttHh_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHh_ttHl_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHl_ggH_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHl_VBF_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHl_WHh_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHl_ZHMET_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHl_ttHh_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHl_ttHl_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHMET_ggH_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHMET_VBF_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHMET_WHh_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHMET_ttHh_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHMET_ttHl_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ttHh_ggH_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ttHh_VBF_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ttHh_WHh_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ttHh_ttHl_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ttHl_ggH_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ttHl_VBF_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ttHl_WHh_ML"
        nonperiodic_columns = ["PFMET", "Z1Pt", "Z2Pt", "ZZEta", "Z1Mass", "Z2Mass", "ZZPt", "ZZMassErr", "ZZMass", "nExtraLep",
                               "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_columns = []
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
