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

// use the local style for the histograms
#include <ZZAnalysis/AnalysisStep/test/Optimizer/include/histo_settings.h>
#include <ZZAnalysis/AnalysisStep/test/Optimizer/include/opt_utils.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/Mor18Classifier.h>
#include <ZZAnalysis/AnalysisStep/interface/Category.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/CatPlotter.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/utils.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/Tree.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/cuts.h>

#define PUNZI_HIST_NAME "punzi_purity"
#define PUNZI_FILE "punzi_plot_hist.root"

// where all the files from the reference classifier are stored
TString refdir = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlots/";
TString outdir = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/OptimizerPlots/";

int main( int argc, char *argv[] )
{
    // load the punzi file for the reference classifier
    std::vector<TString> hist_name = {PUNZI_HIST_NAME};
	
    // need to load now the Punzi histograms that should be sitting in these two directories, then can make their ratio or compare them in some other way
    std::vector<TH1F*> punzi_ref = read_histos(refdir + STORAGE_PREFIX + PUNZI_FILE, hist_name);

    // build the classifier that is going to be optimized
    Classifier* varclass = new Mor18Classifier();

    Mor18Classifier* varclass18 = static_cast<Mor18Classifier*>(varclass);

    // set the default values for the WPs (these are the same that are used in automatic mode *without* QG tagging)
    float WP_VBF2j = 0.46386;
    float WP_VBF1j = 0.37605;
    float WP_WHh = 0.88384;
    float WP_ZHh = 0.91315;
    varclass18 -> SetWPs(WP_VBF2j, WP_VBF1j, WP_WHh, WP_ZHh);

    // evaluate the Punzi value with this (modified) Classifier now
    make_punzi(kTRUE, varclass, "punzi", "no_cut_data", no_cut, refdir, outdir);

    // load low the Punzi histogram of the optimized classifier and compare the two

    return(0);
}
