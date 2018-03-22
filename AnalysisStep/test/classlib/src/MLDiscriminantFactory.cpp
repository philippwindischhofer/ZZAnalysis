#include <ZZAnalysis/AnalysisStep/test/classlib/include/MLDiscriminantFactory.h>

// here, define the EventStreams such that they build the ROC curves for each Discriminant component exactly on the stream on which they were trained (although now taken from the validation region)
DiscriminantCollection* MLDiscriminantFactory::GenerateMorCompatibleDiscriminantCollection(TString outdir, Config& conf)
{
    DiscriminantCollection* coll = new DiscriminantCollection();
    Discriminant* disc;

    // the cuts that will be needed
    auto j2cut = [&](Tree* in) -> bool {
    	return ((in -> nCleanedJetsPt30 >= 2 && mZZ_cut(in)) ? kTRUE : kFALSE);
    };

    // the cuts that will be needed
    auto j1cut = [&](Tree* in) -> bool {
    	return ((in -> nCleanedJetsPt30 == 1 && mZZ_cut(in)) ? kTRUE : kFALSE);
    };

    auto j2WHhadr_cut = [&](Tree* in) -> bool {
	return (j2cut(in) && extraLeptons_0_cut(in));
    };

    auto j2ZHhadr_cut = [&](Tree* in) -> bool {
	return (j2cut(in) && extraNeutrinos_0_Leptons_0_cut(in));
    };

    TString H1_name;
    TString H1_file_name;
    TString H0_name;
    TString H0_file_name;

    EventStream* H1Stream;
    EventStream* H0Stream;

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "VBF2j";
    H0_name = "ggH";
    
    H1Stream = new EventStream();
    H1_file_name = "VBFH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), j2cut);

    H0Stream = new EventStream();
    H0_file_name = "ggH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), j2cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_VBF_ML_disc = [&](Tree* in) -> float {
	return in -> D_VBF_ggH_ML;
    };

    disc -> AddComponent("D_VBF_ggH_2j_ML", no_cut, D_VBF_ML_disc);
    disc -> SetDiscriminantName("D_{VBF2j/ggH}^{ML}");

    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "VBF1j";
    H0_name = "ggH";
    
    H1Stream = new EventStream();
    H1_file_name = "VBFH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), j1cut);

    H0Stream = new EventStream();
    H0_file_name = "ggH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), j1cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    disc -> AddComponent("D_VBF_ggH_1j_ML", no_cut, D_VBF_ML_disc);
    disc -> SetDiscriminantName("D_{VBF1j/ggH}^{ML}");

    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // -------------------------

    disc = new Discriminant(outdir);
    H1_name = "WH-hadr";
    H0_name = "ggH";
    
    H1Stream = new EventStream();
    H1_file_name = "WplusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), j2WHhadr_cut);
    H1_file_name = "WminusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), j2WHhadr_cut);

    H0Stream = new EventStream();
    H0_file_name = "ggH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), j2cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_WHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_WHh_ggH_ML;
    };

    disc -> AddComponent("D_WHh_ggH_ML", no_cut, D_WHh_ML_disc);
    disc -> SetDiscriminantName("D_{WH-hadr/ggH}^{ML}");
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "ZH-hadr";
    H0_name = "ggH";
    
    H1Stream = new EventStream();
    H1_file_name = "ZH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), j2ZHhadr_cut);

    H0Stream = new EventStream();
    H0_file_name = "ggH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), j2cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_ZHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_ZHh_ggH_ML;
    };

    disc -> AddComponent("D_ZHh_ggH_ML", no_cut, D_ZHh_ML_disc);
    disc -> SetDiscriminantName("D_{ZH-hadr/ggH}^{ML}");
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "WH-hadr";
    H0_name = "ZH-hadr";
    
    H1Stream = new EventStream();
    H1_file_name = "WplusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), j2WHhadr_cut);
    H1_file_name = "WminusH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), j2WHhadr_cut);

    H0Stream = new EventStream();
    H0_file_name = "ZH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), j2ZHhadr_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_WHh_ZHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_WHh_ZHh_ML;
    };

    disc -> AddComponent("D_WHh_ZHh_ML", no_cut, D_WHh_ZHh_ML_disc);
    disc -> SetDiscriminantName("D_{WH-hadr/ZH-hadr}^{ML}");
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "VBF2j";
    H0_name = "WH-hadr";
    
    H1Stream = new EventStream();
    H1_file_name = "VBFH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), j2cut);

    H0Stream = new EventStream();
    H0_file_name = "WplusH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), j2WHhadr_cut);
    H0_file_name = "WminusH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), j2WHhadr_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_VBF_WHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_VBF_WHh_ML;
    };

    disc -> AddComponent("D_VBF_WHh_ML", no_cut, D_VBF_WHh_ML_disc);
    disc -> SetDiscriminantName("D_{VBF2j/WH-hadr}^{ML}");
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "VBF2j";
    H0_name = "ZH-hadr";
    
    H1Stream = new EventStream();
    H1_file_name = "VBFH125";
    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), j2cut);

    H0Stream = new EventStream();
    H0_file_name = "ZH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), j2ZHhadr_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_VBF_ZHh_ML_disc = [&](Tree* in) -> float {
	return in -> D_VBF_ZHh_ML;
    };

    disc -> AddComponent("D_VBF_ZHh_ML", no_cut, D_VBF_ZHh_ML_disc);
    disc -> SetDiscriminantName("D_{VBF2j/ZH-hadr}^{ML}");
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    return coll;
}


DiscriminantCollection* MLDiscriminantFactory::GenerateDiscriminantCollection(TString outdir, Config& conf)
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
    auto j2cut = [&](Tree* in) -> bool {
    	return (((in -> nCleanedJetsPt30 >= 2) ? kTRUE : kFALSE)) ? 
    	kTRUE : kFALSE;};

    auto j1cut = [&](Tree* in) -> bool {
    	return (((in -> nCleanedJetsPt30 == 1) ? kTRUE : kFALSE)) ? 
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

    disc -> AddComponent("D_VBF2j_ggH_ML", j2cut, D_VBF_ML_disc);
    disc -> AddComponent("D_VBF1j_ggH_ML", j1cut, D_VBF_ML_disc);
    
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

    disc -> AddComponent("D_WHh_ggH_ML", j2cut, D_WHh_ML_disc);
    
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

    disc -> AddComponent("D_ZHh_ggH_ML", j2cut, D_ZHh_ML_disc);
    
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

    disc -> AddComponent("D_WHh_ZHh_ML", j2cut, D_WHh_ZHh_ML_disc);
    
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

    disc -> AddComponent("D_VBF_WHh_ML", j2cut, D_VBF_WHh_ML_disc);
    
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

    disc -> AddComponent("D_VBF_ZHh_ML", j2cut, D_VBF_ZHh_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    return coll;
}
