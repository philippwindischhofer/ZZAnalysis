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

// own local files
#include <ZZAnalysis/AnalysisStep/test/Profiler/include/Profiler.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/Mor17Classifier.h>
#include <ZZAnalysis/AnalysisStep/interface/Category.h>

// global file locations
float lumi = 35.9f;

TString path = "/data_CMS/cms/tsculac/CJLST_NTuples/";
TString file_name = "/ZZ4lAnalysis.root";
TString out_folder = "../../src/ZZAnalysis/ProfilerPlots/";

std::vector<TString> signal_file_names = {
    "ggH125",
    "VBFH125",
    "WplusH125",
    "WminusH125",
    "ZH125",
    "ttH125",
    "bbH125",
    "tqH125"
};

std::vector<TString> background_file_names = {
    "ZZTo4l", 
    "DYJetsToLL_M50",
    "TTTo2L2Nu",
    "ggTo2e2mu_Contin_MCFM701",
    "ggTo2e2tau_Contin_MCFM701",
    "ggTo2mu2tau_Contin_MCFM701",
    "ggTo4e_Contin_MCFM701",
    "ggTo4mu_Contin_MCFM701",
    "ggTo4tau_Contin_MCFM701"
};

int main(int argc, char *argv[])
{
    Profiler* prof = new Profiler();
    TH1F* hist1d = new TH1F("hist1d", "hist1d", 100, 0, 100);
    TH3F* hist3d = new TH3F("hist3d", "hist3d", 100, -180, -110, 100, -180, -110, 100, 0, 2);
    
    TString input_path = path + signal_file_names[5] + file_name;

    Mor17Classifier* testclass = new Mor17Classifier();

/*
    prof -> FillProfile(input_path, lumi, hist1d, 
			[&](Tree* in) -> bool{
			    return testclass -> ClassifyThisEvent(in) == 5 ? 
				kTRUE : kFALSE;},
			[&](Tree* in) -> float{return in -> PFMET;}
	);
*/
    prof -> FillProfile(input_path, lumi, hist3d,
			[&](Tree* in) -> bool{
			    return testclass -> ClassifyThisEvent(in) == ttHTaggedMor17 ? 
				kTRUE : kFALSE;},
			[&](Tree* in) -> float{return in -> Z1Flav;},
			[&](Tree* in) -> float{return in -> Z2Flav;},
			[&](Tree* in) -> float{return in -> nExtraLep;}
	);

    TCanvas* canv = new TCanvas("canv", "canv", 600, 600);
    canv -> cd();
    hist3d -> Draw("BOX");
    canv -> SaveAs(out_folder + "3d.pdf");

    return(0);
}
