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

float clamp_value(float val, float min, float max)
{
    if(val > max)
	return max;

    if(val < min)
	return min;

    return val;
}

void augment_tree(TString inpath, TString outpath, int randomize)
{
    TFile* input_file = new TFile(inpath, "read");

    // read the metadata
    TH1F* hCounters = (TH1F*)input_file -> Get("ZZTree/Counters");
    Long64_t n_gen_events = (Long64_t)hCounters -> GetBinContent(1);
    Long64_t gen_sum_weights = (Long64_t)hCounters -> GetBinContent(40);

    TFile* output_file = new TFile(outpath, "recreate");
    output_file -> mkdir("ZZTree");

    // read some auxiliary information
    TH1F* input_metadata = (TH1F*)input_file -> Get("ZZTree/Counters");
    TTree* input_tree = (TTree*)input_file -> Get("ZZTree/candTree");

    Tree* buffer = new Tree();
    buffer -> Init(input_tree, inpath);
    Long64_t n_entries = buffer -> fChain -> GetEntriesFast();

    TTree* output_tree = (TTree*)(input_tree -> CloneTree(0));
    TH1F* output_metadata = (TH1F*)(input_metadata -> Clone());

    // create the new branches that are needed in the output tree
    output_tree -> Branch("training_weight", &(buffer -> training_weight), "training_weight/F");

    output_tree -> Branch("D_VBF2j_ggH_ME", &(buffer -> D_VBF2j_ggH_ME), "D_VBF2j_ggH_ME/F");
    output_tree -> Branch("D_VBF1j_ggH_ME", &(buffer -> D_VBF1j_ggH_ME), "D_VBF1j_ggH_ME/F");
    output_tree -> Branch("D_WHh_ggH_ME", &(buffer -> D_WHh_ggH_ME), "D_WHh_ggH_ME/F");
    output_tree -> Branch("D_ZHh_ggH_ME", &(buffer -> D_ZHh_ggH_ME), "D_ZHh_ggH_ME/F");
    output_tree -> Branch("D_WHh_ZHh_ME", &(buffer -> D_WHh_ZHh_ME), "D_WHh_ZHh_ME/F");
    output_tree -> Branch("D_VBF2j_WHh_ME", &(buffer -> D_VBF2j_WHh_ME), "D_VBF2j_WHh_ME/F");
    output_tree -> Branch("D_VBF2j_ZHh_ME", &(buffer -> D_VBF2j_ZHh_ME), "D_VBF2j_ZHh_ME/F");

    // also homogenize in the information needed for the cuts on the Python side, i.e. add dummy values for "LHEAssociatedparticleId"
    TBranch* br_lhe = (TBranch*)(buffer -> fChain -> GetListOfBranches() -> FindObject("LHEAssociatedParticleId"));

    if(br_lhe)
    {
	std::cout << "LHEAssociatedParticleId available in file " << inpath << std::endl;
    }
    else
    {
	std::cout << "adding LHEAssociatedParticleId in file " << inpath << std::endl;
	output_tree -> Branch("LHEAssociatedParticleId", &(buffer -> LHEAssociatedParticleId));
    }

    std::cout << "generating random numbers" << std::endl;

    // generate the randomization of the data:
    std::vector<Long64_t> indices;
    for(Long64_t j_entry = 0; j_entry < n_entries; j_entry++)
    {
	indices.push_back(j_entry);
    }

    if(randomize)
	std::random_shuffle(indices.begin(), indices.end());

    std::cout << "done. starting augmentation" << std::endl;
    
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

	float training_weight = ((buffer -> xsec) * (buffer -> overallEventWeight)) / gen_sum_weights;

	// fill the placeholder variable
	buffer -> training_weight = training_weight;

	if(!br_lhe)
	{
	    buffer -> LHEAssociatedParticleId = new std::vector<short>();
	}

	// fill the variables holding the ME discriminants (for the classes of events for which they actually make sense)
	buffer -> D_VBF2j_ggH_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DVBF2j_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_VBF1j_ggH_ME = clamp_value(buffer -> nCleanedJetsPt30 == 1 ? DVBF1j_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_WHh_ggH_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DWHh_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_ZHh_ggH_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DZHh_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_WHh_ZHh_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DWHZH_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_VBF2j_WHh_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DVBFWH_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_VBF2j_ZHh_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DVBFZH_ME_disc(buffer) : 0.0, 0.0, 1.0);

	output_tree -> Fill();
    }

    output_file -> cd("ZZTree");
    output_tree -> Write();
    output_metadata -> Write();

    output_file -> Close();
    input_file -> Close();
}

int main( int argc, char *argv[] )
{
    // take the very original files and augment them
    Mor18Config* conf = new Mor18Config("/data_CMS/cms/tsculac/CJLST_NTuples/", 35.9, true);

    if(argc != 4)
    {
	std::cerr << "Error: exactly 3 arguments are required" << std::endl;
	return(-1);
    }

    TString in_file = argv[1];
    TString out_file = argv[2];
    int randomize = std::atoi(argv[3]);

    std::cout << "in_file = " << in_file << std::endl;
    std::cout << "out_file = " << out_file << std::endl;
    std::cout << "randomize = " << randomize << std::endl;
    
    augment_tree(in_file, out_file, randomize);

    std::cout << "done with everything" << std::endl;

    return(0);
}
