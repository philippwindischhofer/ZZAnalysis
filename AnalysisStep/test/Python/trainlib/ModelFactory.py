from ModelCollection import ModelCollection
from PCAWhiteningPreprocessor import PCAWhiteningPreprocessor
from config import TrainingConfig

class ModelFactory:

    # this will generate a list of ModelCollections, each one of which will have to be trained later one
    @staticmethod
    def GenerateModelCollections(base_model, weight_path = None):
        mcolls = []

        # ---------------------------------------------

        H1_paths = ["/data_CMS/cms/wind/CJLST_NTuples/VBFH125/ZZ4lAnalysis.root"]
        H0_paths = ["/data_CMS/cms/wind/CJLST_NTuples/ggH125/ZZ4lAnalysis.root"]

        mcoll_name = "D_VBF_ggH_ML"
        mcoll = ModelCollection(mcoll_name, H1_paths, H0_paths)

        # this is the model that will take the place of the VBF2jet/ggH discriminant.
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_VBF2j_ggH_ME"]
        cuts = lambda row: row["nCleanedJetsPt30"] >= 2
        pre = PCAWhiteningPreprocessor(processed_columns = input_columns, cuts = cuts)
        mod = base_model("D_VBF_ggH_2j_ML")
        mod.build(len(input_columns))
        sett = TrainingConfig(max_epochs = 100)
        mcoll.add_model(pre, mod, sett)

        # this is the model that will take the place of the VBF2jet/ggH discriminant.
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_VBF1j_ggH_ME"]
        cuts = lambda row: row["nCleanedJetsPt30"] == 1
        pre = PCAWhiteningPreprocessor(processed_columns = input_columns, cuts = cuts)
        mod = base_model("D_VBF_ggH_1j_ML")
        mod.build(len(input_columns))
        sett = TrainingConfig(max_epochs = 100)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_paths = ["/data_CMS/cms/wind/CJLST_NTuples/WplusH125/ZZ4lAnalysis.root",
                    "/data_CMS/cms/wind/CJLST_NTuples/WminusH125/ZZ4lAnalysis.root"]
        H0_paths = ["/data_CMS/cms/wind/CJLST_NTuples/ggH125/ZZ4lAnalysis.root"]

        mcoll_name = "D_WHh_ggH_ML"
        mcoll = ModelCollection(mcoll_name, H1_paths, H0_paths)

        # this is the model that will take the place of the VBF2jet/ggH discriminant.
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_WHh_ggH_ME"]
        cuts = lambda row: row["nCleanedJetsPt30"] >= 2
        pre = PCAWhiteningPreprocessor(processed_columns = input_columns, cuts = cuts)
        mod = base_model("D_WHh_ggH_2j_ML")
        mod.build(len(input_columns))
        sett = TrainingConfig(max_epochs = 100)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_paths = ["/data_CMS/cms/wind/CJLST_NTuples/ZH125/ZZ4lAnalysis.root"]
        H0_paths = ["/data_CMS/cms/wind/CJLST_NTuples/ggH125/ZZ4lAnalysis.root"]

        mcoll_name = "D_ZHh_ggH_ML"
        mcoll = ModelCollection(mcoll_name, H1_paths, H0_paths)

        # this is the model that will take the place of the VBF2jet/ggH discriminant.
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_ZHh_ggH_ME"]
        cuts = lambda row: row["nCleanedJetsPt30"] >= 2
        pre = PCAWhiteningPreprocessor(processed_columns = input_columns, cuts = cuts)
        mod = base_model("D_ZHh_ggH_2j_ML")
        mod.build(len(input_columns))
        sett = TrainingConfig(max_epochs = 100)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_paths = ["/data_CMS/cms/wind/CJLST_NTuples/WplusH125/ZZ4lAnalysis.root",
                    "/data_CMS/cms/wind/CJLST_NTuples/WminusH125/ZZ4lAnalysis.root"]
        H0_paths = ["/data_CMS/cms/wind/CJLST_NTuples/ZH125/ZZ4lAnalysis.root"]

        mcoll_name = "D_WHh_ZHh_ML"
        mcoll = ModelCollection(mcoll_name, H1_paths, H0_paths)

        # this is the model that will take the place of the VBF2jet/ggH discriminant.
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_WHh_ZHh_ME"]
        cuts = lambda row: row["nCleanedJetsPt30"] >= 2
        pre = PCAWhiteningPreprocessor(processed_columns = input_columns, cuts = cuts)
        mod = base_model("D_WHh_ZHh_2j_ML")
        mod.build(len(input_columns))
        sett = TrainingConfig(max_epochs = 100)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_paths = ["/data_CMS/cms/wind/CJLST_NTuples/VBFH125/ZZ4lAnalysis.root"]
        H0_paths = ["/data_CMS/cms/wind/CJLST_NTuples/WplusH125/ZZ4lAnalysis.root",
                    "/data_CMS/cms/wind/CJLST_NTuples/WminusH125/ZZ4lAnalysis.root"]

        mcoll_name = "D_VBF_WHh_ML"
        mcoll = ModelCollection(mcoll_name, H1_paths, H0_paths)

        # this is the model that will take the place of the VBF2jet/ggH discriminant.
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_VBF2j_WHh_ME"]
        cuts = lambda row: row["nCleanedJetsPt30"] >= 2
        pre = PCAWhiteningPreprocessor(processed_columns = input_columns, cuts = cuts)
        mod = base_model("D_VBF_WHh_2j_ML")
        mod.build(len(input_columns))
        sett = TrainingConfig(max_epochs = 100)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------

        H1_paths = ["/data_CMS/cms/wind/CJLST_NTuples/VBFH125/ZZ4lAnalysis.root"]
        H0_paths = ["/data_CMS/cms/wind/CJLST_NTuples/ZH125/ZZ4lAnalysis.root"]

        mcoll_name = "D_VBF_ZHh_ML"
        mcoll = ModelCollection(mcoll_name, H1_paths, H0_paths)

        # this is the model that will take the place of the VBF2jet/ggH discriminant.
        input_columns = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", 
                         "nExtraLep", "D_VBF2j_ZHh_ME"]
        cuts = lambda row: row["nCleanedJetsPt30"] >= 2
        pre = PCAWhiteningPreprocessor(processed_columns = input_columns, cuts = cuts)
        mod = base_model("D_VBF_ZHh_2j_ML")
        mod.build(len(input_columns))
        sett = TrainingConfig(max_epochs = 100)
        mcoll.add_model(pre, mod, sett)

        if weight_path is not None:
            mcoll.load_weights(weight_path + mcoll_name)
        mcolls.append(mcoll)

        # # ---------------------------------------------        

        return mcolls
