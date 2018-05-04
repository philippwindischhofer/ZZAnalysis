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

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/me_discriminants.h>

void scramble(TString inpath, TString outpath)
{
    TString tree_name = "ClassTree";

    TFile* input_file = new TFile(inpath, "read");

    // read the metadata
    TH1F* hCounters = (TH1F*)input_file -> Get(tree_name + "/Counters");
    Long64_t n_gen_events = (Long64_t)hCounters -> GetBinContent(1);
    Long64_t gen_sum_weights = (Long64_t)hCounters -> GetBinContent(40);

    TFile* output_file = new TFile(outpath, "recreate");
    output_file -> mkdir(tree_name);

    // read some auxiliary information
    TH1F* input_metadata = (TH1F*)input_file -> Get(tree_name + "/Counters");
    TTree* input_tree = (TTree*)input_file -> Get(tree_name + "/candTree");

    input_tree -> LoadBaskets();

    Tree* buffer = new Tree();
    buffer -> Init(input_tree, inpath);
    Long64_t n_entries = buffer -> fChain -> GetEntriesFast();

    TTree* output_tree = (TTree*)(input_tree -> CloneTree(0));
    TH1F* output_metadata = (TH1F*)(input_metadata -> Clone());

    std::cout << "preparation done. starting chunk extraction" << std::endl;

    // generate the randomization of the data:
    std::vector<Long64_t> indices;
    for(Long64_t j_entry = 0; j_entry < n_entries; j_entry++)
    {
	indices.push_back(j_entry);
    }

    std::random_shuffle(indices.begin(), indices.end());
 
    // loop over the entries in chain
    for(Long64_t j_entry = 0; j_entry < n_entries; j_entry++)
    {
	// draw a random index from the array:
	Long64_t rand_index = indices.at(j_entry);

	// get the correct tree in the chain that contains this event
	Long64_t i_entry = buffer -> LoadTree(rand_index);
	if(i_entry < 0) break;

	// now actually read this entry
	buffer -> fChain -> GetEntry(rand_index);

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
    if(argc != 3)
    {
	std::cerr << "Error: exactly 2 arguments are required" << std::endl;
	return(-1);
    }

    TString in_file = argv[1];
    TString out_file = argv[2];

    std::cout << "in_file = " << in_file << std::endl;
    std::cout << "out_file = " << out_file << std::endl;
    
    scramble(in_file, out_file);

    std::cout << "done with everything" << std::endl;

    return(0);
}
