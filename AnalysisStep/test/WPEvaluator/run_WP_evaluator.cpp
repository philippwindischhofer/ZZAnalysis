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

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "Math/GSLSimAnMinimizer.h"

// use the local style for the histograms
#include <ZZAnalysis/AnalysisStep/test/classlib/include/PlottingUtils.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Classifier.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/CatPlotter.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/utils.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Tree.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/cuts.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/CompUtils.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/ConfigFileHandler.h>

#include <ZZAnalysis/AnalysisStep/interface/Category.h>

// compare (and optimize) the Punzi values evaluated on the signal sources (no background taken into account because of insufficient statistics in some categories)
#define OPTIMIZE_PUNZI_S

#ifdef OPTIMIZE_PUNZI_S
TString punzi_infile = "punzi_S_plot_hist.root";
TString punzi_histname = "punzi_S";
TString punzi_outfile = "punzi_S_comp";
#endif

#ifdef OPTIMIZE_PUNZI_SB
TString punzi_infile = "punzi_plot_hist.root";
TString punzi_histname = "punzi";
TString punzi_outfile = "punzi_comp";
#endif

TString punzi_hist_name = "punzi_purity";

TString refdir;
TString outdir;

// build the classifier whose parameters are going to be optimized
Classifier* varclass;
Mor18Classifier* varclass18;
Mor18Config* conf;

// evaluates the cost when using certain values for the working points
double costfunc(const double* params)
{
    // set the default values for the WPs (these are the same that are used in automatic mode *without* QG tagging)
    float WP_VBF2j = (float)params[0];
    float WP_VBF1j = (float)params[1];
    float WP_WHh = (float)params[2];
    float WP_ZHh = (float)params[3];

    varclass18 -> SetWPs(WP_VBF2j, WP_VBF1j, WP_WHh, WP_ZHh);

    std::cout << "WP_VBF2j = " << WP_VBF2j << std::endl;
    std::cout << "WP_VBF1j = " << WP_VBF1j << std::endl;
    std::cout << "WP_WHh = " << WP_WHh << std::endl;
    std::cout << "WP_ZHh = " << WP_ZHh << std::endl;

    // evaluate the Punzi value with this (modified) Classifier now
    PlottingUtils::make_punzi(kTRUE, varclass, outdir, punzi_histname, "no_cut_data", no_cut, conf, 0.0, 0.5, false);
    
    // load now the Punzi histogram of the optimized classifier and compare the two. From this point onwards, is exactly the same as in "Comp"
    float zoom_scale = 1.0;
    float cost = CompUtils::compare_punzi(outdir, refdir, "Mor18_{opt}", "Mor18", conf -> storage_prefix() + punzi_infile, punzi_hist_name, outdir, conf -> storage_prefix() + punzi_outfile, zoom_scale, conf);

    std::cout << "cost = " << cost << std::endl;

    return cost;
}

int main( int argc, char *argv[] )
{
    if(argc != 8)
    {
    	std::cerr << "Error: exactly 7 arguments are required" << std::endl;
    }

    // set default values
    float WP_VBF2j = 0.46386;
    float WP_VBF1j = 0.37605;
    float WP_WHh = 0.88384;
    float WP_ZHh = 0.91315;
    
    refdir = argv[1];
    outdir = argv[2];
    float lumi = std::stof(argv[3]);

    WP_VBF2j = std::stof(argv[4]);
    WP_VBF1j = std::stof(argv[5]);
    WP_WHh = std::stof(argv[6]);
    WP_ZHh = std::stof(argv[7]);

    varclass = new Mor18Classifier();
    varclass18 = static_cast<Mor18Classifier*>(varclass);

#ifdef OPTIMIZE_PUNZI_SB
    conf = new Mor18Config("/data_CMS/cms/wind/CJLST_NTuples/", lumi, true);
#endif

#ifdef OPTIMIZE_PUNZI_S
    conf = new Mor18Config("/data_CMS/cms/wind/CJLST_NTuples/", lumi, false);
#endif

    double params[] = {WP_VBF2j, WP_VBF1j, WP_WHh, WP_ZHh};

    costfunc(params);
   
    return(0);
}
