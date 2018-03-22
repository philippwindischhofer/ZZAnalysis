// C++
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

// ROOT
#include "TApplication.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TStyle.h"
#include "THStack.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMath.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/PlottingUtils.h>

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18ConfigReducedCategorySet.h>

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Classifier.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18LIClassifier.h>

int main(int argc, char *argv[])
{
    if(argc != 4)
    {
	std::cerr << "Error: exactly 3 arguments are required" << std::endl;
	return(-1);
    }

//TString out_folder = "../../src/ZZAnalysis/BenchmarkerPlotsReferenceForOptimization/";

//TString out_folder = "../../src/ZZAnalysis/BenchmarkerPlotsReferenceReducedCategorySet/";
//TString out_folder = "../../src/ZZAnalysis/BenchmarkerPlotsLIReducedCategorySet/";

    TString calibration_folder = argv[1];
    TString MCpath = argv[2];
    TString out_folder = argv[3];

//    TString out_folder = "../../src/ZZAnalysis/BenchmarkerPlotsSandbox/";
//    TString out_folder = "../../src/ZZAnalysis/BenchmarkerPlotsOptMor18/";

//    Classifier* refclass = new Mor18Classifier();

    Classifier* refclass = new Mor18LIClassifier(calibration_folder);
    Mor18ConfigReducedCategorySet* conf = new Mor18ConfigReducedCategorySet(MCpath);

    // these are the optimized working points to give max. Punzi
    // float WP_VBF2j = 0.640951;
    // float WP_VBF1j = 0.00145712;
    // float WP_WHh = 0.952417;
    // float WP_ZHh = 0.931741;
    // Mor18Classifier* refclass18 = static_cast<Mor18Classifier*>(refclass);
    // refclass18 -> SetWPs(WP_VBF2j, WP_VBF1j, WP_WHh, WP_ZHh);

    // this jet cut is needed to make sure both classifiers (the standard Mor18 and the MELA-LI improved version) are operated in their allowed region
    auto jcut = [&](Tree* in) -> bool {
    	return (((in -> nCleanedJetsPt30 >= 1) ? kTRUE : kFALSE)) ? 
    	kTRUE : kFALSE;};

    PlottingUtils::make_SB_barchart(kTRUE, refclass, out_folder, "categorization_SB", "no_cut_data", "", jcut, conf, 0.5, 1.0);
    PlottingUtils::make_S_barchart(kFALSE, refclass, out_folder, "categorization_S", "no_cut_data", "", jcut, conf, 0.5, 1.0);
    PlottingUtils::make_punzi(kFALSE, refclass, out_folder, "punzi", "no_cut_data", jcut, conf, 0.5, 1.0);
    PlottingUtils::make_SBfine_ratio(kFALSE, refclass, out_folder, "SB_fine", "no_cut_data", jcut, conf, 0.5, 1.0);

    // PlottingUtils::make_SB_barchart(kTRUE, refclass, out_folder, "categorization_SB_4mu", "4mu_data", "ZZ #rightarrow 4#mu", final_state_4mu_cut, conf);
    // PlottingUtils::make_SB_barchart(kTRUE, refclass, out_folder, "categorization_SB_2mu2e", "2e2mu_data", "ZZ #rightarrow 2#mu2e", final_state_2e2mu_cut, conf);
    // PlottingUtils::make_SB_barchart(kTRUE, refclass, out_folder, "categorization_SB_4e", "4e_data", "ZZ #rightarrow 4e", final_state_4e_cut, conf);

    // PlottingUtils::make_S_barchart(kTRUE, refclass, out_folder, "categorization_S_4mu", "4mu_data", "ZZ #rightarrow 4#mu", final_state_4mu_cut, conf);
    // PlottingUtils::make_S_barchart(kTRUE, refclass, out_folder, "categorization_S_2mu2e", "2e2mu_data", "ZZ #rightarrow 2#mu2e", final_state_2e2mu_cut, conf);
    // PlottingUtils::make_S_barchart(kTRUE, refclass, out_folder, "categorization_S_4e", "4e_data", "ZZ #rightarrow 4e", final_state_4e_cut, conf);

    return(0);
}
