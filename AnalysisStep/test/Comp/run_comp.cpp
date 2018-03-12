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

#include <ZZAnalysis/AnalysisStep/test/Comp/include/comp_utils.h>
#include <ZZAnalysis/AnalysisStep/test/Comp/include/histo_settings.h>

// global definitions
TString indir_a = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/LikBenchmarkerPlots/";
TString indir_b = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlots/";

TString name_a = "LikClass";
TString name_b = "Mor18";

TString out_folder = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/CompPlots/";

float zoom_scale = 0.05;

TString punzi_hist_name = "punzi_purity";
TString punzi_infile = "punzi_plot_hist.root";
TString punzi_outfile = "punzi_comp";

int main( int argc, char *argv[] )
{
    compare_punzi(indir_a, indir_b, name_a, name_b, STORAGE_PREFIX + punzi_infile, punzi_hist_name, out_folder, STORAGE_PREFIX + punzi_outfile, zoom_scale);

    return(0);
}
