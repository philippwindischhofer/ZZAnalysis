from ModelCollection import ModelCollection
from PCAWhiteningPreprocessor import PCAWhiteningPreprocessor
from config import TrainingConfig
import cuts

class ModelFactory:

    # this will generate a list of ModelCollections, each one of which will have to be trained later one
    @staticmethod
    def GenerateStandardModelCollections(base_model, MC_path, weight_path = None):
        mcolls = []
        global_max_epochs = 100
        global_hyperparams = {'number_layers': 2}

        # ---------------------------------------------

        H1_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_VBF_ggH_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        # this is the model that will take the place of the VBF2jet/ggH discriminant.
        model_name = "D_VBF_ggH_2j_ML"
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_VBF2j_ggH_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = base_model(model_name, input_columns)
        mod.build(global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        # this is the model that will take the place of the VBF2jet/ggH discriminant.
        model_name = "D_VBF_ggH_1j_ML"
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_VBF1j_ggH_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = base_model(model_name, input_columns)
        mod.build(global_hyperparams)
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
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = base_model(model_name, input_columns)
        mod.build(global_hyperparams)
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
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = base_model(model_name, input_columns)
        mod.build(global_hyperparams)
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
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = base_model(model_name, input_columns)
        mod.build(global_hyperparams)
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
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2
        pre = PCAWhiteningPreprocessor(name = model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = base_model(model_name + "_input", input_columns)
        mod.build(global_hyperparams)
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
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2
        pre = PCAWhiteningPreprocessor(model_name + "_input", processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = base_model(model_name, input_columns)
        mod.build(global_hyperparams)
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------        

        return mcolls