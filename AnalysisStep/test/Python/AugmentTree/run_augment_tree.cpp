// C++
#include <iostream>
#include <fstream>
#include <string>

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

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>

int main( int argc, char *argv[] )
{
    Mor18Config* conf = new Mor18Config();
    
    TString signal_name = "ggH125";
    TString out_dir = "/home/llr/cms/wind/CJLST_NTuples/";

    TString input_file_name = conf -> MC_path() + signal_name + conf -> MC_filename();
    TString output_file_name = out_dir + signal_name + conf -> MC_filename();

    TFile* input_file = new TFile(input_file_name, "read");
    TFile* output_file = new TFile(output_file_name, "recreate");

    // read some auxiliary information
    TH1F* hCounters = (TH1F*)input_file -> Get("ZZTree/Counters");
    Long64_t n_gen_events = (Long64_t)hCounters -> GetBinContent(1);
    Long64_t gen_sum_weights = (Long64_t)hCounters -> GetBinContent(40);
    TTree* input_tree = (TTree*)input_file -> Get("ZZTree/candTree");

    Tree* buffer = new Tree();
    buffer -> Init(input_tree, input_file_name);

    Long64_t n_entries = buffer -> fChain -> GetEntriesFast();

    TTree* output_tree = (TTree*)(input_tree -> CloneTree(0));
    TBranch* newBranch = output_tree -> Branch("testval", &(buffer -> testval), "testval/F");

    // loop over the entries in chain
    for(Long64_t j_entry = 0; j_entry < n_entries; j_entry++)
    {
	// get the correct tree in the chain that contains this event
	Long64_t i_entry = buffer -> LoadTree(j_entry);
	if(i_entry < 0) break;

	// now actually read this entry
	buffer -> fChain -> GetEntry(j_entry);
	buffer -> testval = 3.14;

	output_tree -> Fill();
    }

    output_file -> cd();
    output_tree -> Write();
    output_file -> Close();
    input_file -> Close();

    return(0);
}
