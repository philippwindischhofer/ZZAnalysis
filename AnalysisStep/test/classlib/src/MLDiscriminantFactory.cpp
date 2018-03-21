#include <ZZAnalysis/AnalysisStep/test/classlib/include/MLDiscriminantFactory.h>

DiscriminantCollection* MLDiscriminantFactory::GenerateDiscriminantCollection(Config& conf)
{
    DiscriminantCollection* coll = new DiscriminantCollection();
    Discriminant* disc;

    // the cuts that will be needed
    auto j2cut = [&](Tree* in) -> bool {
    	return (((in -> nCleanedJetsPt30 >= 2 && mZZ_cut(in)) ? kTRUE : kFALSE)) ? 
    	kTRUE : kFALSE;};

    TString H1_name;
    TString H1_file_name;
    TString H0_name;
    TString H0_file_name;

    EventStream* H1Stream;
    EventStream* H0Stream;

    // ---------------------------    

    disc = new Discriminant("no_dir");
    H1_name = "VBF2jH125";
    H0_name = "ggH125";
    
    H1Stream = new EventStream();
    H1_file_name = "VBFH125";

    H1Stream -> AddEventSource(conf.MC_path() + H1_file_name + conf.MC_filename(), j2cut);

    H0Stream = new EventStream();
    H0_file_name = "ggH125";
    H0Stream -> AddEventSource(conf.MC_path() + H0_file_name + conf.MC_filename(), j2cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);
    
    auto D_VBF_ML_disc = [&](Tree* in) -> float {
	return in -> VBF_ggH;
    };

    disc -> AddComponent("VBF2j", no_cut, D_VBF_ML_disc);
    
    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    return coll;
}
