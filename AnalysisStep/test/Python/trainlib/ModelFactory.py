from ModelCollection import ModelCollection
from PCAWhiteningPreprocessor import PCAWhiteningPreprocessor
from config import TrainingConfig
import cuts

class ModelFactory:

    # this will generate a list of ModelCollections, each one of which will have to be trained later one
    @staticmethod
    def GenerateModelCollections(base_model, MC_path, weight_path = None):
        mcolls = []
        global_max_epochs = 100

        WHhadr_cut = lambda row: cuts.countAssocLeptons(row) == 0 and cuts.mZZ_cut(row)
        ZHhadr_cut = lambda row: cuts.countAssocLeptons(row) == 0 and cuts.countNeutrinos(row) == 0 and cuts.mZZ_cut(row)

        # ---------------------------------------------

        H1_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_VBF_ggH_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        # this is the model that will take the place of the VBF2jet/ggH discriminant.
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_VBF2j_ggH_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2
        pre = PCAWhiteningPreprocessor(processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = base_model("D_VBF_ggH_2j_ML")
        mod.build(len(input_columns))
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        # this is the model that will take the place of the VBF2jet/ggH discriminant.
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_VBF1j_ggH_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] == 1
        pre = PCAWhiteningPreprocessor(processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = base_model("D_VBF_ggH_1j_ML")
        mod.build(len(input_columns))
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": WHhadr_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_WHh_ggH_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_WHh_ggH_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2
        pre = PCAWhiteningPreprocessor(processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = base_model("D_WHh_ggH_2j_ML")
        mod.build(len(input_columns))
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": ZHhadr_cut}
        H0_stream = {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.mZZ_cut}

        mcoll_name = "D_ZHh_ggH_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_ZHh_ggH_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2
        pre = PCAWhiteningPreprocessor(processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = base_model("D_ZHh_ggH_2j_ML")
        mod.build(len(input_columns))
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": WHhadr_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": ZHhadr_cut}

        mcoll_name = "D_WHh_ZHh_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_WHh_ZHh_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2
        pre = PCAWhiteningPreprocessor(processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = base_model("D_WHh_ZHh_2j_ML")
        mod.build(len(input_columns))
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}
        H0_stream = {MC_path + "WplusH125/ZZ4lAnalysis.root": WHhadr_cut,
                     MC_path + "WminusH125/ZZ4lAnalysis.root": WHhadr_cut}

        mcoll_name = "D_VBF_WHh_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        # this is the model that will take the place of the VBF2jet/ggH discriminant.
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_VBF2j_WHh_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2
        pre = PCAWhiteningPreprocessor(processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = base_model("D_VBF_WHh_2j_ML")
        mod.build(len(input_columns))
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_stream = {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.mZZ_cut}
        H0_stream = {MC_path + "ZH125/ZZ4lAnalysis.root": ZHhadr_cut}

        mcoll_name = "D_VBF_ZHh_ML"
        mcoll = ModelCollection(mcoll_name, H1_stream, H0_stream)

        # this is the model that will take the place of the VBF2jet/ggH discriminant.
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_VBF2j_ZHh_ME"]
        preprocessor_cuts = lambda row: row["nCleanedJetsPt30"] >= 2
        pre = PCAWhiteningPreprocessor(processed_columns = input_columns, cuts = preprocessor_cuts)
        mod = base_model("D_VBF_ZHh_2j_ML")
        mod.build(len(input_columns))
        sett = TrainingConfig(max_epochs = global_max_epochs)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------        

        return mcolls
