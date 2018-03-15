#include <ZZAnalysis/AnalysisStep/test/classlib/include/MEDiscriminantFactory.h>

DiscriminantCollection* MEDiscriminantFactory::GenerateDiscriminantCollection(TString out_folder, Config& conf)
{
    // the cuts that will be needed
    auto j2cut = [&](Tree* in) -> bool {
    	return (((in -> nCleanedJetsPt30 >= 2) ? kTRUE : kFALSE)) ? 
    	kTRUE : kFALSE;};

    auto j1cut = [&](Tree* in) -> bool {
    	return (((in -> nCleanedJetsPt30 == 1) ? kTRUE : kFALSE)) ? 
    	kTRUE : kFALSE;};

    // need the raw MELA kinematic discriminants
    auto DVBF2j_ME_disc = [&](Tree* in) -> float{return DVBF2j_ME(
    	    in -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal, 
    	    in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, 
    	    in -> ZZMass);
    };

    auto DVBF1j_ME_disc = [&](Tree* in) -> float{return DVBF1j_ME(
    	    in -> p_JVBF_SIG_ghv1_1_JHUGen_JECNominal, 
    	    in -> pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
    	    in -> p_JQCD_SIG_ghg2_1_JHUGen_JECNominal,
    	    in -> ZZMass);
    };

    auto DWHh_ME_disc = [&](Tree* in) -> float{return DWHh_ME(
    	    in -> p_HadWH_SIG_ghw1_1_JHUGen_JECNominal,
    	    in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
    	    in -> p_HadWH_mavjj_JECNominal,
    	    in -> p_HadWH_mavjj_true_JECNominal,
    	    in -> ZZMass
    	    );
    };

    auto DZHh_ME_disc = [&](Tree* in) -> float{return DZHh_ME(
    	    in -> p_HadZH_SIG_ghz1_1_JHUGen_JECNominal,
    	    in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
    	    in -> p_HadZH_mavjj_JECNominal,
    	    in -> p_HadZH_mavjj_true_JECNominal,
    	    in -> ZZMass
    	    );
    };

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
    H0Stream -> AddEventSource(conf.MC_path() + H0_name + conf.MC_filename(), mZZ_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);

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
    H0Stream -> AddEventSource(conf.MC_path() + H0_name + conf.MC_filename(), mZZ_cut);

    disc -> SetH1Source(H1Stream);
    disc -> SetH0Source(H0Stream);

    // now set the components of this discriminant. for VBF, it needs to distinguish between events with 1 jet and events with 2 jets
    disc -> AddComponent("ZHhadr_ME", j2cut, DZHh_ME_disc);    

    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);

    // ----------------------------

    return coll;
}
