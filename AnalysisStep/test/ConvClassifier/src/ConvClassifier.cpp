#include <ZZAnalysis/AnalysisStep/test/ConvClassifier/include/ConvClassifier.h>
#include <ZZAnalysis/AnalysisStep/interface/Category.h>
#include <ZZAnalysis/AnalysisStep/interface/Discriminants.h>

ConvClassifier::ConvClassifier():Tree()
{ }

ConvClassifier::~ConvClassifier()
{ }

// returns the number of events put into each category
void ConvClassifier::FillHistogram(TString input_file_name, float lumi, TH1F* hist)
{    
    input_file = new TFile(input_file_name);

    // read some auxiliary information
    hCounters = (TH1F*)input_file -> Get("ZZTree/Counters");
    n_gen_events = (Long64_t)hCounters -> GetBinContent(1);
    gen_sum_weights = (Long64_t)hCounters -> GetBinContent(40);

    input_tree = (TTree*)input_file -> Get("ZZTree/candTree");
    Init(input_tree, input_file_name);

    if(fChain == 0) return;

    // total number of entries stored in the entire chain
    Long64_t n_entries = fChain -> GetEntriesFast();
    std::cout << "total number of entries = " << n_entries << std::endl;

    // loop over the entries in chain
    for(Long64_t j_entry = 0; j_entry < n_entries; j_entry++)
    {
	// get the correct tree in the chain that contains this event
	Long64_t i_entry = LoadTree(j_entry);
	if(i_entry < 0) break;

	// now actually read this entry
	fChain -> GetEntry(j_entry);

	int result = ClassifyEvent();

	float event_weight = (lumi * xsec * 1000. * overallEventWeight) / gen_sum_weights;

	hist -> Fill(result, event_weight);
    }
}

int ConvClassifier::ClassifyEvent()
{
/*    float jetQGLikelihood[99];
    float jetPhi[99];

    for(int i = 0; i < nCleanedJetsPt30; i++)
    {
	jetQGLikelihood[i] = JetQGLikelihood -> at(i);
	jetPhi[i] = JetPhi -> at(i);
    }

    return categoryMor17(
	nExtraLep,
	nExtraZ,
	nCleanedJetsPt30, 
	nCleanedJetsPt30BTagged_bTagSF,
	jetQGLikelihood,
	p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
	p_JQCD_SIG_ghg2_1_JHUGen_JECNominal,
	p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal,
	p_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
	pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
	p_HadWH_SIG_ghw1_1_JHUGen_JECNominal,
	p_HadZH_SIG_ghz1_1_JHUGen_JECNominal,
	p_HadWH_mavjj_JECNominal,
	p_HadWH_mavjj_true_JECNominal,
	p_HadZH_mavjj_JECNominal,
	p_HadZH_mavjj_true_JECNominal,
	jetPhi,
	ZZMass,
	PFMET,
	true,
	false);
*/	

    // perform the classification:
    if(IsVBF2JetTagged())
    {
	return VBF2jTaggedMor17;
    }
    else if(IsVHHadronicTagged())
    {
	return VHHadrTaggedMor17;
    }
    else if(IsVHLeptonicTagged())
    {
	return VHLeptTaggedMor17;
    }
    else if(IsttHTagged())
    {
	return ttHTaggedMor17;
    }
    else if(IsVHEtMissTagged())
    {
	return VHMETTaggedMor17;
    }
    else if(IsVBF1JetTagged())
    {
	return VBF1jTaggedMor17;
    }
    else
    {
	return UntaggedMor17;
    }
}

int ConvClassifier::IsVBF2JetTagged()
{
    /* require:
     * -> exactly four leptons
     * -> either two OR three jets, at most one of which is B-tagged
     * -> OR four or more jets, none of which are B-tagged
     * D2_jet >= 0.5
     */
    
    float D2_jet = DVBF2j_ME(p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, ZZMass);

    float D2_jet_WP = 0.4894;

    if(nExtraLep == 0)
    {
	if(D2_jet >= D2_jet_WP)
	{
	    if((nCleanedJetsPt30 == 2) || (nCleanedJetsPt30 == 3))
	    {
		if(NumberBTaggedJets() <= 1)
		{
		    return(1);
		}
	    }
	    else if(nCleanedJetsPt30 >= 4)
	    {
		if(NumberBTaggedJets() == 0)
		{
		    return(1);
		}
	    }
	}
    }

    return(0);
}

int ConvClassifier::IsVHHadronicTagged()
{
    /* require:
     * -> exactly four leptons
     * -> either two OR three jets (no requirement on their B-Tagging
     * -> OR four or more jets, none of which are B-tagged
     * -> D_VH >= 0.5
     */

    float D_WH = DWHh_ME(p_HadWH_SIG_ghw1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, p_HadWH_mavjj_JECNominal, p_HadWH_mavjj_true_JECNominal, ZZMass);
    float D_ZH = DZHh_ME(p_HadZH_SIG_ghz1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, p_HadZH_mavjj_JECNominal, p_HadZH_mavjj_true_JECNominal, ZZMass);
    float D_VH = std::max(D_WH, D_ZH);

    float D_WH_WP = 0.52597;
    float D_ZH_WP = 0.52419;

    if(nExtraLep == 0)
    {
	if((D_WH > D_WH_WP) || (D_ZH_WP > D_ZH_WP))
	{
	    if((nCleanedJetsPt30 == 2) || (nCleanedJetsPt30 == 3))
	    {
		return(1);
	    }
	    else if(nCleanedJetsPt30 >= 4)
	    {
		if(NumberBTaggedJets() == 0)
		{
		    return(1);
		}
	    }
	}
    }

    return(0);
}

int ConvClassifier::IsVHLeptonicTagged()
{
    /* require:
     * -> no more than three jets
     * -> no B-tagged jets
     * -> exactly one additional lepton OR one additional pair of OS, same-flavour leptons
     *
     * -> OR exactly NO jet + at least one additional lepton
     */
    
    if((nCleanedJetsPt30 == 0) && (nExtraLep >= 1))
    {
	return(1);
    }
    else if((nCleanedJetsPt30 <= 3) && (NumberBTaggedJets() == 0))
    {
	// check for exactly one additional lepton OR exactly one additional pair of OS, same-flavour leptons
	if(nExtraLep == 1)
	{
	    return(1);
	}
	/*else if((nExtraLep == 2) && 
		( ((ExtraLepLepId -> at(0)) * (ExtraLepLepId -> at(1)) == -121) ||
		((ExtraLepLepId -> at(0)) * (ExtraLepLepId -> at(1)) == -169) ))*/
	else if(nExtraZ >= 1) // this is semantically the same! 
	{
	    return(1);
	}
    }

    return(0);
}

int ConvClassifier::IsttHTagged()
{
    /* require:
     * -> at least four jets, at least one of which is B-tagged
     * -> OR at least one additional lepton
     */

    if((nCleanedJetsPt30 >= 4) && (NumberBTaggedJets() >= 1))
    {
	return(1);
    }
    else
    {
	if(nExtraLep >= 1)
	{
	    return(1);
	}
    }

    return(0);
}

int ConvClassifier::IsVHEtMissTagged()
{
    /* require:
     * -> exactly four leptons
     * -> no more than one jet
     * -> EtMiss > 100 GeV
     */
    
    if((nExtraLep == 0) && (nCleanedJetsPt30 <= 1) && (PFMET >= 100))
    {
	return(1);
    }

    return(0);
}

int ConvClassifier::IsVBF1JetTagged()
{
    /* require:
     * -> exactly four leptons
     * -> exactly one jet
     * -> D1_jet >= 0.5
     */

    float D1_jet_WP = 0.37605;

    float D1_jet = DVBF1j_ME(p_JVBF_SIG_ghv1_1_JHUGen_JECNominal, pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal, p_JQCD_SIG_ghg2_1_JHUGen_JECNominal, ZZMass);

    if((nExtraLep == 0) && (nCleanedJetsPt30 == 1) && (D1_jet >= D1_jet_WP))
    {
	return(1);
    }

    return(0);
}

int ConvClassifier::NumberBTaggedJets()
{
/*
    int retval = 0;
    for(unsigned int jet = 0; jet < JetPt -> size(); jet++)
    {
	if(JetIsBtagged -> at(jet) > 0.5)
	{
	    retval += 1;
	}
    }

    return retval;
*/
    return nCleanedJetsPt30BTagged_bTagSF;
}
