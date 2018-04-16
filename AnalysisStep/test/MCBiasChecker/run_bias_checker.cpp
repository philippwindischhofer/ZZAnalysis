// C++
#include <iostream>
#include <fstream>
#include <string>
#include <limits>

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
#include "TH2F.h"
#include "TGraph.h"
#include "TMultiGraph.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Tree.h>

float lumi = 35.9;
Long64_t gen_sum_weights = 0;

void ProfileData(TString input_file_name, TString output_file_name, const std::function<float(Tree*)>& var, const std::function<bool(Tree*)>& cut)
{
    gStyle -> SetOptStat(0);
    
//    TString input_file_name = "/data_CMS/cms/wind/CJLST_NTuples/VBFH125/ZZ4lAnalysis.root";
    TFile* input_file = new TFile(input_file_name);
    
    std::cout << "reading from " << input_file_name << std::endl;
    
    // read some auxiliary information
    TH1F* hCounters = (TH1F*)input_file -> Get("ZZTree/Counters");
    Long64_t n_gen_events = (Long64_t)hCounters -> GetBinContent(1);
    gen_sum_weights = (Long64_t)hCounters -> GetBinContent(40);

    TTree* input_tree = (TTree*)input_file -> Get("ZZTree/candTree");
    
    Tree* buffer = new Tree();
    buffer -> Init(input_tree, input_file_name);
    
    // total number of entries stored in the entire chain
    Long64_t n_entries = buffer -> fChain -> GetEntriesFast();
    std::cout << "total number of entries = " << n_entries << std::endl;

    // determine the minimum and maximum values
    float maxval = 0.0;
    float minval = std::numeric_limits<float>::max();
    for(Long64_t j_entry = 0; j_entry < n_entries; j_entry++)
    {
	// get the correct tree in the chain that contains this event
	Long64_t i_entry = buffer -> LoadTree(j_entry);
	if(i_entry < 0) break;

	// now actually read this entry
	buffer -> fChain -> GetEntry(j_entry);

	//if((buffer -> ZZMass > 118.0) && (buffer -> ZZMass < 130.0))
	if(cut(buffer))
	{
	    //float event_weight = (lumi * buffer -> xsec * 1000. * buffer -> overallEventWeight) / gen_sum_weights;
	    float plotvar = var(buffer);
	    
	    if(plotvar < minval)
		minval = plotvar;
	    
	    if(plotvar > maxval)
		maxval = plotvar;
	}
    }

    TH2F* datahist = new TH2F("", "", 500, 0, n_entries, 100, minval - 0.1 * (maxval - minval), maxval + 0.1 * (maxval - minval));

    float weight_sum = 0;
    int fill_cnt = 0;

    // loop over the entries in chain
    for(Long64_t j_entry = 0; j_entry < n_entries; j_entry++)
    {
	// get the correct tree in the chain that contains this event
	Long64_t i_entry = buffer -> LoadTree(j_entry);
	if(i_entry < 0) break;

	// now actually read this entry
	buffer -> fChain -> GetEntry(j_entry);

	//if((buffer -> ZZMass > 118.0) && (buffer -> ZZMass < 130.0))
	if(cut(buffer))
	{
	    float event_weight = (lumi * buffer -> xsec * 1000. * buffer -> overallEventWeight) / gen_sum_weights;
	    float plotvar = var(buffer);
	    
	    datahist -> Fill(j_entry, plotvar);
	    weight_sum += event_weight;
	    fill_cnt++;
	}
    }

    TCanvas* canv = new TCanvas("canv", "canv", 800, 600);

    datahist -> Draw("colz");
    datahist -> GetXaxis() -> SetTitle("event number (a.u.)");
    datahist -> GetYaxis() -> SetTitle("event weight");
    canv -> SetRightMargin(0.16);
    canv -> SetLeftMargin(0.16);
    canv -> Draw("same colz");
    //canv -> SaveAs("event_weight.pdf");
    canv -> SaveAs(output_file_name);
}

float get_event_weight(Tree* buffer)
{
    return (lumi * buffer -> xsec * 1000. * buffer -> overallEventWeight) / gen_sum_weights;
}

float mZZ_cut(Tree* buffer)
{
    return ((buffer -> ZZMass > 118.0) && (buffer -> ZZMass < 130.0)) ? kTRUE : kFALSE;
}

int main( int argc, char *argv[] )
{
    std::vector<TString> files = {"VBFH125", "WminusH125", "WplusH125", "ZH125", "ZZTo4l", "ggH125", "ggTo2e2mu_Contin_MCFM701", "ggTo2e2tau_Contin_MCFM701", "ggTo2mu2tau_Contin_MCFM701", "ggTo4e_Contin_MCFM701", "ggTo4mu_Contin_MCFM701", "ggTo4tau_Contin_MCFM701", "ttH125"};
    TString file_prefix = "/data_CMS/cms/wind/CJLST_NTuples_randomized/";
    TString file_postfix = "/ZZ4lAnalysis.root";

    for(auto file: files)
    {
	ProfileData(file_prefix + file + file_postfix, file + "_event_weight.pdf", get_event_weight, mZZ_cut);
    }

    return(0);
}
