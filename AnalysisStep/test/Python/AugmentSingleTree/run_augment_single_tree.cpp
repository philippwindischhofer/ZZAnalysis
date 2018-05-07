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

    // the name of the relevant input tree will differ, depending on whether this script is run on data or MC!
    TString tree_name;
    if(inpath.Contains("AllData"))
    {
	tree_name = "CRZLLTree";
    }
    else
    {
	tree_name = "ZZTree";
    }

    // ... but the augmented tree it produces is always going to have the same name!
    TString tree_name_output = "ClassTree";
    
    // read the metadata
    TH1F* hCounters = (TH1F*)input_file -> Get(tree_name + "/Counters");
    Long64_t n_gen_events = (Long64_t)hCounters -> GetBinContent(1);
    Long64_t gen_sum_weights = (Long64_t)hCounters -> GetBinContent(40);

    TFile* output_file = new TFile(outpath, "recreate");
    output_file -> mkdir(tree_name_output);

    // read some auxiliary information
    TH1F* input_metadata = (TH1F*)input_file -> Get(tree_name + "/Counters");
    TTree* input_tree = (TTree*)input_file -> Get(tree_name + "/candTree");

    input_tree -> LoadBaskets(1000000000000);

    Tree* buffer = new Tree();
    buffer -> Init(input_tree, inpath);
    Long64_t n_entries = buffer -> fChain -> GetEntriesFast();

    TTree* output_tree = (TTree*)(input_tree -> CloneTree(0));
    TH1F* output_metadata = (TH1F*)(input_metadata -> Clone());

    // create the new branches that are needed in the output tree
    output_tree -> Branch("training_weight", &(buffer -> training_weight), "training_weight/F");

    output_tree -> Branch("ZZMass_masked", &(buffer -> ZZMass_masked), "ZZMass_masked/F");

    output_tree -> Branch("D_bkg_ME", &(buffer -> D_bkg_ME), "D_bkg_ME/F");

    output_tree -> Branch("D_VBF2j_ggH_ME", &(buffer -> D_VBF2j_ggH_ME), "D_VBF2j_ggH_ME/F");
    output_tree -> Branch("D_VBF1j_ggH_ME", &(buffer -> D_VBF1j_ggH_ME), "D_VBF1j_ggH_ME/F");
    output_tree -> Branch("D_WHh_ggH_ME", &(buffer -> D_WHh_ggH_ME), "D_WHh_ggH_ME/F");
    output_tree -> Branch("D_ZHh_ggH_ME", &(buffer -> D_ZHh_ggH_ME), "D_ZHh_ggH_ME/F");
    output_tree -> Branch("D_WHh_ZHh_ME", &(buffer -> D_WHh_ZHh_ME), "D_WHh_ZHh_ME/F");
    output_tree -> Branch("D_VBF2j_WHh_ME", &(buffer -> D_VBF2j_WHh_ME), "D_VBF2j_WHh_ME/F");
    output_tree -> Branch("D_VBF2j_ZHh_ME", &(buffer -> D_VBF2j_ZHh_ME), "D_VBF2j_ZHh_ME/F");

    TBranch* br_oew = (TBranch*)(buffer -> fChain -> GetListOfBranches() -> FindObject("overallEventWeight"));
    if(br_oew)
    {
    	std::cout << "overallEventWeight available in file " << inpath << std::endl;
    }
    else
    {
    	std::cout << "adding overallEventWeight in file " << inpath << std::endl;
    	output_tree -> Branch("overallEventWeight", &(buffer -> overallEventWeight), "overallEventWeight/F");
    }

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

    TBranch* br_gap1 = (TBranch*)(buffer -> fChain -> GetListOfBranches() -> FindObject("GenAssocLep1Id"));
    if(br_gap1)
    {
    	std::cout << "GenAssocLep1Id available in file " << inpath << std::endl;
    }
    else
    {
    	std::cout << "adding GenAssocLep1Id in file " << inpath << std::endl;
    	output_tree -> Branch("GenAssocLep1Id", &(buffer -> GenAssocLep1Id));
    }

    TBranch* br_gap2 = (TBranch*)(buffer -> fChain -> GetListOfBranches() -> FindObject("GenAssocLep2Id"));
    if(br_gap2)
    {
    	std::cout << "GenAssocLep2Id available in file " << inpath << std::endl;
    }
    else
    {
    	std::cout << "adding GenAssocLep2Id in file " << inpath << std::endl;
    	output_tree -> Branch("GenAssocLep2Id", &(buffer -> GenAssocLep2Id));
    }

    TBranch* br_xs = (TBranch*)(buffer -> fChain -> GetListOfBranches() -> FindObject("xsec"));
    if(br_xs)
    {
	std::cout << "xsec available in file " << inpath << std::endl;
    }
    else
    {
	std::cout << "xsec not available in file " << inpath << std::endl;
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

	std::cout << j_entry << " <-> " << rand_index << std::endl;
	
	// now actually read this entry
	buffer -> fChain -> GetEntry(rand_index);

	if(!br_lhe)
	{
	    buffer -> LHEAssociatedParticleId = new std::vector<short>();
	}

	if(!br_gap1)
	{
	    buffer -> GenAssocLep1Id = 0.0;
	}

	if(!br_gap2)
	{
	    buffer -> GenAssocLep2Id = 0.0;
	}

	// fill the variables holding the ME discriminants (for the classes of events for which they actually make sense)
	buffer -> D_bkg_ME = clamp_value(D_bkg_ME_disc(buffer), 0.0, 1.0);

	buffer -> D_VBF2j_ggH_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DVBF2j_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_VBF1j_ggH_ME = clamp_value(buffer -> nCleanedJetsPt30 == 1 ? DVBF1j_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_WHh_ggH_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DWHh_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_ZHh_ggH_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DZHh_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_WHh_ZHh_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DWHZH_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_VBF2j_WHh_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DVBFWH_ME_disc(buffer) : 0.0, 0.0, 1.0);
	buffer -> D_VBF2j_ZHh_ME = clamp_value(buffer -> nCleanedJetsPt30 >= 2 ? DVBFZH_ME_disc(buffer) : 0.0, 0.0, 1.0);

	// fill the masked ZZMass
	if(buffer -> ZZMass > 150.0)
	{
	    buffer -> ZZMass_masked = buffer -> ZZMass;
	}
	else
	{
	    buffer -> ZZMass_masked = 0.0;
	}

	if(!br_oew)
	{
	    buffer -> overallEventWeight = 1.0;
	}

	float training_weight = 1.0;
	if(br_xs)
	{
	    training_weight = ((buffer -> xsec) * (buffer -> overallEventWeight)) / gen_sum_weights;
	}

	buffer -> training_weight = training_weight;

	output_tree -> Fill();
    }

    output_file -> cd(tree_name_output);
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
