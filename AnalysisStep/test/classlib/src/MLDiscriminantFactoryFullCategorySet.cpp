#include <ZZAnalysis/AnalysisStep/test/classlib/include/MLDiscriminantFactoryFullCategorySet.h>

DiscriminantCollection* MLDiscriminantFactoryFullCategorySet::GenerateDiscriminantCollection(TString outdir, Config& conf)
{
    DiscriminantCollection* coll = new DiscriminantCollection();
    Discriminant* disc;

    TString H1_name;
    TString H1_file_name;
    TString H0_name;
    TString H0_file_name;

    EventStream* H1Stream;
    EventStream* H0Stream;

    // the cuts that will be needed
    // TODO: get rid of this very weird construction
    auto j2cut = [&](Tree* in) -> bool {
    	return (((in -> nCleanedJetsPt30 >= 2) ? kTRUE : kFALSE)) ? 
    	kTRUE : kFALSE;};

    auto j1cut = [&](Tree* in) -> bool {
    	return (((in -> nCleanedJetsPt30 == 1) ? kTRUE : kFALSE)) ? 
    	kTRUE : kFALSE;};

    auto j0cut = [&](Tree* in) -> bool {
    	return (((in -> nCleanedJetsPt30 == 0) ? kTRUE : kFALSE)) ? 
    	kTRUE : kFALSE;};

    auto lj2cut = [&](Tree* in) -> bool {
    	return (((in -> nCleanedJetsPt30 < 2) ? kTRUE : kFALSE)) ? 
    	kTRUE : kFALSE;};

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "VBFH125";
    H0_name = "ggH125";
    
    H1Stream = new EventStream();
    H1_file_name = "VBFH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), mZZ_cut);

    H0Stream = new EventStream();
    H0_file_name = "ggH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), mZZ_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_VBF_ML_disc = [&](Tree* in) -> float {
	return in -> D_VBF_ggH_ML;
    };

    disc -> AddComponent("D_VBF_ggH_2j_ML", j2cut, D_VBF_ML_disc);
    disc -> AddComponent("D_VBF_ggH_1j_ML", j1cut, D_VBF_ML_disc);
    disc -> AddComponent("D_VBF_ggH_0j_ML", j0cut, D_VBF_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "WHhadr";
    H0_name = "ggH125";
    
    H1Stream = new EventStream();
    H1_file_name = "WplusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_0_cut);
    H1_file_name = "WminusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_0_cut);

    H0Stream = new EventStream();
    H0_file_name = "ggH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), mZZ_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_WHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_WHh_ggH_ML;
    };

    disc -> AddComponent("D_WHh_ggH_2j_ML", j2cut, D_WHh_ML_disc);
    disc -> AddComponent("D_WHh_ggH_01j_ML", lj2cut, D_WHh_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ZHhadr";
    H0_name = "ggH125";
    
    H1Stream = new EventStream();
    H1_file_name = "ZH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraNeutrinos_0_Leptons_0_cut);

    H0Stream = new EventStream();
    H0_file_name = "ggH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), mZZ_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ZHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_ZHh_ggH_ML;
    };

    disc -> AddComponent("D_ZHh_ggH_2j_ML", j2cut, D_ZHh_ML_disc);
    disc -> AddComponent("D_ZHh_ggH_01j_ML", lj2cut, D_ZHh_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "WHhadr";
    H0_name = "ZHhadr";
    
    H1Stream = new EventStream();
    H1_file_name = "WplusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_0_cut);
    H1_file_name = "WminusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_0_cut);

    H0Stream = new EventStream();
    H0_file_name = "ZH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraNeutrinos_0_Leptons_0_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_WHh_ZHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_WHh_ZHh_ML;
    };

    disc -> AddComponent("D_WHh_ZHh_2j_ML", j2cut, D_WHh_ZHh_ML_disc);
    disc -> AddComponent("D_WHh_ZHh_01j_ML", lj2cut, D_WHh_ZHh_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "VBFH125";
    H0_name = "WHhadr";
    
    H1Stream = new EventStream();
    H1_file_name = "VBFH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), mZZ_cut);

    H0Stream = new EventStream();
    H0_file_name = "WplusH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_0_cut);
    H0_file_name = "WminusH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_0_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_VBF_WHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_VBF_WHh_ML;
    };

    disc -> AddComponent("D_VBF_WHh_2j_ML", j2cut, D_VBF_WHh_ML_disc);
    disc -> AddComponent("D_VBF_WHh_01j_ML", lj2cut, D_VBF_WHh_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "VBFH125";
    H0_name = "ZHhadr";
    
    H1Stream = new EventStream();
    H1_file_name = "VBFH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), mZZ_cut);

    H0Stream = new EventStream();
    H0_file_name = "ZH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraNeutrinos_0_Leptons_0_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_VBF_ZHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_VBF_ZHh_ML;
    };

    disc -> AddComponent("D_VBF_ZHh_2j_ML", j2cut, D_VBF_ZHh_ML_disc);
    disc -> AddComponent("D_VBF_ZHh_01j_ML", lj2cut, D_VBF_ZHh_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "WHlept";
    H0_name = "ggH125";
    
    H1Stream = new EventStream();
    H1_file_name = "WplusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_1_cut);
    H1_file_name = "WminusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_1_cut);
    
    H0Stream = new EventStream();
    H0_file_name = "ggH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), mZZ_cut);    

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_WHl_ggH_ML_disc = [&](Tree* in) -> float {
	return in -> D_WHl_ggH_ML;
    };

    disc -> AddComponent("D_WHl_ggH_ML", no_cut, D_WHl_ggH_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "WHlept";
    H0_name = "VBFH125";
    
    H1Stream = new EventStream();
    H1_file_name = "WplusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_1_cut);
    H1_file_name = "WminusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_1_cut);
    
    H0Stream = new EventStream();
    H0_file_name = "VBFH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), mZZ_cut);    

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_WHl_VBF_ML_disc = [&](Tree* in) -> float {
	return in -> D_WHl_VBF_ML;
    };

    disc -> AddComponent("D_WHl_VBF_ML", no_cut, D_WHl_VBF_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "WHlept";
    H0_name = "WHhadr";
    
    H1Stream = new EventStream();
    H1_file_name = "WplusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_1_cut);
    H1_file_name = "WminusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_1_cut);
    
    H0Stream = new EventStream();
    H0_file_name = "WplusH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_0_cut);
    H0_file_name = "WminusH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_0_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_WHl_WHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_WHl_WHh_ML;
    };

    disc -> AddComponent("D_WHl_WHh_ML", no_cut, D_WHl_WHh_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "WHlept";
    H0_name = "ZHhadr";
    
    H1Stream = new EventStream();
    H1_file_name = "WplusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_1_cut);
    H1_file_name = "WminusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_1_cut);
    
    H0Stream = new EventStream();
    H0_file_name = "ZH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraNeutrinos_0_Leptons_0_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_WHl_ZHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_WHl_ZHh_ML;
    };

    disc -> AddComponent("D_WHl_ZHh_ML", no_cut, D_WHl_ZHh_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "WHlept";
    H0_name = "ZHlept";
    
    H1Stream = new EventStream();
    H1_file_name = "WplusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_1_cut);
    H1_file_name = "WminusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_1_cut);
    
    H0Stream = new EventStream();
    H0_file_name = "ZH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_2_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_WHl_ZHl_ML_disc = [&](Tree* in) -> float {
	return in -> D_WHl_ZHl_ML;
    };

    disc -> AddComponent("D_WHl_ZHl_ML", no_cut, D_WHl_ZHl_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "WHlept";
    H0_name = "ZHMET";
    
    H1Stream = new EventStream();
    H1_file_name = "WplusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_1_cut);
    H1_file_name = "WminusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_1_cut);
    
    H0Stream = new EventStream();
    H0_file_name = "ZH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraNeutrinos_2_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_WHl_ZHMET_ML_disc = [&](Tree* in) -> float {
	return in -> D_WHl_ZHMET_ML;
    };

    disc -> AddComponent("D_WHl_ZHMET_ML", no_cut, D_WHl_ZHMET_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "WHlept";
    H0_name = "ttHhadr";
    
    H1Stream = new EventStream();
    H1_file_name = "WplusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_1_cut);
    H1_file_name = "WminusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_1_cut);
    
    H0Stream = new EventStream();
    H0_file_name = "ttH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_0_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_WHl_ttHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_WHl_ttHh_ML;
    };

    disc -> AddComponent("D_WHl_ttHh_ML", no_cut, D_WHl_ttHh_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "WHlept";
    H0_name = "ttHlept";
    
    H1Stream = new EventStream();
    H1_file_name = "WplusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_1_cut);
    H1_file_name = "WminusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_1_cut);
    
    H0Stream = new EventStream();
    H0_file_name = "ttH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_12_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_WHl_ttHl_ML_disc = [&](Tree* in) -> float {
	return in -> D_WHl_ttHl_ML;
    };

    disc -> AddComponent("D_WHl_ttHl_ML", no_cut, D_WHl_ttHl_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ZHhadr";
    H0_name = "ZHlept";
    
    H1Stream = new EventStream();
    H1_file_name = "ZH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraNeutrinos_0_Leptons_0_cut);    

    H0Stream = new EventStream();
    H0_file_name = "ZH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_2_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ZHh_ZHl_ML_disc = [&](Tree* in) -> float {
	return in -> D_ZHh_ZHl_ML;
    };

    disc -> AddComponent("D_ZHh_ZHl_ML", no_cut, D_ZHh_ZHl_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ZHhadr";
    H0_name = "ZHMET";
    
    H1Stream = new EventStream();
    H1_file_name = "ZH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraNeutrinos_0_Leptons_0_cut);    

    H0Stream = new EventStream();
    H0_file_name = "ZH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraNeutrinos_2_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ZHh_ZHMET_ML_disc = [&](Tree* in) -> float {
	return in -> D_ZHh_ZHMET_ML;
    };

    disc -> AddComponent("D_ZHh_ZHMET_ML", no_cut, D_ZHh_ZHMET_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ZHhadr";
    H0_name = "ttHhadr";
    
    H1Stream = new EventStream();
    H1_file_name = "ZH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraNeutrinos_0_Leptons_0_cut);    

    H0Stream = new EventStream();
    H0_file_name = "ttH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_0_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ZHh_ttHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_ZHh_ttHh_ML;
    };

    disc -> AddComponent("D_ZHh_ttHh_ML", no_cut, D_ZHh_ttHh_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ZHhadr";
    H0_name = "ttHlept";
    
    H1Stream = new EventStream();
    H1_file_name = "ZH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraNeutrinos_0_Leptons_0_cut);    

    H0Stream = new EventStream();
    H0_file_name = "ttH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_12_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ZHh_ttHl_ML_disc = [&](Tree* in) -> float {
	return in -> D_ZHh_ttHl_ML;
    };

    disc -> AddComponent("D_ZHh_ttHl_ML", no_cut, D_ZHh_ttHl_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);
    

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ZHlept";
    H0_name = "ggH125";

    H1Stream = new EventStream();
    H1_file_name = "ZH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_2_cut);

    H0Stream = new EventStream();
    H0_file_name = "ggH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), mZZ_cut);        

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ZHl_ggH_ML_disc = [&](Tree* in) -> float {
	return in -> D_ZHl_ggH_ML;
    };

    disc -> AddComponent("D_ZHl_ggH_ML", no_cut, D_ZHl_ggH_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ZHlept";
    H0_name = "VBFH125";

    H1Stream = new EventStream();
    H1_file_name = "ZH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_2_cut);

    H0Stream = new EventStream();
    H0_file_name = "VBFH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), mZZ_cut);        

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ZHl_VBF_ML_disc = [&](Tree* in) -> float {
	return in -> D_ZHl_VBF_ML;
    };

    disc -> AddComponent("D_ZHl_VBF_ML", no_cut, D_ZHl_VBF_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ZHlept";
    H0_name = "WHhadr";

    H1Stream = new EventStream();
    H1_file_name = "ZH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_2_cut);

    H0Stream = new EventStream();
    H0_file_name = "WplusH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_0_cut);
    H0_file_name = "WminusH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_0_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ZHl_WHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_ZHl_WHh_ML;
    };

    disc -> AddComponent("D_ZHl_WHh_ML", no_cut, D_ZHl_WHh_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ZHlept";
    H0_name = "ZHMET";

    H1Stream = new EventStream();
    H1_file_name = "ZH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_2_cut);
    
    H0Stream = new EventStream();
    H0_file_name = "ZH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraNeutrinos_2_cut);    

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ZHl_ZHMET_ML_disc = [&](Tree* in) -> float {
	return in -> D_ZHl_ZHMET_ML;
    };

    disc -> AddComponent("D_ZHl_ZHMET_ML", no_cut, D_ZHl_ZHMET_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ZHlept";
    H0_name = "ttHhadr";

    H1Stream = new EventStream();
    H1_file_name = "ZH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_2_cut);

    H0Stream = new EventStream();
    H0_file_name = "ttH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_0_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ZHl_ttHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_ZHl_ttHh_ML;
    };

    disc -> AddComponent("D_ZHl_ttHh_ML", no_cut, D_ZHl_ttHh_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ZHlept";
    H0_name = "ttHlept";

    H1Stream = new EventStream();
    H1_file_name = "ZH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_2_cut);

    H0Stream = new EventStream();
    H0_file_name = "ttH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_12_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ZHl_ttHl_ML_disc = [&](Tree* in) -> float {
	return in -> D_ZHl_ttHl_ML;
    };

    disc -> AddComponent("D_ZHl_ttHl_ML", no_cut, D_ZHl_ttHl_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ZHMET";
    H0_name = "ggH125";

    H1Stream = new EventStream();
    H1_file_name = "ZH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraNeutrinos_2_cut);    

    H0Stream = new EventStream();
    H0_file_name = "ggH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), mZZ_cut);        
    
    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ZHMET_ggH_ML_disc = [&](Tree* in) -> float {
	return in -> D_ZHMET_ggH_ML;
    };

    disc -> AddComponent("D_ZHMET_ggH_ML", no_cut, D_ZHMET_ggH_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ZHMET";
    H0_name = "VBFH125";

    H1Stream = new EventStream();
    H1_file_name = "ZH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraNeutrinos_2_cut);    

    H0Stream = new EventStream();
    H0_file_name = "VBFH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), mZZ_cut);        
    
    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ZHMET_VBF_ML_disc = [&](Tree* in) -> float {
	return in -> D_ZHMET_VBF_ML;
    };

    disc -> AddComponent("D_ZHMET_VBF_ML", no_cut, D_ZHMET_VBF_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ZHMET";
    H0_name = "WHhadr";

    H1Stream = new EventStream();
    H1_file_name = "ZH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraNeutrinos_2_cut);    

    H0Stream = new EventStream();
    H0_file_name = "WplusH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_0_cut);
    H0_file_name = "WminusH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_0_cut);
    
    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ZHMET_WHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_ZHMET_WHh_ML;
    };

    disc -> AddComponent("D_ZHMET_WHh_ML", no_cut, D_ZHMET_WHh_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ZHMET";
    H0_name = "ttHhadr";

    H1Stream = new EventStream();
    H1_file_name = "ZH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraNeutrinos_2_cut);    

    H0Stream = new EventStream();
    H0_file_name = "ttH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_0_cut);
    
    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ZHMET_ttHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_ZHMET_ttHh_ML;
    };

    disc -> AddComponent("D_ZHMET_ttHh_ML", no_cut, D_ZHMET_ttHh_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ZHMET";
    H0_name = "ttHlept";

    H1Stream = new EventStream();
    H1_file_name = "ZH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraNeutrinos_2_cut);    

    H0Stream = new EventStream();
    H0_file_name = "ttH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_12_cut);
    
    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ZHMET_ttHl_ML_disc = [&](Tree* in) -> float {
	return in -> D_ZHMET_ttHl_ML;
    };

    disc -> AddComponent("D_ZHMET_ttHl_ML", no_cut, D_ZHMET_ttHl_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ttHhadr";
    H0_name = "ggH125";

    H1Stream = new EventStream();
    H1_file_name = "ttH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_0_cut);

    H0Stream = new EventStream();
    H0_file_name = "ggH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), mZZ_cut);        
    
    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ttHh_ggH_ML_disc = [&](Tree* in) -> float {
	return in -> D_ttHh_ggH_ML;
    };

    disc -> AddComponent("D_ttHh_ggH_ML", no_cut, D_ttHh_ggH_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ttHhadr";
    H0_name = "VBFH125";

    H1Stream = new EventStream();
    H1_file_name = "ttH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_0_cut);

    H0Stream = new EventStream();
    H0_file_name = "VBFH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), mZZ_cut);        
    
    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ttHh_VBF_ML_disc = [&](Tree* in) -> float {
	return in -> D_ttHh_VBF_ML;
    };

    disc -> AddComponent("D_ttHh_VBF_ML", no_cut, D_ttHh_VBF_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ttHhadr";
    H0_name = "WHhadr";

    H1Stream = new EventStream();
    H1_file_name = "ttH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_0_cut);

    H0Stream = new EventStream();
    H0_file_name = "WplusH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_0_cut);
    H0_file_name = "WminusH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_0_cut);
    
    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ttHh_WHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_ttHh_WHh_ML;
    };

    disc -> AddComponent("D_ttHh_WHh_ML", no_cut, D_ttHh_WHh_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ttHhadr";
    H0_name = "ttHlept";

    H1Stream = new EventStream();
    H1_file_name = "ttH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_0_cut);

    H0Stream = new EventStream();
    H0_file_name = "ttH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_12_cut);
    
    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ttHh_ttHl_ML_disc = [&](Tree* in) -> float {
	return in -> D_ttHh_ttHl_ML;
    };

    disc -> AddComponent("D_ttHh_ttHl_ML", no_cut, D_ttHh_ttHl_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ttHlept";
    H0_name = "ggH125";

    H1Stream = new EventStream();
    H1_file_name = "ttH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_12_cut);

    H0Stream = new EventStream();
    H0_file_name = "ggH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), mZZ_cut);        
    
    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ttHl_ggH_ML_disc = [&](Tree* in) -> float {
	return in -> D_ttHl_ggH_ML;
    };

    disc -> AddComponent("D_ttHl_ggH_ML", no_cut, D_ttHl_ggH_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ttHlept";
    H0_name = "VBFH125";

    H1Stream = new EventStream();
    H1_file_name = "ttH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_12_cut);

    H0Stream = new EventStream();
    H0_file_name = "VBFH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), mZZ_cut);        
    
    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ttHl_VBF_ML_disc = [&](Tree* in) -> float {
	return in -> D_ttHl_VBF_ML;
    };

    disc -> AddComponent("D_ttHl_VBF_ML", no_cut, D_ttHl_VBF_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ttHlept";
    H0_name = "WHhadr";

    H1Stream = new EventStream();
    H1_file_name = "ttH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), extraLeptons_12_cut);

    H0Stream = new EventStream();
    H0_file_name = "WplusH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_0_cut);
    H0_file_name = "WminusH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), extraLeptons_0_cut);
    
    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ttHl_WHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_ttHl_WHh_ML;
    };

    disc -> AddComponent("D_ttHl_WHh_ML", no_cut, D_ttHl_WHh_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    return coll;

}
