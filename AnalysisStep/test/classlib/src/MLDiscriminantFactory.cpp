#include <ZZAnalysis/AnalysisStep/test/classlib/include/MLDiscriminantFactory.h>

DiscriminantCollection* MLDiscriminantFactory::GenerateDiscriminantCollection(TString outdir, Config& conf)
{
    DiscriminantCollection* coll = new DiscriminantCollection();
    Discriminant* disc;

    // the cuts that will be needed
    auto j2cut = [&](Tree* in) -> bool {
    	return (((in -> nCleanedJetsPt30 >= 2 && mZZ_cut(in)) ? kTRUE : kFALSE)) ? 
    	kTRUE : kFALSE;};

    auto j1cut = [&](Tree* in) -> bool {
    	return (((in -> nCleanedJetsPt30 < 2 && mZZ_cut(in)) ? kTRUE : kFALSE)) ? 
    	kTRUE : kFALSE;};

    TString H1_name;
    TString H1_file_name;
    TString H0_name;
    TString H0_file_name;

    EventStream* H1Stream;
    EventStream* H0Stream;

    // ---------------------------    

    disc = new Discriminant(outdir);
    H1_name = "VBFH125";
    H0_name = "ggH125";
    
    // note: if want to use this discriminant for ROCs, MUST put here the cut that lets through ONLY 2j-events!
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
