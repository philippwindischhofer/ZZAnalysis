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

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "Math/GSLSimAnMinimizer.h"

// use the local style for the histograms
#include <ZZAnalysis/AnalysisStep/test/classlib/include/opt_utils.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Classifier.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/CatPlotter.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/utils.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Tree.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/cuts.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/comp_utils.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>

#include <ZZAnalysis/AnalysisStep/interface/Category.h>

TString punzi_hist_name = "punzi_purity";
TString punzi_infile = "punzi_plot_hist.root";
TString punzi_outfile = "punzi_comp";

// where all the files from the reference classifier are stored
TString refdir = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlotsReferenceForOptimization/";
TString outdir = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/OptimizerPlotsAllVariables2/";

int evalcnt = 0;

// build the classifier whose parameters are going to be optimized
Classifier* varclass = new Mor18Classifier();

Mor18Classifier* varclass18 = static_cast<Mor18Classifier*>(varclass);
Mor18Config* conf = new Mor18Config();

// evaluates the cost when using certain values for the working points
double costfunc(const double* params)
{
    // set the default values for the WPs (these are the same that are used in automatic mode *without* QG tagging)
    float WP_VBF2j = (float)params[0];
    float WP_VBF1j = (float)params[1];
    float WP_WHh = (float)params[2];
    float WP_ZHh = (float)params[3];
    varclass18 -> SetWPs(WP_VBF2j, WP_VBF1j, WP_WHh, WP_ZHh);

    std::cout << "-------------------------------------------------------" << std::endl;
    std::cout << "evaluation = " << evalcnt << std::endl;

    std::cout << "WP_VBF2j = " << WP_VBF2j << std::endl;
    std::cout << "WP_VBF1j = " << WP_VBF1j << std::endl;
    std::cout << "WP_WHh = " << WP_WHh << std::endl;
    std::cout << "WP_ZHh = " << WP_ZHh << std::endl;

    // evaluate the Punzi value with this (modified) Classifier now
    make_punzi(kTRUE, varclass, outdir, "punzi", "no_cut_data", no_cut, conf);
    
    // load low the Punzi histogram of the optimized classifier and compare the two. From this point onwards, is exactly the same as in "Comp"
    float zoom_scale = 1.0;
    float cost = compare_punzi(outdir, refdir, "Mor18_{opt}", "Mor18", conf -> storage_prefix() + punzi_infile, punzi_hist_name, outdir, conf -> storage_prefix() + punzi_outfile + Form("%i", evalcnt), zoom_scale, conf);

    std::cout << "cost = " << cost << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;

    evalcnt++;

    return cost;
}

int main( int argc, char *argv[] )
{
    // start the optimization with the currently used values
    float WP_VBF2j_init = 0.46386;
    float WP_VBF1j_init = 0.37605;
    float WP_WHh_init = 0.88384;
    float WP_ZHh_init = 0.91315;

    float var[4] = {WP_VBF2j_init, WP_VBF1j_init, WP_WHh_init, WP_ZHh_init};
    float step[4] = {0.01, 0.01, 0.01, 0.01};

    //ROOT::Math::Minimizer* min = ROOT::Math::Factory::CreateMinimizer("Minuit2", "");
    ROOT::Math::Minimizer* min = new ROOT::Math::GSLSimAnMinimizer();
    min -> SetMaxFunctionCalls(500);
    min -> SetMaxIterations(500);
    min -> SetTolerance(0.1);
    min -> SetPrintLevel(1);

    ROOT::Math::Functor f(&costfunc, 4);
    min -> SetFunction(f);

    min -> SetLimitedVariable(0, "WP_VBF2j", var[0], step[0], 0.0, 1.0);
    min -> SetLimitedVariable(1, "WP_VBF1j", var[1], step[1], 0.0, 1.0);
    // min -> SetFixedVariable(2, "WP_WHh", WP_WHh_init);
    // min -> SetFixedVariable(3, "WP_ZHh", WP_ZHh_init);

    min -> SetLimitedVariable(2, "WP_WHh", var[2], step[2], 0.0, 1.0);
    min -> SetLimitedVariable(3, "WP_ZHh", var[3], step[3], 0.0, 1.0);

    min -> Minimize();

    // these are the final parameters
    const double* res = min -> X();

    std::cout << "-------------------------------------------------------" << std::endl;    
    std::cout << "optimization finished!" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;    
    std::cout << "final parameter values: " << std::endl;

    std::cout << "WP_VBF2j = " << res[0] << std::endl;
    std::cout << "WP_VBF1j = " << res[1] << std::endl;
    std::cout << "WP_WHh = " << res[2] << std::endl;
    std::cout << "WP_ZHh = " << res[3] << std::endl;

    std::cout << "-------------------------------------------------------" << std::endl;        

    return(0);
}
