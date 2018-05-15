from ModelCollection import ModelCollection
from Category import Category
from DiscriminantEndpiece import DiscriminantEndpiece
from DiscriminantEndpieceComponent import DiscriminantEndpieceComponent
from FlexiblePCAWhiteningPreprocessor import FlexiblePCAWhiteningPreprocessor
from PCAWhiteningPreprocessor import PCAWhiteningPreprocessor
from SimpleModel import SimpleModel
from config import TrainingConfig
import cuts

class ModelFactoryFullMassRangeDynamicInclusive:

    @staticmethod
    def GenerateSimpleModelCollections(MC_path, weight_path = None, input_config_file = None, mass_point = 125.0):
        mcolls = []
        global_max_epochs = 100
        global_hyperparams = {'number_layers': 2, 'number_neurons': 128}

        # some shortcuts
        nonperiodic_variables_default_0j = ["PFMET", "Z1Pt", "Z2Pt", "Z1Mass", "Z2Mass", "ZZPt", "ZZEta", "ZZMass", "ZZMassErr"]
        nonperiodic_variables_default_1j = nonperiodic_variables_default_0j + ["nCleanedJetsPt30BTagged_bTagSF"]
        nonperiodic_variables_default = nonperiodic_variables_default_0j + ["nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF"]
        periodic_variables_default = ["ZZPhi"]

        def nonperiodic_variables_jet(num):
            retval = []
            for i in range(num):
                retval.append("JetPt[JetPt|{:d}]".format(i)) 
                retval.append("JetEta[JetPt|{:d}]".format(i)) 
            return retval

        def periodic_variables_jet(num):
            retval = []
            for i in range(num):
                retval.append("JetPhi[JetPt|{:d}]".format(i)) 
            return retval

        def nonperiodic_variables_extra_lep(num):
            retval = []
            for i in range(num):
                retval.append("ExtraLepPt[ExtraLepPt|{:d}]".format(i)) 
                retval.append("ExtraLepEta[ExtraLepPt|{:d}]".format(i)) 
            return retval

        def periodic_variables_extra_lep(num):
            retval = []
            for i in range(num):
                retval.append("ExtraLepPhi[ExtraLepPt|{:d}]".format(i)) 
            return retval

        MELA_2j_variables = ["D_VBF2j_ggH_ME", "D_ZHh_ggH_ME", "D_WHh_ZHh_ME", "D_WHh_ggH_ME", "D_VBF2j_WHh_ME", "D_VBF2j_ZHh_ME"]
        MELA_1j_variables = ["D_VBF1j_ggH_ME"]

        # translation dictionaries that link Higgs mass points to the corresponding training files
        mass_point_translation = {120.0: "120", 124.0: "124", 125.0: "125", 126.0: "126", 130.0: "130"}
        mass_point_suffix = mass_point_translation[mass_point]

        # ------------------------

        # define the categories for the classifier
        VBF_cat = Category("VBF", {MC_path + "VBFH" + mass_point_suffix + "/ZZ4lAnalysis.root": cuts.no_cut})

        # mode that is fully differential in the number of jets
        ep = DiscriminantEndpiece("2|1|0j")
        ep_comp = DiscriminantEndpieceComponent(name = "2j", public_name = "2j",
                                                component_cut = lambda row: row["nCleanedJetsPt30"] >= 2,
                                                nonperiodic_columns = nonperiodic_variables_default + MELA_2j_variables + nonperiodic_variables_jet(2),
                                                periodic_columns = periodic_variables_default + periodic_variables_jet(2),
                                                model_basetype = SimpleModel,
                                                model_hyperparams = global_hyperparams,
                                                preprocessor_basetype = FlexiblePCAWhiteningPreprocessor)
        ep.add_component(ep_comp)        
        ep_comp = DiscriminantEndpieceComponent(name = "1j", public_name = "1j",
                                                component_cut = lambda row: row["nCleanedJetsPt30"] == 1,
                                                nonperiodic_columns = nonperiodic_variables_default + MELA_1j_variables + nonperiodic_variables_jet(1),
                                                periodic_columns = periodic_variables_default + periodic_variables_jet(1),
                                                model_basetype = SimpleModel,
                                                model_hyperparams = global_hyperparams,
                                                preprocessor_basetype = FlexiblePCAWhiteningPreprocessor)
        ep.add_component(ep_comp)        
        ep_comp = DiscriminantEndpieceComponent(name = "0j", public_name = "0j",
                                                component_cut = lambda row: row["nCleanedJetsPt30"] == 0,
                                                nonperiodic_columns = nonperiodic_variables_default_0j,
                                                periodic_columns = [],
                                                model_basetype = SimpleModel,
                                                model_hyperparams = global_hyperparams,
                                                preprocessor_basetype = FlexiblePCAWhiteningPreprocessor)
        ep.add_component(ep_comp)        
        VBF_cat.add_endpiece(ep)
        
        # mode that is inclusive in the number of jets
        ep = DiscriminantEndpiece("210j")        
        ep_comp = DiscriminantEndpieceComponent(name = "210j", public_name = "",
                                                component_cut = lambda row: True,
                                                nonperiodic_columns = nonperiodic_variables_default + nonperiodic_variables_jet(2),
                                                periodic_columns = periodic_variables_default + periodic_variables_jet(2),
                                                model_basetype = SimpleModel,
                                                model_hyperparams = global_hyperparams,
                                                preprocessor_basetype = FlexiblePCAWhiteningPreprocessor)
        ep.add_component(ep_comp)
        VBF_cat.add_endpiece(ep)

        # ------------------------------------

        # define the categories for the classifier
        ggH_cat = Category("ggH", {MC_path + "ggH" + mass_point_suffix + "/ZZ4lAnalysis.root": cuts.no_cut})

        # mode that is fully differential in the number of jets
        ep = DiscriminantEndpiece("2|1|0j")
        ep_comp = DiscriminantEndpieceComponent(name = "2j", public_name = "2j",
                                                component_cut = lambda row: row["nCleanedJetsPt30"] >= 2,
                                                nonperiodic_columns = nonperiodic_variables_default + MELA_2j_variables + nonperiodic_variables_jet(2),
                                                periodic_columns = periodic_variables_default + periodic_variables_jet(2),
                                                model_basetype = SimpleModel,
                                                model_hyperparams = global_hyperparams,
                                                preprocessor_basetype = FlexiblePCAWhiteningPreprocessor)
        ep.add_component(ep_comp)        
        ep_comp = DiscriminantEndpieceComponent(name = "1j", public_name = "1j",
                                                component_cut = lambda row: row["nCleanedJetsPt30"] == 1,
                                                nonperiodic_columns = nonperiodic_variables_default_1j + MELA_1j_variables + nonperiodic_variables_jet(1),
                                                periodic_columns = periodic_variables_default + periodic_variables_jet(1),
                                                model_basetype = SimpleModel,
                                                model_hyperparams = global_hyperparams,
                                                preprocessor_basetype = FlexiblePCAWhiteningPreprocessor)
        ep.add_component(ep_comp)        
        ep_comp = DiscriminantEndpieceComponent(name = "0j", public_name = "0j",
                                                component_cut = lambda row: row["nCleanedJetsPt30"] == 0,
                                                nonperiodic_columns = nonperiodic_variables_default_0j,
                                                periodic_columns = [],
                                                model_basetype = SimpleModel,
                                                model_hyperparams = global_hyperparams,
                                                preprocessor_basetype = FlexiblePCAWhiteningPreprocessor)
        ep.add_component(ep_comp)        
        ggH_cat.add_endpiece(ep)
        
        # mode that is inclusive in the number of jets
        ep = DiscriminantEndpiece("210j")        
        ep_comp = DiscriminantEndpieceComponent(name = "210j", public_name = "",
                                                component_cut = lambda row: True,
                                                nonperiodic_columns = nonperiodic_variables_default + nonperiodic_variables_jet(2),
                                                periodic_columns = periodic_variables_default + periodic_variables_jet(2),
                                                model_basetype = SimpleModel,
                                                model_hyperparams = global_hyperparams,
                                                preprocessor_basetype = FlexiblePCAWhiteningPreprocessor)
        ep.add_component(ep_comp)
        ggH_cat.add_endpiece(ep)
        
        # ------------------------------------

        # define the categories for the classifier
        WHh_cat = Category("WHh", {MC_path + "WplusH" + mass_point_suffix + "/ZZ4lAnalysis.root": cuts.WHhadr_cut,
                                   MC_path + "WminusH" + mass_point_suffix + "/ZZ4lAnalysis.root": cuts.WHhadr_cut})
        
        # mode that is inclusive in the number of jets
        ep = DiscriminantEndpiece("210j")        
        ep_comp = DiscriminantEndpieceComponent(name = "210j", public_name = "",
                                                component_cut = lambda row: True,
                                                nonperiodic_columns = nonperiodic_variables_default + nonperiodic_variables_jet(2),
                                                periodic_columns = periodic_variables_default + periodic_variables_jet(2),
                                                model_basetype = SimpleModel,
                                                model_hyperparams = global_hyperparams,
                                                preprocessor_basetype = FlexiblePCAWhiteningPreprocessor)
        ep.add_component(ep_comp)
        WHh_cat.add_endpiece(ep)

        # ------------------------------------

        # define the categories for the classifier
        ZHh_cat = Category("ZHh", {MC_path + "ZH" + mass_point_suffix + "/ZZ4lAnalysis.root": cuts.ZHhadr_cut})
        
        # mode that is inclusive in the number of jets
        ep = DiscriminantEndpiece("210j")        
        ep_comp = DiscriminantEndpieceComponent(name = "210j", public_name = "",
                                                component_cut = lambda row: True,
                                                nonperiodic_columns = nonperiodic_variables_default + nonperiodic_variables_jet(2),
                                                periodic_columns = periodic_variables_default + periodic_variables_jet(2),
                                                model_basetype = SimpleModel,
                                                model_hyperparams = global_hyperparams,
                                                preprocessor_basetype = FlexiblePCAWhiteningPreprocessor)
        ep.add_component(ep_comp)
        ZHh_cat.add_endpiece(ep)

        # ------------------------------------
        
        # define the categories for the classifier
        ZHl_cat = Category("ZHl", {MC_path + "ZH" + mass_point_suffix +  "/ZZ4lAnalysis.root": cuts.ZHlept_cut})
        
        # mode that is inclusive in the number of jets
        ep = DiscriminantEndpiece("210j")        
        ep_comp = DiscriminantEndpieceComponent(name = "210j", public_name = "",
                                                component_cut = lambda row: True,
                                                nonperiodic_columns = nonperiodic_variables_default + nonperiodic_variables_extra_lep(2) + nonperiodic_variables_jet(2),
                                                periodic_columns = periodic_variables_default + periodic_variables_extra_lep(2) + periodic_variables_jet(2),
                                                model_basetype = SimpleModel,
                                                model_hyperparams = global_hyperparams,
                                                preprocessor_basetype = FlexiblePCAWhiteningPreprocessor)
        ep.add_component(ep_comp)
        ZHl_cat.add_endpiece(ep)
        
        # ------------------------------------
        
        # define the categories for the classifier
        WHl_cat = Category("WHl", {MC_path + "WplusH" + mass_point_suffix + "/ZZ4lAnalysis.root": cuts.WHlept_cut,
                                   MC_path + "WminusH" + mass_point_suffix + "/ZZ4lAnalysis.root": cuts.WHlept_cut})
        
        # mode that is inclusive in the number of jets
        ep = DiscriminantEndpiece("210j")        
        ep_comp = DiscriminantEndpieceComponent(name = "210j", public_name = "",
                                                component_cut = lambda row: True,
                                                nonperiodic_columns = nonperiodic_variables_default + nonperiodic_variables_extra_lep(2) + nonperiodic_variables_jet(2),
                                                periodic_columns = periodic_variables_default + periodic_variables_extra_lep(2) + periodic_variables_jet(2),
                                                model_basetype = SimpleModel,
                                                model_hyperparams = global_hyperparams,
                                                preprocessor_basetype = FlexiblePCAWhiteningPreprocessor)
        ep.add_component(ep_comp)
        WHl_cat.add_endpiece(ep)

        # ------------------------------------
        
        # define the categories for the classifier
        ZHMET_cat = Category("ZHMET", {MC_path + "ZH" + mass_point_suffix + "/ZZ4lAnalysis.root": cuts.ZHMET_cut})
        
        # mode that is inclusive in the number of jets
        ep = DiscriminantEndpiece("210j")        
        ep_comp = DiscriminantEndpieceComponent(name = "210j", public_name = "",
                                                component_cut = lambda row: True,
                                                nonperiodic_columns = nonperiodic_variables_default + nonperiodic_variables_jet(2),
                                                periodic_columns = periodic_variables_default + periodic_variables_jet(2),
                                                model_basetype = SimpleModel,
                                                model_hyperparams = global_hyperparams,
                                                preprocessor_basetype = FlexiblePCAWhiteningPreprocessor)
        ep.add_component(ep_comp)
        ZHMET_cat.add_endpiece(ep)

        # ------------------------------------
        
        # define the categories for the classifier
        ttHh_cat = Category("ttHh", {MC_path + "ttH" + mass_point_suffix + "/ZZ4lAnalysis.root": cuts.ttHhadr_cut})
        
        # mode that is inclusive in the number of jets
        ep = DiscriminantEndpiece("210j")        
        ep_comp = DiscriminantEndpieceComponent(name = "210j", public_name = "",
                                                component_cut = lambda row: True,
                                                nonperiodic_columns = nonperiodic_variables_default + nonperiodic_variables_jet(2) + nonperiodic_variables_extra_lep(2),
                                                periodic_columns = periodic_variables_default + periodic_variables_jet(2) + periodic_variables_extra_lep(2),
                                                model_basetype = SimpleModel,
                                                model_hyperparams = global_hyperparams,
                                                preprocessor_basetype = FlexiblePCAWhiteningPreprocessor)
        ep.add_component(ep_comp)
        ttHh_cat.add_endpiece(ep)

        # ------------------------------------
        
        # define the categories for the classifier
        ttHl_cat = Category("ttHl", {MC_path + "ttH" + mass_point_suffix + "/ZZ4lAnalysis.root": cuts.ttHlept_cut})
        
        # mode that is inclusive in the number of jets
        ep = DiscriminantEndpiece("210j")        
        ep_comp = DiscriminantEndpieceComponent(name = "210j", public_name = "",
                                                component_cut = lambda row: True,
                                                nonperiodic_columns = nonperiodic_variables_default + nonperiodic_variables_extra_lep(2) + nonperiodic_variables_jet(2),
                                                periodic_columns = periodic_variables_default + periodic_variables_extra_lep(2) + periodic_variables_jet(2),
                                                model_basetype = SimpleModel,
                                                model_hyperparams = global_hyperparams,
                                                preprocessor_basetype = FlexiblePCAWhiteningPreprocessor)
        ep.add_component(ep_comp)
        ttHl_cat.add_endpiece(ep)

        # ------------------------------------

        # for training on Z+X only
        ZX_datastream = {MC_path + "AllData/ZZ4lAnalysis.root": cuts.no_cut}
        ZX_cat = Category("ZX", {MC_path + "ZX/ZZ4lAnalysis.root": cuts.no_cut}, datastream_unmixed = ZX_datastream)
        
        # mode that is inclusive in the number of jets
        ep = DiscriminantEndpiece("210j")        
        ep_comp = DiscriminantEndpieceComponent(name = "210j", public_name = "",
                                                component_cut = lambda row: True,
                                                nonperiodic_columns = nonperiodic_variables_default + nonperiodic_variables_extra_lep(2) + nonperiodic_variables_jet(2),
                                                periodic_columns = periodic_variables_default + periodic_variables_extra_lep(2) + periodic_variables_jet(2),
                                                model_basetype = SimpleModel,
                                                model_hyperparams = global_hyperparams,
                                                preprocessor_basetype = FlexiblePCAWhiteningPreprocessor)
        ep.add_component(ep_comp)
        ZX_cat.add_endpiece(ep)

        # ------------------------------------

        # for training on Z+X only
        qq_cat = Category("qq", {MC_path + "ZZTo4l/ZZ4lAnalysis.root": cuts.no_cut})
        
        # mode that is inclusive in the number of jets
        ep = DiscriminantEndpiece("210j")        
        ep_comp = DiscriminantEndpieceComponent(name = "210j", public_name = "",
                                                component_cut = lambda row: True,
                                                nonperiodic_columns = nonperiodic_variables_default + nonperiodic_variables_extra_lep(2) + nonperiodic_variables_jet(2),
                                                periodic_columns = periodic_variables_default + periodic_variables_extra_lep(2) + periodic_variables_jet(2),
                                                model_basetype = SimpleModel,
                                                model_hyperparams = global_hyperparams,
                                                preprocessor_basetype = FlexiblePCAWhiteningPreprocessor)
        ep.add_component(ep_comp)
        qq_cat.add_endpiece(ep)


        # now make all combinations between those categories and add them to the model collections (do it manually now, automatized later)
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(VBF_cat, ggH_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(WHh_cat, ggH_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHh_cat, ggH_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(WHh_cat, ZHh_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(VBF_cat, WHh_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(VBF_cat, ZHh_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(WHl_cat, ggH_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(WHl_cat, VBF_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(WHl_cat, WHh_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(WHl_cat, ZHh_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(WHl_cat, ZHl_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(WHl_cat, ZHMET_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(WHl_cat, ttHh_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(WHl_cat, ttHl_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHh_cat, ZHl_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHh_cat, ZHMET_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHh_cat, ttHh_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHh_cat, ttHl_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHl_cat, ggH_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHl_cat, VBF_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHl_cat, WHh_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHl_cat, ZHMET_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHl_cat, ttHh_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHl_cat, ttHl_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHMET_cat, ggH_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHMET_cat, VBF_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHMET_cat, WHh_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHMET_cat, ttHh_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHMET_cat, ttHl_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ttHh_cat, ggH_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ttHh_cat, VBF_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ttHh_cat, WHh_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ttHh_cat, ttHl_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ttHl_cat, ggH_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ttHl_cat, VBF_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ttHl_cat, WHh_cat), input_config_file = input_config_file))

        # pairings of Z+X category with the signals
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ggH_cat, ZX_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(VBF_cat, ZX_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHh_cat, ZX_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHl_cat, ZX_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(WHh_cat, ZX_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(WHl_cat, ZX_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHMET_cat, ZX_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ttHh_cat, ZX_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ttHl_cat, ZX_cat), input_config_file = input_config_file))

        # pairings of qq category with the signals
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ggH_cat, qq_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(VBF_cat, qq_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHh_cat, qq_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHl_cat, qq_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(WHh_cat, qq_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(WHl_cat, qq_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZHMET_cat, qq_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ttHh_cat, qq_cat), input_config_file = input_config_file))
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ttHl_cat, qq_cat), input_config_file = input_config_file))

        # the two background categories among themselves
        mcolls.append(ModelCollection.from_discriminant_endpieces(*Category.match(ZX_cat, qq_cat), input_config_file = input_config_file))

        return mcolls
