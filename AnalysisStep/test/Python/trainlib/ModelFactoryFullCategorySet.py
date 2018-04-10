from ModelCollection import ModelCollection
from PCAWhiteningPreprocessor import PCAWhiteningPreprocessor
from CombinedPreprocessor import CombinedPreprocessor
from RNNPreprocessor import RNNPreprocessor
from SimpleModel import SimpleModel
from CombinedModel import CombinedModel
from config import TrainingConfig
import cuts

class ModelFactoryFullCategorySet:

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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_VBF2j_ggH_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ggH_1j_ML"
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_VBF1j_ggH_ME", "leading_jet_eta"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ggH_0j_ML"
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 0 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_WHh_ggH_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_WHh_ggH_1j_ML"
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "leading_jet_eta"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_WHh_ggH_0j_ML"
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 0 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_ZHh_ggH_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_ZHh_ggH_1j_ML"
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "leading_jet_eta"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_ZHh_ggH_0j_ML"
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 0 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_WHh_ZHh_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_WHh_ZHh_01j_ML"
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] < 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_VBF2j_WHh_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_WHh_1j_ML"
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "leading_jet_eta"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_WHh_0j_ML"
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 0 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_VBF2j_ZHh_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ZHh_1j_ML"
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "leading_jet_eta"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ZHh_0j_ML"
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 0 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = SimpleModel(model_name, input_columns, global_hyperparams)
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

        # ---------------------------------------------

        H1_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_VBF_ggH_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_VBF_ggH_2j_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_VBF2j_ggH_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ggH_1j_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_VBF1j_ggH_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ggH_0j_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 0 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
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
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_WHh_ggH_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_WHh_ggH_01j_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] < 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
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
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_ZHh_ggH_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_ZHh_ggH_01j_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] < 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
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
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_WHh_ZHh_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_WHh_ZHh_01j_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] < 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_VBF_WHh_2j_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_VBF2j_WHh_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_WHh_01j_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] < 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
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
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_VBF2j_ZHh_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        model_name = "D_VBF_ZHh_01j_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] < 2 and row["ZZMass"] > 118. and row["ZZMass"] < 130.
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
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_WHl_ggH_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_WHl_ggH_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_WHl_VBF_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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
        H0_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}

        mcoll_name = "D_WHl_WHh_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_WHl_WHh_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}

        mcoll_name = "D_WHl_ZHh_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_WHl_ZHh_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}

        mcoll_name = "D_WHl_ZHl_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_WHl_ZHl_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}

        mcoll_name = "D_WHl_ZHMET_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_WHl_ZHMET_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}

        mcoll_name = "D_WHl_ttHh_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_WHl_ttHh_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}

        mcoll_name = "D_WHl_ttHl_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_WHl_ttHl_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}

        mcoll_name = "D_ZHh_ZHl_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHh_ZHl_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}

        mcoll_name = "D_ZHh_ZHMET_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHh_ZHMET_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}

        mcoll_name = "D_ZHh_ttHh_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHh_ttHh_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}

        mcoll_name = "D_ZHh_ttHl_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHh_ttHl_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ZHl_ggH_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHl_ggH_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}
        H0_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ZHl_VBF_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHl_VBF_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}
        H0_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}

        mcoll_name = "D_ZHl_WHh_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHl_WHh_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}

        mcoll_name = "D_ZHl_ZHMET_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHl_ZHMET_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}

        mcoll_name = "D_ZHl_ttHh_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHl_ttHh_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}

        mcoll_name = "D_ZHl_ttHl_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHl_ttHl_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ZHMET_ggH_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHMET_ggH_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}
        H0_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ZHMET_VBF_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHMET_VBF_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}
        H0_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}

        mcoll_name = "D_ZHMET_WHh_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHMET_WHh_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}

        mcoll_name = "D_ZHMET_ttHh_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHMET_ttHh_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}

        mcoll_name = "D_ZHMET_ttHl_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ZHMET_ttHl_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ttHh_ggH_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ttHh_ggH_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}
        H0_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ttHh_VBF_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ttHh_VBF_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}
        H0_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}

        mcoll_name = "D_ttHh_WHh_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ttHh_WHh_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut}
        H0_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}

        mcoll_name = "D_ttHh_ttHl_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ttHh_ttHl_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ttHl_ggH_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ttHl_ggH_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut}
        H0_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ttHl_VBF_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ttHl_VBF_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
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

        H1_stream = {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut} 
        H0_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut}

        mcoll_name = "D_ttHl_WHh_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        model_name = "D_ttHl_WHh_ML"
        scalar_input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep"]
        preprocessor_cuts = lambda row: row["ZZMass"] > 118. and row["ZZMass"] < 130.
        pre = CombinedPreprocessor(model_name, scalar_input_columns, PCAWhiteningPreprocessor, list_input_columns, RNNPreprocessor, preprocessor_cuts)
        mod = CombinedModel(model_name, scalar_input_columns, list_input_columns, global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        return mcolls
