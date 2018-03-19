#include <ZZAnalysis/AnalysisStep/test/classlib/include/MEDiscriminantFactory.h>

DiscriminantCollection* MEDiscriminantFactory::GenerateDiscriminantCollection(TString out_folder, Config& conf)
{
    // for the weights:
    // float ggH_xs = 50.35;
    // float VBF_xs = 4.17;
    // float ZHhadr_xs = 0.88 * 0.7;
    // float WHhadr_xs = 1.50 * 0.67;

    float ggH_xs = 1.0;
    float VBF_xs = 1.0;
    float ZHhadr_xs = 1.0;
    float WHhadr_xs = 1.0;

    // the cuts that will be needed
    auto j2cut = [&](Tree* in) -> bool {
    	return (((in -> nCleanedJetsPt30 >= 2) ? kTRUE : kFALSE)) ? 
    	kTRUE : kFALSE;};

    auto j1cut = [&](Tree* in) -> bool {
    	return (((in -> nCleanedJetsPt30 == 1) ? kTRUE : kFALSE)) ? 
    	kTRUE : kFALSE;};

    DiscriminantCollection* coll = new DiscriminantCollection();
    Discriminant* disc;

    TString H1_name;
    TString H1_file_name;
    TString H0_name;
    TString H0_file_name;

    EventStream* H1Stream;
    EventStream* H0Stream;

    // ---------------------------

    disc = new Discriminant(out_folder);
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
    disc -> SetH1Weight(VBF_xs);
    disc -> SetH0Weight(ggH_xs);

    // now set the components of this discriminant. for VBF, it needs to distinguish between events with 1 jet and events with 2 jets
    disc -> AddComponent("VBF2j_ME", j2cut, DVBF2j_ME_disc);    
    disc -> AddComponent("VBF1j_ME", j1cut, DVBF1j_ME_disc);    

    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ----------------------------

    // ---------------------------

    disc = new Discriminant(out_folder);
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
    disc -> SetH1Weight(WHhadr_xs);
    disc -> SetH0Weight(ggH_xs);

    // now set the components of this discriminant. for VBF, it needs to distinguish between events with 1 jet and events with 2 jets
    disc -> AddComponent("WHhadr_ME", j2cut, DWHh_ME_disc);    

    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ----------------------------

    // ---------------------------

    disc = new Discriminant(out_folder);
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
    disc -> SetH1Weight(ZHhadr_xs);
    disc -> SetH0Weight(ggH_xs);

    // now set the components of this discriminant. for VBF, it needs to distinguish between events with 1 jet and events with 2 jets
    disc -> AddComponent("ZHhadr_ME", j2cut, DZHh_ME_disc);    

    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ----------------------------

    // ---------------------------

    disc = new Discriminant(out_folder);
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
    disc -> SetH1Weight(WHhadr_xs);
    disc -> SetH0Weight(ZHhadr_xs);

    // now set the components of this discriminant. require two jets such that both sides of the discriminant can be evaluated
    disc -> AddComponent("ZHhadrWHhadr_ME", j2cut, DWHZH_ME_disc);    

    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ----------------------------

    // ---------------------------

    disc = new Discriminant(out_folder);
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
    disc -> SetH1Weight(VBF_xs);
    disc -> SetH0Weight(WHhadr_xs);

    // now set the components of this discriminant. require two jets such that both sides of the discriminant can be evaluated
    disc -> AddComponent("VBFWHhadr_ME", j2cut, DVBFWH_ME_disc);    

    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ----------------------------

    // ---------------------------

    disc = new Discriminant(out_folder);
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
    disc -> SetH1Weight(VBF_xs);
    disc -> SetH0Weight(ZHhadr_xs);

    // now set the components of this discriminant. require two jets such that both sides of the discriminant can be evaluated
    disc -> AddComponent("VBFZHhadr_ME", j2cut, DVBFZH_ME_disc);    

    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ----------------------------

    return coll;
}
