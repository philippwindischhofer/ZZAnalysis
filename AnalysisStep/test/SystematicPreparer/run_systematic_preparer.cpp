// C++
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/range/combine.hpp>

// ROOT
#include "TApplication.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TRandom3.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/me_discriminants.h>

float clamp_value(float val, float min, float max)
{
    if(val > max)
	return max;

    if(val < min)
	return min;

    return val;
}

void prepare_systematics(TString inpath, TString outpath, TString mode)
{
    TFile* input_file = new TFile(inpath, "read");
    TString tree_name = "ClassTree";
    
    // read the metadata
    TH1F* hCounters = (TH1F*)input_file -> Get(tree_name + "/Counters");
    Long64_t n_gen_events = (Long64_t)hCounters -> GetBinContent(1);
    Long64_t gen_sum_weights = (Long64_t)hCounters -> GetBinContent(40);

    TFile* output_file = new TFile(outpath, "recreate");
    output_file -> mkdir(tree_name);

    // read some auxiliary information
    TH1F* input_metadata = (TH1F*)input_file -> Get(tree_name + "/Counters");
    TTree* input_tree = (TTree*)input_file -> Get(tree_name + "/candTree");

    Tree* buffer = new Tree();
    buffer -> Init(input_tree, inpath);
    Long64_t n_entries = buffer -> fChain -> GetEntriesFast();

    TTree* output_tree = (TTree*)(input_tree -> CloneTree(0));
    TH1F* output_metadata = (TH1F*)(input_metadata -> Clone());
    
    // loop over the entries in chain
    for(Long64_t j_entry = 0; j_entry < n_entries; j_entry++)
    {
	// get the correct tree in the chain that contains this event
	Long64_t i_entry = buffer -> LoadTree(j_entry);
	if(i_entry < 0) break;
	
	// now actually read this entry
	buffer -> fChain -> GetEntry(j_entry);

	// depending on the mode, now swap the shifted entries (shifted with JEC up/down or BTAG up /down) into the main positions
	// such that systematics variations are transparent to all classifiers that run on this file later on
	if(mode == "JEC_UP")
	{
	    // scale the jet pt up here
	    for(unsigned int j = 0; j < buffer -> JetPt -> size(); j++)
	    {
		buffer -> JetPt -> at(j) = buffer -> JetJERUp -> at(j);
	    }

	    buffer -> nCleanedJetsPt30 = buffer -> nCleanedJetsPt30_jecUp;
	    buffer -> nCleanedJetsPt30BTagged_bTagSF = buffer -> nCleanedJetsPt30BTagged_bTagSF_jecUp;
	    buffer -> PFMET = buffer -> PFMET_jesUp;

	    buffer -> pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal = buffer -> pAux_JVBF_SIG_ghv1_1_JHUGen_JECUp;
            buffer -> p_JVBF_SIG_ghv1_1_JHUGen_JECNominal = buffer -> p_JVBF_SIG_ghv1_1_JHUGen_JECUp;
            buffer -> p_JQCD_SIG_ghg2_1_JHUGen_JECNominal = buffer -> p_JQCD_SIG_ghg2_1_JHUGen_JECUp;
            buffer -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1_1_JHUGen_JECUp;
            buffer -> p_JJVBF_SIG_ghv1prime2_1E4_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1prime2_1E4_JHUGen_JECUp;
            buffer -> p_JJVBF_SIG_ghv2_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv2_1_JHUGen_JECUp;
            buffer -> p_JJVBF_SIG_ghv4_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv4_1_JHUGen_JECUp;
            buffer -> p_JJVBF_SIG_ghza1prime2_1E4_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghza1prime2_1E4_JHUGen_JECUp;
            buffer -> p_JJVBF_SIG_ghza2_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghza2_1_JHUGen_JECUp;
            buffer -> p_JJVBF_SIG_ghza4_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghza4_1_JHUGen_JECUp;
            buffer -> p_JJVBF_SIG_gha2_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_gha2_1_JHUGen_JECUp;
            buffer -> p_JJVBF_SIG_gha4_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_gha4_1_JHUGen_JECUp;
            buffer -> p_JJVBF_SIG_ghv1_1_ghv1prime2_1E4_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1_1_ghv1prime2_1E4_JHUGen_JECUp;
            buffer -> p_JJVBF_SIG_ghv1_1_ghv2_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1_1_ghv2_1_JHUGen_JECUp;
            buffer -> p_JJVBF_SIG_ghv1_1_ghv4_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1_1_ghv4_1_JHUGen_JECUp;
            buffer -> p_JJVBF_SIG_ghv1_1_ghza1prime2_1E4_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1_1_ghza1prime2_1E4_JHUGen_JECUp;
            buffer -> p_JJVBF_SIG_ghv1_1_ghza2_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1_1_ghza2_1_JHUGen_JECUp;
            buffer -> p_JJVBF_SIG_ghv1_1_ghza4_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1_1_ghza4_1_JHUGen_JECUp;
            buffer -> p_JJVBF_SIG_ghv1_1_gha2_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1_1_gha2_1_JHUGen_JECUp;
            buffer -> p_JJVBF_SIG_ghv1_1_gha4_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1_1_gha4_1_JHUGen_JECUp;
            buffer -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal = buffer -> p_JJQCD_SIG_ghg2_1_JHUGen_JECUp;
            buffer -> p_JJQCD_SIG_ghg4_1_JHUGen_JECNominal = buffer -> p_JJQCD_SIG_ghg4_1_JHUGen_JECUp;
            buffer -> p_JJQCD_SIG_ghg2_1_ghg4_1_JHUGen_JECNominal = buffer -> p_JJQCD_SIG_ghg2_1_ghg4_1_JHUGen_JECUp;
            buffer -> p_HadZH_SIG_ghz1_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1_1_JHUGen_JECUp;
            buffer -> p_HadZH_SIG_ghz1prime2_1E4_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1prime2_1E4_JHUGen_JECUp;
            buffer -> p_HadZH_SIG_ghz2_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz2_1_JHUGen_JECUp;
            buffer -> p_HadZH_SIG_ghz4_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz4_1_JHUGen_JECUp;
            buffer -> p_HadZH_SIG_ghza1prime2_1E4_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghza1prime2_1E4_JHUGen_JECUp;
            buffer -> p_HadZH_SIG_ghza2_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghza2_1_JHUGen_JECUp;
            buffer -> p_HadZH_SIG_ghza4_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghza4_1_JHUGen_JECUp;
            buffer -> p_HadZH_SIG_gha2_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_gha2_1_JHUGen_JECUp;
            buffer -> p_HadZH_SIG_gha4_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_gha4_1_JHUGen_JECUp;
            buffer -> p_HadZH_SIG_ghz1_1_ghz1prime2_1E4_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1_1_ghz1prime2_1E4_JHUGen_JECUp;
            buffer -> p_HadZH_SIG_ghz1_1_ghz2_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1_1_ghz2_1_JHUGen_JECUp;
            buffer -> p_HadZH_SIG_ghz1_1_ghz4_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1_1_ghz4_1_JHUGen_JECUp;
            buffer -> p_HadZH_SIG_ghz1_1_ghza1prime2_1E4_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1_1_ghza1prime2_1E4_JHUGen_JECUp;
            buffer -> p_HadZH_SIG_ghz1_1_ghza2_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1_1_ghza2_1_JHUGen_JECUp;
            buffer -> p_HadZH_SIG_ghz1_1_ghza4_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1_1_ghza4_1_JHUGen_JECUp;
            buffer -> p_HadZH_SIG_ghz1_1_gha2_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1_1_gha2_1_JHUGen_JECUp;
            buffer -> p_HadZH_SIG_ghz1_1_gha4_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1_1_gha4_1_JHUGen_JECUp;
            buffer -> p_HadWH_SIG_ghw1_1_JHUGen_JECNominal = buffer -> p_HadWH_SIG_ghw1_1_JHUGen_JECUp;
            buffer -> p_HadWH_SIG_ghw1prime2_1E4_JHUGen_JECNominal = buffer -> p_HadWH_SIG_ghw1prime2_1E4_JHUGen_JECUp;
            buffer -> p_HadWH_SIG_ghw2_1_JHUGen_JECNominal = buffer -> p_HadWH_SIG_ghw2_1_JHUGen_JECUp;
            buffer -> p_HadWH_SIG_ghw4_1_JHUGen_JECNominal = buffer -> p_HadWH_SIG_ghw4_1_JHUGen_JECUp;
            buffer -> p_HadWH_SIG_ghw1_1_ghw1prime2_1E4_JHUGen_JECNominal = buffer -> p_HadWH_SIG_ghw1_1_ghw1prime2_1E4_JHUGen_JECUp;
            buffer -> p_HadWH_SIG_ghw1_1_ghw2_1_JHUGen_JECNominal = buffer -> p_HadWH_SIG_ghw1_1_ghw2_1_JHUGen_JECUp;
            buffer -> p_HadWH_SIG_ghw1_1_ghw4_1_JHUGen_JECNominal = buffer -> p_HadWH_SIG_ghw1_1_ghw4_1_JHUGen_JECUp;
            buffer -> p_ttHUndecayed_SIG_kappa_1_JHUGen_JECNominal = buffer -> p_ttHUndecayed_SIG_kappa_1_JHUGen_JECUp;
            buffer -> p_ttHUndecayed_SIG_kappatilde_1_JHUGen_JECNominal = buffer -> p_ttHUndecayed_SIG_kappatilde_1_JHUGen_JECUp;
            buffer -> p_ttHUndecayed_SIG_kappa_1_kappatilde_1_JHUGen_JECNominal = buffer -> p_ttHUndecayed_SIG_kappa_1_kappatilde_1_JHUGen_JECUp;
            buffer -> p_bbH_SIG_kappa_1_JHUGen_JECNominal = buffer -> p_bbH_SIG_kappa_1_JHUGen_JECUp;
            buffer -> p_HadZH_mavjj_JECNominal = buffer -> p_HadZH_mavjj_JECUp;
            buffer -> p_HadZH_mavjj_true_JECNominal = buffer -> p_HadZH_mavjj_true_JECUp;
            buffer -> p_HadWH_mavjj_JECNominal = buffer -> p_HadWH_mavjj_JECUp;
            buffer -> p_HadWH_mavjj_true_JECNominal = buffer -> p_HadWH_mavjj_true_JECUp;
            buffer -> p_JJEW_SIG_ghv1_1_MCFM_JECNominal = buffer -> p_JJEW_SIG_ghv1_1_MCFM_JECUp;
            buffer -> p_JJEW_BSI_ghv1_1_MCFM_JECNominal = buffer -> p_JJEW_BSI_ghv1_1_MCFM_JECUp;
            buffer -> p_JJEW_BSI_ghv1_i_MCFM_JECNominal = buffer -> p_JJEW_BSI_ghv1_i_MCFM_JECUp;
            buffer -> p_JJEW_BKG_MCFM_JECNominal = buffer -> p_JJEW_BKG_MCFM_JECUp;
            buffer -> p_JJVBF_BKG_MCFM_JECNominal = buffer -> p_JJVBF_BKG_MCFM_JECUp;
            buffer -> p_JJQCD_BKG_MCFM_JECNominal = buffer -> p_JJQCD_BKG_MCFM_JECUp;
            buffer -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal_BestDJJ = buffer -> p_JJVBF_SIG_ghv1_1_JHUGen_JECUp_BestDJJ;
            buffer -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal_BestDJJ = buffer -> p_JJQCD_SIG_ghg2_1_JHUGen_JECUp_BestDJJ;
	}
	else if(mode == "JEC_DN")
	{
            // scale the jet pt down here
	    for(unsigned int j = 0; j < buffer -> JetPt -> size(); j++)
	    {
		buffer -> JetPt -> at(j) = buffer -> JetJERDown -> at(j);
	    }

	    buffer -> nCleanedJetsPt30 = buffer -> nCleanedJetsPt30_jecDn;
	    buffer -> nCleanedJetsPt30BTagged_bTagSF = buffer -> nCleanedJetsPt30BTagged_bTagSF_jecDn;
	    buffer -> PFMET = buffer -> PFMET_jesDn;

	    buffer -> pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal = buffer -> pAux_JVBF_SIG_ghv1_1_JHUGen_JECDn;
            buffer -> p_JVBF_SIG_ghv1_1_JHUGen_JECNominal = buffer -> p_JVBF_SIG_ghv1_1_JHUGen_JECDn;
            buffer -> p_JQCD_SIG_ghg2_1_JHUGen_JECNominal = buffer -> p_JQCD_SIG_ghg2_1_JHUGen_JECDn;
            buffer -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1_1_JHUGen_JECDn;
            buffer -> p_JJVBF_SIG_ghv1prime2_1E4_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1prime2_1E4_JHUGen_JECDn;
            buffer -> p_JJVBF_SIG_ghv2_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv2_1_JHUGen_JECDn;
            buffer -> p_JJVBF_SIG_ghv4_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv4_1_JHUGen_JECDn;
            buffer -> p_JJVBF_SIG_ghza1prime2_1E4_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghza1prime2_1E4_JHUGen_JECDn;
            buffer -> p_JJVBF_SIG_ghza2_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghza2_1_JHUGen_JECDn;
            buffer -> p_JJVBF_SIG_ghza4_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghza4_1_JHUGen_JECDn;
            buffer -> p_JJVBF_SIG_gha2_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_gha2_1_JHUGen_JECDn;
            buffer -> p_JJVBF_SIG_gha4_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_gha4_1_JHUGen_JECDn;
            buffer -> p_JJVBF_SIG_ghv1_1_ghv1prime2_1E4_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1_1_ghv1prime2_1E4_JHUGen_JECDn;
            buffer -> p_JJVBF_SIG_ghv1_1_ghv2_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1_1_ghv2_1_JHUGen_JECDn;
            buffer -> p_JJVBF_SIG_ghv1_1_ghv4_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1_1_ghv4_1_JHUGen_JECDn;
            buffer -> p_JJVBF_SIG_ghv1_1_ghza1prime2_1E4_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1_1_ghza1prime2_1E4_JHUGen_JECDn;
            buffer -> p_JJVBF_SIG_ghv1_1_ghza2_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1_1_ghza2_1_JHUGen_JECDn;
            buffer -> p_JJVBF_SIG_ghv1_1_ghza4_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1_1_ghza4_1_JHUGen_JECDn;
            buffer -> p_JJVBF_SIG_ghv1_1_gha2_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1_1_gha2_1_JHUGen_JECDn;
            buffer -> p_JJVBF_SIG_ghv1_1_gha4_1_JHUGen_JECNominal = buffer -> p_JJVBF_SIG_ghv1_1_gha4_1_JHUGen_JECDn;
            buffer -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal = buffer -> p_JJQCD_SIG_ghg2_1_JHUGen_JECDn;
            buffer -> p_JJQCD_SIG_ghg4_1_JHUGen_JECNominal = buffer -> p_JJQCD_SIG_ghg4_1_JHUGen_JECDn;
            buffer -> p_JJQCD_SIG_ghg2_1_ghg4_1_JHUGen_JECNominal = buffer -> p_JJQCD_SIG_ghg2_1_ghg4_1_JHUGen_JECDn;
            buffer -> p_HadZH_SIG_ghz1_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1_1_JHUGen_JECDn;
            buffer -> p_HadZH_SIG_ghz1prime2_1E4_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1prime2_1E4_JHUGen_JECDn;
            buffer -> p_HadZH_SIG_ghz2_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz2_1_JHUGen_JECDn;
            buffer -> p_HadZH_SIG_ghz4_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz4_1_JHUGen_JECDn;
            buffer -> p_HadZH_SIG_ghza1prime2_1E4_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghza1prime2_1E4_JHUGen_JECDn;
            buffer -> p_HadZH_SIG_ghza2_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghza2_1_JHUGen_JECDn;
            buffer -> p_HadZH_SIG_ghza4_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghza4_1_JHUGen_JECDn;
            buffer -> p_HadZH_SIG_gha2_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_gha2_1_JHUGen_JECDn;
            buffer -> p_HadZH_SIG_gha4_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_gha4_1_JHUGen_JECDn;
            buffer -> p_HadZH_SIG_ghz1_1_ghz1prime2_1E4_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1_1_ghz1prime2_1E4_JHUGen_JECDn;
            buffer -> p_HadZH_SIG_ghz1_1_ghz2_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1_1_ghz2_1_JHUGen_JECDn;
            buffer -> p_HadZH_SIG_ghz1_1_ghz4_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1_1_ghz4_1_JHUGen_JECDn;
            buffer -> p_HadZH_SIG_ghz1_1_ghza1prime2_1E4_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1_1_ghza1prime2_1E4_JHUGen_JECDn;
            buffer -> p_HadZH_SIG_ghz1_1_ghza2_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1_1_ghza2_1_JHUGen_JECDn;
            buffer -> p_HadZH_SIG_ghz1_1_ghza4_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1_1_ghza4_1_JHUGen_JECDn;
            buffer -> p_HadZH_SIG_ghz1_1_gha2_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1_1_gha2_1_JHUGen_JECDn;
            buffer -> p_HadZH_SIG_ghz1_1_gha4_1_JHUGen_JECNominal = buffer -> p_HadZH_SIG_ghz1_1_gha4_1_JHUGen_JECDn;
            buffer -> p_HadWH_SIG_ghw1_1_JHUGen_JECNominal = buffer -> p_HadWH_SIG_ghw1_1_JHUGen_JECDn;
            buffer -> p_HadWH_SIG_ghw1prime2_1E4_JHUGen_JECNominal = buffer -> p_HadWH_SIG_ghw1prime2_1E4_JHUGen_JECDn;
            buffer -> p_HadWH_SIG_ghw2_1_JHUGen_JECNominal = buffer -> p_HadWH_SIG_ghw2_1_JHUGen_JECDn;
            buffer -> p_HadWH_SIG_ghw4_1_JHUGen_JECNominal = buffer -> p_HadWH_SIG_ghw4_1_JHUGen_JECDn;
            buffer -> p_HadWH_SIG_ghw1_1_ghw1prime2_1E4_JHUGen_JECNominal = buffer -> p_HadWH_SIG_ghw1_1_ghw1prime2_1E4_JHUGen_JECDn;
            buffer -> p_HadWH_SIG_ghw1_1_ghw2_1_JHUGen_JECNominal = buffer -> p_HadWH_SIG_ghw1_1_ghw2_1_JHUGen_JECDn;
            buffer -> p_HadWH_SIG_ghw1_1_ghw4_1_JHUGen_JECNominal = buffer -> p_HadWH_SIG_ghw1_1_ghw4_1_JHUGen_JECDn;
            buffer -> p_ttHUndecayed_SIG_kappa_1_JHUGen_JECNominal = buffer -> p_ttHUndecayed_SIG_kappa_1_JHUGen_JECDn;
            buffer -> p_ttHUndecayed_SIG_kappatilde_1_JHUGen_JECNominal = buffer -> p_ttHUndecayed_SIG_kappatilde_1_JHUGen_JECDn;
            buffer -> p_ttHUndecayed_SIG_kappa_1_kappatilde_1_JHUGen_JECNominal = buffer -> p_ttHUndecayed_SIG_kappa_1_kappatilde_1_JHUGen_JECDn;
            buffer -> p_bbH_SIG_kappa_1_JHUGen_JECNominal = buffer -> p_bbH_SIG_kappa_1_JHUGen_JECDn;
            buffer -> p_HadZH_mavjj_JECNominal = buffer -> p_HadZH_mavjj_JECDn;
            buffer -> p_HadZH_mavjj_true_JECNominal = buffer -> p_HadZH_mavjj_true_JECDn;
            buffer -> p_HadWH_mavjj_JECNominal = buffer -> p_HadWH_mavjj_JECDn;
            buffer -> p_HadWH_mavjj_true_JECNominal = buffer -> p_HadWH_mavjj_true_JECDn;
            buffer -> p_JJEW_SIG_ghv1_1_MCFM_JECNominal = buffer -> p_JJEW_SIG_ghv1_1_MCFM_JECDn;
            buffer -> p_JJEW_BSI_ghv1_1_MCFM_JECNominal = buffer -> p_JJEW_BSI_ghv1_1_MCFM_JECDn;
            buffer -> p_JJEW_BSI_ghv1_i_MCFM_JECNominal = buffer -> p_JJEW_BSI_ghv1_i_MCFM_JECDn;
            buffer -> p_JJEW_BKG_MCFM_JECNominal = buffer -> p_JJEW_BKG_MCFM_JECDn;
            buffer -> p_JJVBF_BKG_MCFM_JECNominal = buffer -> p_JJVBF_BKG_MCFM_JECDn;
            buffer -> p_JJQCD_BKG_MCFM_JECNominal = buffer -> p_JJQCD_BKG_MCFM_JECDn;
            buffer -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal_BestDJJ = buffer -> p_JJVBF_SIG_ghv1_1_JHUGen_JECDn_BestDJJ;
            buffer -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal_BestDJJ = buffer -> p_JJQCD_SIG_ghg2_1_JHUGen_JECDn_BestDJJ;
	}
	else if(mode == "BTag_UP")
	{
	    buffer -> nCleanedJetsPt30BTagged_bTagSF = buffer -> nCleanedJetsPt30BTagged_bTagSFUp;
	}
	else if(mode == "BTag_DN")
	{
	    buffer -> nCleanedJetsPt30BTagged_bTagSF = buffer -> nCleanedJetsPt30BTagged_bTagSFDn;
	}
	else if(mode == "LEC_UP")
	{
	    float s_e = 0.003;
	    float s_mu = 0.05;

	    // scale up all the associated leptons
	    for(unsigned int j = 0; j < buffer -> ExtraLepPt -> size(); j++)
	    {
		if(abs(buffer -> ExtraLepLepId -> at(j)) == 11)
		{
		    // have an electron: scale up by 0.3% in both barrel and endcap
		    (buffer -> ExtraLepPt -> at(j)) *= (1 + s_e);
		}
		else if(abs(buffer -> ExtraLepLepId -> at(j)) == 13)
		{
		    // have a muon: scale up by 5% * p_T / 1 TeV and smear by an additional 0.6%
		    (buffer -> ExtraLepPt -> at(j)) *= (1 + s_mu * (buffer -> ExtraLepPt -> at(j)) / 1000.0);

		    TRandom3 rand;
		    rand.SetSeed(abs(static_cast<int>(sin(buffer -> ExtraLepPhi -> at(j)) * 100000)));
		    float smear = rand.Gaus(0, 1.);

		    float sigma_up = 0.006 * (buffer -> ExtraLepPt -> at(j));

		    buffer -> ExtraLepPt -> at(j) = std::max(0.0f, smear * sigma_up + buffer -> ExtraLepPt -> at(j));
		}
	    }

	    // also scale up the invariant masses and p_t of the individual Z and ZZ candidates, for the worst case
	    if(buffer -> Z1Flav == -121.0)
	    {
		// Z -> ee
		buffer -> Z1Mass *= (1 + s_e);
		buffer -> Z1Pt *= (1 + 2 * s_e);
	    }
	    else if(buffer -> Z1Flav == -169.0)
	    {
		// Z -> mumu
		buffer -> Z1Mass *= (1 + s_mu * (buffer -> Z1Pt) / 1000.0);
		buffer -> Z1Pt *= (1 + 2 * s_mu * (buffer -> Z1Pt) / 1000.0);
	    }

	    if(buffer -> Z2Flav == -121.0)
	    {
		// Z -> ee
		buffer -> Z1Mass *= (1 + s_e);
		buffer -> Z1Pt *= (1 + 2 * s_e);
	    }
	    else if(buffer -> Z2Flav == -169.0)
	    {
		// Z -> mumu
		buffer -> Z2Mass *= (1 + s_mu * (buffer -> Z2Pt) / 1000.0);
		buffer -> Z2Pt *= (1 + 2 * s_mu * (buffer -> Z2Pt) / 1000.0);
	    }

	    // scaling of the four-lepton invariant mass in the worst case
	    buffer -> ZZPt *= (1 + 4 * s_e);
	    buffer -> ZZMass *= (1 + s_e); // according to the AN, already this covers by far the actual variation
	}
	else if(mode == "LEC_DN")
	{
	    float s_e = 0.003;
	    float s_mu = 0.05;

	    for(unsigned int j = 0; j < buffer -> ExtraLepPt -> size(); j++)
	    {
		if(abs(buffer -> ExtraLepLepId -> at(j)) == 11)
		{
		    // have an electron: scale down by 0.3% in both barrel and endcap
		    (buffer -> ExtraLepPt -> at(j)) *= (1 - 0.003);
		}
		else if(abs(buffer -> ExtraLepLepId -> at(j)) == 13)
		{
		    // have a muon: scale down by 5% * p_T / 1 TeV and smear by an additional 0.6%
		    (buffer -> ExtraLepPt -> at(j)) *= (1 - 0.05 * (buffer -> ExtraLepPt -> at(j)) / 1000.0);

		    TRandom3 rand;
		    rand.SetSeed(abs(static_cast<int>(sin(buffer -> ExtraLepPhi -> at(j)) * 100000)));
		    float smear = rand.Gaus(0, 1.);

		    float sigma_dn = 0.006 * (buffer -> ExtraLepPt -> at(j));

		    buffer -> ExtraLepPt -> at(j) = std::max(0.0f, smear * sigma_dn + buffer -> ExtraLepPt -> at(j));
		}
	    }

	    // also scale down the invariant masses and p_t of the individual Z and ZZ candidates
	    // also scale up the invariant masses and p_t of the individual Z and ZZ candidates, for the worst case
	    if(buffer -> Z1Flav == -121.0)
	    {
		// Z -> ee
		buffer -> Z1Mass *= (1 - s_e);
		buffer -> Z1Pt *= (1 - 2 * s_e);
	    }
	    else if(buffer -> Z1Flav == -169.0)
	    {
		// Z -> mumu
		buffer -> Z1Mass *= (1 - s_mu * (buffer -> Z1Pt) / 1000.0);
		buffer -> Z1Pt *= (1 - 2 * s_mu * (buffer -> Z1Pt) / 1000.0);
	    }

	    if(buffer -> Z2Flav == -121.0)
	    {
		// Z -> ee
		buffer -> Z1Mass *= (1 - s_e);
		buffer -> Z1Pt *= (1 - 2 * s_e);
	    }
	    else if(buffer -> Z2Flav == -169.0)
	    {
		// Z -> mumu
		buffer -> Z2Mass *= (1 - s_mu * (buffer -> Z2Pt) / 1000.0);
		buffer -> Z2Pt *= (1 - 2 * s_mu * (buffer -> Z2Pt) / 1000.0);
	    }

	    // scaling of the four-lepton invariant mass in the worst case
	    buffer -> ZZPt *= (1 - 4 * s_e);
	    buffer -> ZZMass *= (1 - s_e);	    
	    buffer -> ZZMass_masked *= (1 - s_e);	    
	}
	else
	{
	    std::cerr << "mode not supported, doing nothing" << std::endl;
	}

	// recompute all the MELA discriminants now with the swapped variables
	buffer -> D_bkg_ME = clamp_value(D_bkg_ME_disc(buffer), 0.0, 1.0);

	buffer -> D_VBF2j_ggH_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DVBF2j_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_VBF1j_ggH_ME = clamp_value(buffer -> nCleanedJetsPt30 == 1 ? DVBF1j_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_WHh_ggH_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DWHh_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_ZHh_ggH_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DZHh_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_WHh_ZHh_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DWHZH_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_VBF2j_WHh_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DVBFWH_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_VBF2j_ZHh_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DVBFZH_ME_disc(buffer) : 0.0, 0.0, 1.0);

	buffer -> D_VBF_dec_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DVBF2j_bkg_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_VH_dec_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DVH_bkg_ME_disc(buffer) : 0.0, 0.0, 1.0);

	output_tree -> Fill();
    }

    output_file -> cd(tree_name);
    output_tree -> Write();
    output_metadata -> Write();

    output_file -> Close();
    input_file -> Close();
}

int main( int argc, char *argv[] )
{
    if(argc != 4)
    {
	std::cerr << "Error: exactly 3 arguments are required" << std::endl;
	return(-1);
    }

    TString in_file = argv[1];
    TString out_file = argv[2];
    TString mode = argv[3];

    std::cout << "in_file = " << in_file << std::endl;
    std::cout << "out_file = " << out_file << std::endl;
    std::cout << "mode = " << mode << std::endl;
    
    prepare_systematics(in_file, out_file, mode);

    std::cout << "done with everything" << std::endl;

    return(0);
}
