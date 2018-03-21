// C++
#include <iostream>
#include <fstream>

// ROOT
#include "TApplication.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TStyle.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/CompUtils.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18ConfigReducedCategorySet.h>

// global definitions
TString indir_a = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlotsLIReducedCategorySet/";
TString indir_b = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlotsReferenceReducedCategorySet/";

// TString indir_a = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlotsOptMor18/";
// TString indir_b = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlotsReferenceForOptimization/";

// TString name_a = "Mor18_{LI}";
// TString name_b = "Mor18    (simple voting, likelihood only)";

TString name_a = "Mor18_{LI}";
TString name_b = "Mor18  (experimental)  [nCleanedJetsPt30 >= 1]";

TString out_folder = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/CompPlots/";

float zoom_scale = 0.1;

TString punzi_hist_name = "punzi_purity";
TString punzi_infile = "punzi_plot_hist.root";
TString punzi_outfile = "punzi_comp";

int main( int argc, char *argv[] )
{
    //Mor18Config* conf = new Mor18Config();
    Mor18ConfigReducedCategorySet* conf = new Mor18ConfigReducedCategorySet();

    CompUtils::compare_punzi(indir_a, indir_b, name_a, name_b, conf -> storage_prefix() + punzi_infile, punzi_hist_name, out_folder, conf -> storage_prefix() + punzi_outfile, zoom_scale, conf);

    std::cout << "done" << std::endl;

    return(0);
}
