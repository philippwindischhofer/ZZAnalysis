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

TString name_a = "Mor18_{LI}";
TString name_b = "Mor18  (experimental)";

// TString name_a = "Mor18_{OPT}";
// TString name_b = "Mor18";

float zoom_scale = 1.0;

TString punzi_hist_name = "punzi_purity";
TString punzi_infile = "punzi_plot_hist.root";
TString punzi_outfile = "punzi_comp";

int main( int argc, char *argv[] )
{
    if(argc < 4)
    {
	std::cerr << "Error: at least 3 arguments are required" << std::endl;
	return(-1);
    }

    TString indir_a = argv[1];
    TString indir_b = argv[2];
    TString out_folder = argv[3];

    if(argc == 5)
    {
	zoom_scale = std::stof(argv[4]);
    }

    // choose the category set that is used on both sides of the comparison
    Mor18Config* conf = new Mor18Config();
    //Mor18ConfigReducedCategorySet* conf = new Mor18ConfigReducedCategorySet();

    CompUtils::compare_punzi(indir_a, indir_b, name_a, name_b, conf -> storage_prefix() + punzi_infile, punzi_hist_name, out_folder, conf -> storage_prefix() + punzi_outfile, zoom_scale, conf);

    std::cout << "done" << std::endl;

    return(0);
}
