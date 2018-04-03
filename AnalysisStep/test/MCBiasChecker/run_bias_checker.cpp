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
#include "TH2F.h"
#include "TGraph.h"
#include "TMultiGraph.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Tree.h>

int main( int argc, char *argv[] )
{
    gStyle -> SetOptStat(0);

    float lumi = 35.9;
    
    TString input_file_name = "/data_CMS/cms/wind/CJLST_NTuples/ggH125/ZZ4lAnalysis.root";
    TFile* input_file = new TFile(input_file_name);
    
    std::cout << "reading from " << input_file_name << std::endl;
    
    // read some auxiliary information
    TH1F* hCounters = (TH1F*)input_file -> Get("ZZTree/Counters");
    Long64_t n_gen_events = (Long64_t)hCounters -> GetBinContent(1);
    Long64_t gen_sum_weights = (Long64_t)hCounters -> GetBinContent(40);

    TTree* input_tree = (TTree*)input_file -> Get("ZZTree/candTree");
    
    Tree* buffer = new Tree();
    buffer -> Init(input_tree, input_file_name);
    
    if(buffer -> fChain == 0) return -1;

    // total number of entries stored in the entire chain
    Long64_t n_entries = buffer -> fChain -> GetEntriesFast();
    std::cout << "total number of entries = " << n_entries << std::endl;

    float weight_sum = 0;
    int fill_cnt = 0;

    TH2F* datahist = new TH2F("", "", 500, 0, n_entries, 100, 0, 200);

    // loop over the entries in chain
    for(Long64_t j_entry = 0; j_entry < n_entries; j_entry++)
    {
	// get the correct tree in the chain that contains this event
	Long64_t i_entry = buffer -> LoadTree(j_entry);
	if(i_entry < 0) break;

	// now actually read this entry
	buffer -> fChain -> GetEntry(j_entry);

	float event_weight = (lumi * buffer -> xsec * 1000. * buffer -> overallEventWeight) / gen_sum_weights;

	datahist -> Fill(j_entry, event_weight);
	weight_sum += event_weight;
	fill_cnt++;
    }

    TCanvas* canv = new TCanvas("canv", "canv", 800, 600);

    datahist -> Draw("colz");
    datahist -> GetXaxis() -> SetTitle("event number (a.u.)");
    datahist -> GetYaxis() -> SetTitle("LepPt[0]");
    canv -> SetRightMargin(0.16);
    canv -> Draw("same colz");
    canv -> SaveAs("LepPt.pdf");
    return(0);
}
