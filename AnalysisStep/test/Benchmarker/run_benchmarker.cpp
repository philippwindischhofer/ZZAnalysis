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
#include <ZZAnalysis/AnalysisStep/test/classlib/include/ConfigFileHandler.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/cuts.h>

int main(int argc, char *argv[])
{
    if(argc < 6)
    {
	std::cerr << "Error: at least 5 arguments are required" << std::endl;
	return(-1);
    }

    TString calibration_folder = argv[1];
    TString MCpath = argv[2];
    TString out_folder = argv[3];

    float start_fraction = std::atof(argv[4]);
    float end_fraction = std::atof(argv[5]);

    TString engine = "rand_KL";
    if(argc >= 7)
    {
	engine = argv[6];
    }

    // switch here between the classifier based on trained discriminants or the one based only on the available MELA values
    //Classifier* refclass = new Mor18Classifier();

    TString config_path = calibration_folder + "../settings.conf";
    Classifier* refclass = new Mor18LIClassifier(calibration_folder, config_path, engine);
    Mor18LIClassifier* refclass18 = static_cast<Mor18LIClassifier*>(refclass);
	
    // put intermediate-quality engine settings
    refclass18 -> SetEngineParameter("min_iterations", 50);
    refclass18 -> SetEngineParameter("max_iterations", 150);

    //priors based on event numbers determined from the training data
    // float VBF_prior = 1.0;
    // float ggH_prior = 1.76;
    // float WHhadr_prior = 0.55;
    // float ZHhadr_prior = 0.21;
    // float WHlept_prior = 0.27;
    // float ZHlept_prior = 0.06;
    // float ZHMET_prior = 0.066;
    // float ttHhadr_prior = 0.15;
    // float ttHlept_prior = 0.19;

    // optimized priors (used as default)
    // float VBF_prior = 1.0;
    // float ggH_prior = 1.46657;
    // float WHhadr_prior = 0.582676;
    // float ZHhadr_prior = 0.707539;
    // float WHlept_prior = 0.172772;
    // float ZHlept_prior = 0.0633419;
    // float ZHMET_prior = 0.0666923;
    // float ttHhadr_prior = 0.145215;
    // float ttHlept_prior = 0.1954;

    // flat priors (used at the beginning of the optimization)
    float VBF_prior = 1.0;
    float ggH_prior = 1.0;
    float WHhadr_prior = 1.0;
    float ZHhadr_prior = 1.0;
    float WHlept_prior = 1.0;
    float ZHlept_prior = 1.0;
    float ZHMET_prior = 1.0;
    float ttHhadr_prior = 1.0;
    float ttHlept_prior = 1.0;

    if(argc == 8)
    {
	// // the path to the prior file was given -> read the priors that were optimized on the training dataset, and evaluate the classification performance on the validation dataset
	TString prior_path = argv[7];
	ConfigFileHandler* handler = new ConfigFileHandler(prior_path, "read");
	VBF_prior = handler -> GetField("VBF_prior");
	ggH_prior = handler -> GetField("ggH_prior");
	WHhadr_prior = handler -> GetField("WHhadr_prior");
	ZHhadr_prior = handler -> GetField("ZHhadr_prior");
	WHlept_prior = handler -> GetField("WHlept_prior");
	ZHlept_prior = handler -> GetField("ZHlept_prior");
	ZHMET_prior = handler -> GetField("ZHMET_prior");
	ttHhadr_prior = handler -> GetField("ttHhadr_prior");
	ttHlept_prior = handler -> GetField("ttHlept_prior");
    }

    std::cout << "-------------------------------------------------------" << std::endl;
    std::cout << "start_fraction = " << start_fraction << std::endl;
    std::cout << "end_fraction = " << end_fraction << std::endl;

    std::cout << "using the following priors:" << std::endl;

    std::cout << "VBF_prior = " << VBF_prior << std::endl;
    std::cout << "ggH_prior = " << ggH_prior << std::endl;
    std::cout << "WHhadr_prior = " << WHhadr_prior << std::endl;
    std::cout << "ZHhadr_prior = " << ZHhadr_prior << std::endl;
    std::cout << "WHlept_prior = " << WHlept_prior << std::endl;
    std::cout << "ZHlept_prior = " << ZHlept_prior << std::endl;
    std::cout << "ZHMET_prior = " << ZHMET_prior << std::endl;
    std::cout << "ttHhadr_prior = " << ttHhadr_prior << std::endl;
    std::cout << "ttHlept_prior = " << ttHlept_prior << std::endl;

    std::cout << "-------------------------------------------------------" << std::endl;

    // // priors based on event weights
    // float VBF_prior = 1.0;
    // float ggH_prior = 11.45;
    // float WHhadr_prior = 0.20;
    // float ZHhadr_prior = 0.13;
    // float WHlept_prior = 0.09;
    // float ZHlept_prior = 0.04;
    // float ZHMET_prior = 0.04;
    // float ttHhadr_prior = 0.05;
    // float ttHlept_prior = 0.07;

    refclass18 -> SetPriors(VBF_prior, ggH_prior, WHhadr_prior, ZHhadr_prior, WHlept_prior, ZHlept_prior, ZHMET_prior, ttHhadr_prior, ttHlept_prior);

    // switch here between all Moriond 2018 categories and a reduced set for development
    //Mor18ConfigReducedCategorySet* conf = new Mor18ConfigReducedCategorySet(MCpath);
    Mor18Config* conf = new Mor18Config(MCpath, 35.9, true);
    Mor18Config* conf_S = new Mor18Config(MCpath, 35.9, false);
    //Mor18Config* conf = new Mor18Config(MCpath, 150.0, true);

    // these are the optimized working points to give max. Punzi for an integrated luminosity of 150fb^-1
    // float WP_VBF2j = 0.667281;
    // float WP_VBF1j = 0.000111881;
    // float WP_WHh = 0.970386;
    // float WP_ZHh = 0.932839;
    // Mor18Classifier* refclass18 = static_cast<Mor18Classifier*>(refclass);
    // refclass18 -> SetWPs(WP_VBF2j, WP_VBF1j, WP_WHh, WP_ZHh);

    // these are the optimized working points to give max. Punzi for an integrated luminosity of 35.9fb^-1
    // float WP_VBF2j = 0.640951;
    // float WP_VBF1j = 0.00145712;
    // float WP_WHh = 0.952417;
    // float WP_ZHh = 0.931741;
    // Mor18Classifier* refclass18 = static_cast<Mor18Classifier*>(refclass);
    // refclass18 -> SetWPs(WP_VBF2j, WP_VBF1j, WP_WHh, WP_ZHh);

    // these are the optimized working points to give max. Punzi for an integrated luminosity of 150fb^-1, done with global bayesian optimization
    // float WP_WHh = 0.843692261093;
    // float WP_VBF2j = 0.633562388718;
    // float WP_VBF1j = 0.0272457386188;
    // float WP_ZHh = 0.874365002746;

    // Mor18Classifier* refclass18 = static_cast<Mor18Classifier*>(refclass);
    // refclass18 -> SetWPs(WP_VBF2j, WP_VBF1j, WP_WHh, WP_ZHh);

    // these are the optimized working points to give max. Punzi for an integrated luminosity of 35.9fb^-1, done with global bayesian optimization
    // float WP_WHh = 0.849018646276;
    // float WP_VBF2j = 0.623974230702;
    // float WP_VBF1j = 0.0446782463634;
    // float WP_ZHh = 0.709247908423;

    //Mor18Classifier* refclass18 = static_cast<Mor18Classifier*>(refclass);
    //refclass18 -> SetWPs(WP_VBF2j, WP_VBF1j, WP_WHh, WP_ZHh);

    // these are the optimized working points to give max. Punzi for an integrated luminosity of 150fb^-1, done with simulated annealing and the saturating cost function
    // float WP_VBF2j = 0.637169;
    // float WP_VBF1j = 0.000042;
    // float WP_WHh = 0.955341;
    // float WP_ZHh = 0.931576;
    // Mor18Classifier* refclass18 = static_cast<Mor18Classifier*>(refclass);
    // refclass18 -> SetWPs(WP_VBF2j, WP_VBF1j, WP_WHh, WP_ZHh);

    // these are the optimized working points to give max. Punzi for an integrated luminosity of 35.9fb^-1, done with simulated annealing and the saturating cost function
    // float WP_VBF2j = 0.627177;
    // float WP_VBF1j = 0.046854;
    // float WP_WHh = 0.948885;
    // float WP_ZHh = 0.896969;
    // Mor18Classifier* refclass18 = static_cast<Mor18Classifier*>(refclass);
    // refclass18 -> SetWPs(WP_VBF2j, WP_VBF1j, WP_WHh, WP_ZHh);

    // this jet cut is needed to make sure both classifiers (the standard Mor18 and the MELA-LI improved version) are operated in their allowed region
    // auto jcut = [&](Tree* in) -> bool {
    // 	return (((in -> nCleanedJetsPt30 >= 1) ? kTRUE : kFALSE)) ? 
    // 	kTRUE : kFALSE;};

    // generate all plots on the validation dataset only! (i.e. the second half of each datafile)
    PlottingUtils::make_SB_barchart(kTRUE, refclass, out_folder, "categorization_SB", "no_cut_data", "", mZZ_cut, conf, start_fraction, end_fraction, false);
    PlottingUtils::make_S_barchart(kFALSE, refclass, out_folder, "categorization_S", "no_cut_data", "", mZZ_cut, conf, start_fraction, end_fraction, false);
    PlottingUtils::make_punzi(kFALSE, refclass, out_folder, "punzi", "no_cut_data", mZZ_cut, conf, start_fraction, end_fraction, false);
    PlottingUtils::make_SBfine_ratio(kFALSE, refclass, out_folder, "SB_fine", "no_cut_data", mZZ_cut, conf, start_fraction, end_fraction, false);

    PlottingUtils::make_punzi(kTRUE, refclass, out_folder, "punzi_S", "no_cut_data_S", mZZ_cut, conf_S, start_fraction, end_fraction, false);

    // PlottingUtils::make_SB_barchart(kTRUE, refclass, out_folder, "categorization_SB_4mu", "4mu_data", "ZZ #rightarrow 4#mu", final_state_4mu_cut, conf);
    // PlottingUtils::make_SB_barchart(kTRUE, refclass, out_folder, "categorization_SB_2mu2e", "2e2mu_data", "ZZ #rightarrow 2#mu2e", final_state_2e2mu_cut, conf);
    // PlottingUtils::make_SB_barchart(kTRUE, refclass, out_folder, "categorization_SB_4e", "4e_data", "ZZ #rightarrow 4e", final_state_4e_cut, conf);

    // PlottingUtils::make_S_barchart(kTRUE, refclass, out_folder, "categorization_S_4mu", "4mu_data", "ZZ #rightarrow 4#mu", final_state_4mu_cut, conf);
    // PlottingUtils::make_S_barchart(kTRUE, refclass, out_folder, "categorization_S_2mu2e", "2e2mu_data", "ZZ #rightarrow 2#mu2e", final_state_2e2mu_cut, conf);
    // PlottingUtils::make_S_barchart(kTRUE, refclass, out_folder, "categorization_S_4e", "4e_data", "ZZ #rightarrow 4e", final_state_4e_cut, conf);

    std::cout << "done" << std::endl;

    return(0);
}
