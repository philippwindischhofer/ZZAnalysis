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

void augment_tree(TString inpath, TString outpath)
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

    output_tree -> Branch("leading_jet_pt", &(buffer -> leading_jet_pt), "leading_jet_pt/F");
    output_tree -> Branch("leading_jet_eta", &(buffer -> leading_jet_eta), "leading_jet_eta/F");
    output_tree -> Branch("leading_jet_sin_phi", &(buffer -> leading_jet_sin_phi), "leading_jet_sin_phi/F");
    output_tree -> Branch("leading_jet_cos_phi", &(buffer -> leading_jet_cos_phi), "leading_jet_cos_phi/F");

    output_tree -> Branch("leading_lep_pt", &(buffer -> leading_lep_pt), "leading_lep_pt/F");
    output_tree -> Branch("leading_lep_eta", &(buffer -> leading_lep_eta), "leading_lep_eta/F");
    output_tree -> Branch("leading_lep_sin_phi", &(buffer -> leading_lep_sin_phi), "leading_lep_sin_phi/F");
    output_tree -> Branch("leading_lep_cos_phi", &(buffer -> leading_lep_cos_phi), "leading_lep_cos_phi/F");
    
    output_tree -> Branch("leading_extra_lep_pt", &(buffer -> leading_extra_lep_pt), "leading_extra_lep_pt/F");
    output_tree -> Branch("leading_extra_lep_eta", &(buffer -> leading_extra_lep_eta), "leading_extra_lep_eta/F");
    output_tree -> Branch("leading_extra_lep_sin_phi", &(buffer -> leading_extra_lep_sin_phi), "leading_extra_lep_sin_phi/F");
    output_tree -> Branch("leading_extra_lep_cos_phi", &(buffer -> leading_extra_lep_cos_phi), "leading_extra_lep_cos_phi/F");

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

    //std::random_shuffle(indices.begin(), indices.end());

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

	// fill the variables holding the ME discriminants (don't care about their validity here, just compute everything. later on, when reading them, will need to check the number of jets etc. to make sure the MELA discriminants actually make sense)
	buffer -> D_VBF2j_ggH_ME = DVBF2j_ME_disc(buffer);
	buffer -> D_VBF1j_ggH_ME = DVBF1j_ME_disc(buffer);
	buffer -> D_WHh_ggH_ME = DWHh_ME_disc(buffer);
	buffer -> D_ZHh_ggH_ME = DZHh_ME_disc(buffer);
	buffer -> D_WHh_ZHh_ME = DWHZH_ME_disc(buffer);
	buffer -> D_VBF2j_WHh_ME = DVBFWH_ME_disc(buffer);
	buffer -> D_VBF2j_ZHh_ME = DVBFZH_ME_disc(buffer);

	// look for the leading jets and store its variables separately
	float leading_jet_pt = std::numeric_limits<float>::min();
	unsigned int leading_jet = 0;
	unsigned int number_jets = buffer -> JetPt -> size();
	for(unsigned int i = 0; i < number_jets; i++)
	{
	    if(buffer -> JetPt -> at(i) > leading_jet_pt)
	    {
		leading_jet = i;
		leading_jet_pt = buffer -> JetPt -> at(i);
	    }
	}

	if((number_jets > 0) && (leading_jet_pt > 30.0))
	{
	    buffer -> leading_jet_pt = leading_jet_pt;
	    buffer -> leading_jet_eta = buffer -> JetEta -> at(leading_jet);
	    buffer -> leading_jet_sin_phi = TMath::Sin(buffer -> JetPhi -> at(leading_jet));
	    buffer -> leading_jet_cos_phi = TMath::Cos(buffer -> JetPhi -> at(leading_jet));
	}
	else
	{
	    buffer -> leading_jet_pt = 0.0;
	    buffer -> leading_jet_eta = 0.0;
	    buffer -> leading_jet_sin_phi = 0.0;
	    buffer -> leading_jet_cos_phi = 0.0;
	}

	// look for the leading leptons and store its variables separately
	float leading_lep_pt = std::numeric_limits<float>::min();
	unsigned int leading_lep = 0;
	unsigned int number_leps = buffer -> LepPt -> size();
	for(unsigned int i = 0; i < number_leps; i++)
	{
	    if(buffer -> LepPt -> at(i) > leading_lep_pt)
	    {
		leading_lep = i;
		leading_lep_pt = buffer -> LepPt -> at(i);
	    }
	}

	if((number_leps > 0) && (leading_lep_pt > 30.0))
	{
	    buffer -> leading_lep_pt = leading_lep_pt;
	    buffer -> leading_lep_eta = buffer -> LepEta -> at(leading_lep);
	    buffer -> leading_lep_sin_phi = TMath::Sin(buffer -> LepPhi -> at(leading_lep));
	    buffer -> leading_lep_cos_phi = TMath::Cos(buffer -> LepPhi -> at(leading_lep));
	}
	else
	{
	    buffer -> leading_lep_pt = 0.0;
	    buffer -> leading_lep_eta = 0.0;
	    buffer -> leading_lep_sin_phi = 0.0;
	    buffer -> leading_lep_cos_phi = 0.0;
	}

	// look for the leading extra leptons and store its variables separately
	float leading_extra_lep_pt = std::numeric_limits<float>::min();
	unsigned int leading_extra_lep = 0;
	unsigned int number_extra_leps = buffer -> ExtraLepPt -> size();
	for(unsigned int i = 0; i < number_extra_leps; i++)
	{
	    if(buffer -> ExtraLepPt -> at(i) > leading_extra_lep_pt)
	    {
		leading_extra_lep = i;
		leading_extra_lep_pt = buffer -> ExtraLepPt -> at(i);
	    }
	}

	if((number_extra_leps > 0) && (leading_extra_lep_pt > 30.0))
	{
	    buffer -> leading_extra_lep_pt = leading_extra_lep_pt;
	    buffer -> leading_extra_lep_eta = buffer -> ExtraLepEta -> at(leading_extra_lep);
	    buffer -> leading_extra_lep_sin_phi = TMath::Sin(buffer -> ExtraLepPhi -> at(leading_extra_lep));
	    buffer -> leading_extra_lep_cos_phi = TMath::Cos(buffer -> ExtraLepPhi -> at(leading_extra_lep));
	}
	else
	{
	    buffer -> leading_extra_lep_pt = 0.0;
	    buffer -> leading_extra_lep_eta = 0.0;
	    buffer -> leading_extra_lep_sin_phi = 0.0;
	    buffer -> leading_extra_lep_cos_phi = 0.0;
	}

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
    Mor18Config* conf = new Mor18Config("/data_CMS/cms/tsculac/CJLST_NTuples/", 35.9, true);
    TString out_dir = "/data_CMS/cms/wind/CJLST_NTuples_randomized/";
    
    // first, make a list of all files, independent of the cut with which they appear in the signal routing table
    std::vector<std::pair<TString, Routing*>> routing_table = conf -> get_routing();
    std::vector<TString> infile_paths;
    std::vector<TString> outfile_paths;

    for(auto& routing: routing_table)
    {
	TString cur_file = routing.first;
	if(std::find(infile_paths.begin(), infile_paths.end(), conf -> MC_path() + cur_file + conf -> MC_filename()) == infile_paths.end())
	{
	    infile_paths.push_back(conf -> MC_path() + cur_file + conf -> MC_filename());
	    outfile_paths.push_back(out_dir + cur_file + conf -> MC_filename());

	    std::cout << "added " << conf -> MC_path() + cur_file + conf -> MC_filename() << " to queue" << std::endl;
	}
    }

    for(auto tup: boost::combine(infile_paths, outfile_paths))
    {
	TString infile;
	TString outfile;
	boost::tie(infile, outfile) = tup;

	std::cout << "now augmenting " << infile << " ==> " << outfile << std::endl;

	augment_tree(infile, outfile);
    }

    return(0);
}
