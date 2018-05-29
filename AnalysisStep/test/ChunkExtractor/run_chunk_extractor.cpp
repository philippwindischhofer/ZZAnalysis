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

void extract_chunk(TString inpath, TString outpath, float start_fraction, float end_fraction, bool reweighting)
{
    TString tree_name = "ClassTree";

    std::cout << "reading from " << inpath << std::endl;

    TFile* input_file = new TFile(inpath, "read");

    // read the metadata
    TH1F* hCounters = (TH1F*)input_file -> Get(tree_name + "/Counters");
    Long64_t gen_sum_weights = (Long64_t)hCounters -> GetBinContent(40);

    TFile* output_file = new TFile(outpath, "recreate");
    output_file -> mkdir(tree_name);

    // read some auxiliary information
    TH1F* input_metadata = (TH1F*)input_file -> Get(tree_name + "/Counters");
    TTree* input_tree = (TTree*)input_file -> Get(tree_name + "/candTree");

    Tree* buffer = new Tree();
    buffer -> Init(input_tree, inpath);
    Long64_t n_entries = buffer -> fChain -> GetEntriesFast();

    std::cout << "have " << n_entries << " events available in this file" << std::endl;

    TTree* output_tree = (TTree*)(input_tree -> CloneTree(0));
    TH1F* output_metadata = (TH1F*)(input_metadata -> Clone());

    std::cout << "preparation done. starting chunk extraction" << std::endl;

    // compute the reweighting factor
    float reweighting_factor = 1.0;

    if(reweighting)
    {
	float full_gen_sum_weights = 0.0;
	float reduced_gen_sum_weights = 0.0;

	for(Long64_t j_entry = (Long64_t)(n_entries * start_fraction); j_entry < (Long64_t)(n_entries * end_fraction); j_entry++)
	{
	    buffer -> LoadTree(j_entry);
	    buffer -> fChain -> GetEntry(j_entry);
	    reduced_gen_sum_weights += buffer -> overallEventWeight;	
	}

	full_gen_sum_weights = reduced_gen_sum_weights;

	// for the full weight sum, need to add the part in the beginning
	for(Long64_t j_entry = 0; j_entry < (Long64_t)(n_entries * start_fraction); j_entry++)
	{
	    buffer -> LoadTree(j_entry);
	    buffer -> fChain -> GetEntry(j_entry);
	    full_gen_sum_weights += buffer -> overallEventWeight;
	}

	// and in the end
	for(Long64_t j_entry = (Long64_t)(n_entries * end_fraction); j_entry < n_entries; j_entry++)
	{
	    buffer -> LoadTree(j_entry);
	    buffer -> fChain -> GetEntry(j_entry);
	    full_gen_sum_weights += buffer -> overallEventWeight;
	}

	reweighting_factor = reduced_gen_sum_weights / full_gen_sum_weights;

	std::cout << "reduced_gen_sum_weights = " << reduced_gen_sum_weights << std::endl;
	std::cout << "full_gen_sum_weights = " << full_gen_sum_weights << std::endl;
    }

    std::cout << "reweighting_factor = " << reweighting_factor << std::endl;
 
    // loop over the entries in chain
    int entries_out = 0;
    for(Long64_t j_entry = (Long64_t)(n_entries * start_fraction); j_entry < (Long64_t)(n_entries * end_fraction); j_entry++)
    {
	// get the correct tree in the chain that contains this event
	Long64_t i_entry = buffer -> LoadTree(j_entry);
	if(i_entry < 0) break;

	// now actually read this entry
	buffer -> fChain -> GetEntry(j_entry);

	// the only thing that needs to be changed in terms of the stored values is the weight of the event, and any value that is derived from it (i.e. only "training_weight")
	buffer -> overallEventWeight /= reweighting_factor;

	float training_weight = ((buffer -> xsec) * (buffer -> overallEventWeight)) / gen_sum_weights;
	buffer -> training_weight = training_weight;

	output_tree -> Fill();
	entries_out++;
    }

    output_file -> cd(tree_name);
    output_tree -> Write();
    output_metadata -> Write();

    output_file -> Close();
    input_file -> Close();

    std::cout << "wrote " << entries_out << " events" << std::endl;
}

int main( int argc, char *argv[] )
{
    if(argc < 5)
    {
	std::cerr << "Error: at least 4 arguments are required" << std::endl;
	return(-1);
    }

    TString in_file = argv[1];
    TString out_file = argv[2];
    float start_fraction = std::atof(argv[3]);
    float end_fraction = std::atof(argv[4]);

    bool reweighting = true;

    if(argc == 6)
    {
	reweighting = std::atoi(argv[5]);
    }

    std::cout << "in_file = " << in_file << std::endl;
    std::cout << "out_file = " << out_file << std::endl;
    std::cout << "start_fraction = " << start_fraction << std::endl;
    std::cout << "end_fraction = " << end_fraction << std::endl;
    std::cout << "reweighting = " << reweighting << std::endl;
    
    extract_chunk(in_file, out_file, start_fraction, end_fraction, reweighting);

    std::cout << "done with everything" << std::endl;

    return(0);
}
