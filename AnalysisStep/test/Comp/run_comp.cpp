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

#define Mor18

#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/utils.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/CatPlotter.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/histo_settings.h>

// global definitions
float lumi = 35.9f;
TString indir_a = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/LikBenchmarkerPlots/";
TString indir_b = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlots/";

TString name_a = "LikClass";
TString name_b = "Mor18";

TString out_folder = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/CompPlots/";

float zoom_scale = 0.05;

#define PUNZI_HIST_NAME "punzi_purity"
#define PUNZI_FILE "punzi_plot_hist.root"

int main( int argc, char *argv[] )
{
    std::vector<TString> hist_name = {PUNZI_HIST_NAME};

    // need to load now the Punzi histograms that should be sitting in these two directories, then can make their ratio or compare them in some other way
    std::vector<TH1F*> punzi_a = read_histos(indir_a + STORAGE_PREFIX + PUNZI_FILE, hist_name);
    std::vector<TH1F*> punzi_b = read_histos(indir_b + STORAGE_PREFIX + PUNZI_FILE, hist_name);

    std::vector<TH1F*> comp_vec;
    comp_vec.push_back(punzi_a[0]);
    comp_vec[0] -> Divide(punzi_b[0]);

    // now can make the ratio
    CatPlotter plotter;
    plotter.Construct(comp_vec, cat_labels, std::vector<TString>(), std::vector<float>(), "Punzi purity ratio", lumi);
    plotter.DrawLabel(name_a + " vs. " + name_b);
    plotter.GetStack() -> SetMinimum(1 - zoom_scale);
    plotter.GetStack() -> SetMaximum(1 + zoom_scale);
    plotter.Update();
    plotter.SaveAs(out_folder + STORAGE_PREFIX + "punzi_comp" + ".pdf");

    return(0);
}
