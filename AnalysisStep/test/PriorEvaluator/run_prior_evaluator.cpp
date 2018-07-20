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
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18LIClassifier.h>
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
TString punzi_infile;
TString punzi_histname;
TString punzi_outfile;

TString punzi_hist_name = "punzi_purity";

TString refdir;
TString outdir;

// build the classifier whose parameters are going to be optimized
Classifier* varclass;

Mor18LIClassifier* varclass18;
Mor18Config* conf;

// evaluates the cost when using certain values for the working points
double costfunc(const double* params)
{
    // set the default values for the WPs (these are the same that are used in automatic mode *without* QG tagging)
    float VBF_prior = (float)params[0];
    float ggH_prior = (float)params[1];
    float WHhadr_prior = (float)params[2];
    float ZHhadr_prior = (float)params[3];
    float WHlept_prior = (float)params[4];
    float ZHlept_prior = (float)params[5];
    float ZHMET_prior = (float)params[6];
    float ttHhadr_prior = (float)params[7];
    float ttHlept_prior = (float)params[8];
    float bkg_prior = (float)params[9];
    float qq_prior = (float)params[10];

    varclass18 -> SetPriors(VBF_prior, ggH_prior, WHhadr_prior, ZHhadr_prior, WHlept_prior, ZHlept_prior, ZHMET_prior, ttHhadr_prior, ttHlept_prior, bkg_prior, qq_prior);

    std::cout << "VBF_prior = " << VBF_prior << std::endl;
    std::cout << "ggH_prior = " << ggH_prior << std::endl;
    std::cout << "WHhadr_prior = " << WHhadr_prior << std::endl;
    std::cout << "ZHhadr_prior = " << ZHhadr_prior << std::endl;
    std::cout << "WHlept_prior = " << WHlept_prior << std::endl;
    std::cout << "ZHlept_prior = " << ZHlept_prior << std::endl;
    std::cout << "ZHMET_prior = " << ZHMET_prior << std::endl;
    std::cout << "ttHhadr_prior = " << ttHhadr_prior << std::endl;
    std::cout << "ttHlept_prior = " << ttHlept_prior << std::endl;
    std::cout << "bkg_prior = " << bkg_prior << std::endl;
    std::cout << "qq_prior = " << qq_prior << std::endl;

    // evaluate the Punzi value with this (modified) Classifier now
    // note: this computes Punzi in the total mass range - no mass cut at all is placed here --> therefore, the Punzi values themselves will differ from the ones produced by the Benchmarker
    PlottingUtils::make_punzi(kTRUE, varclass, outdir, punzi_histname, "no_cut_data", mZZ_cut, conf, 0.0, 1.0, false);
    
    // load low the Punzi histogram of the optimized classifier and compare the two. From this point onwards, is exactly the same as in "Comp"
    float zoom_scale = 1.0;
    float cost = CompUtils::compare_punzi(outdir, refdir, "Mor18_{opt}", "Mor18", conf -> storage_prefix() + punzi_infile, punzi_hist_name, outdir, conf -> storage_prefix() + punzi_outfile, zoom_scale, conf);

    std::cout << "cost = " << cost << std::endl;

    return cost;
}

int main( int argc, char *argv[] )
{
    if(argc != 18)
    {
    	std::cerr << "Error: exactly 17 arguments are required" << std::endl;
    }

    // set default values
    float VBF_prior = 1.0;
    float ggH_prior = 1.46657;
    float WHhadr_prior = 0.582676;
    float ZHhadr_prior = 0.707539;
    float WHlept_prior = 0.272772;
    float ZHlept_prior = 0.0633419;
    float ZHMET_prior = 0.0666923;
    float ttHhadr_prior = 0.145215;
    float ttHlept_prior = 0.1954;
    float bkg_prior = 1.0;
    float qq_prior = 1.0;
    
    TString run_dir = argv[1];
    outdir = argv[2];
    TString engine = argv[3];
    
    int min_iterations = std::atoi(argv[4]);
    int max_iterations = std::atoi(argv[5]);

    ggH_prior = std::stof(argv[6]);
    WHhadr_prior = std::stof(argv[7]);
    ZHhadr_prior = std::stof(argv[8]);
    WHlept_prior = std::stof(argv[9]);
    ZHlept_prior = std::stof(argv[10]);
    ZHMET_prior = std::stof(argv[11]);
    ttHhadr_prior = std::stof(argv[12]);
    ttHlept_prior = std::stof(argv[13]);
    bkg_prior = std::stof(argv[14]);
    qq_prior = std::stof(argv[15]);

    TString switchval = argv[16];
    refdir = argv[17];

    if(switchval == "S")
    {
	std::cout << "evaluating priors on S" << std::endl;

	punzi_infile = "punzi_S_plot_hist.root";
	punzi_histname = "punzi_S";
	punzi_outfile = "punzi_S_comp";

	conf = new Mor18Config(run_dir + "augmentation_validation/", 41.37, false);
    }
    else if(switchval == "SB")
    {
	std::cout << "evaluating priors on S + B" << std::endl;

	punzi_infile = "punzi_plot_hist.root";
	punzi_histname = "punzi";
	punzi_outfile = "punzi_comp";

	conf = new Mor18Config(run_dir + "augmentation_validation/", 41.37, true);
    }

    std::cout << "run_dir = " << run_dir << std::endl;
    std::cout << "ref_dir = " << refdir << std::endl;
    std::cout << "engine = " << engine << std::endl;
    std::cout << "out_dir = " << outdir << std::endl;
    std::cout << "min_iterations = " << min_iterations << std::endl;
    std::cout << "max_iteratins = " << max_iterations << std::endl;

    // use the corresponding benchmark scenario without optimized priors as reference
    // refdir = run_dir + "benchmark_" + engine + "_validation/";

    // can also try to use the validation reference of the legacy classifier as reference for the optimization
    // Note: this really just takes the *scalings*, i.e. the reference values for the achievable Punzi purity from the other classifier -> should be OK!
    // refdir = "/data_CMS/cms/wind/Mor18References/125/validation/";

    refdir += "/125/validation/";

    varclass = new Mor18LIClassifier(run_dir + "calibration_validation/", run_dir + "settings.conf", engine);

    varclass18 = static_cast<Mor18LIClassifier*>(varclass);

    // this sets the quality of the evaluation, i.e. its noise level
    varclass18 -> SetEngineParameter("min_iterations", min_iterations); // 25 is default
    varclass18 -> SetEngineParameter("max_iterations", max_iterations); // 100 is default

    double params[] = {VBF_prior, ggH_prior, WHhadr_prior, ZHhadr_prior, WHlept_prior, ZHlept_prior, ZHMET_prior, ttHhadr_prior, ttHlept_prior, bkg_prior, qq_prior};

    costfunc(params);
   
    return(0);
}
