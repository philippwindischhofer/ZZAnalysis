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
    if(argc < 7)
    {
	std::cerr << "Error: at least 6 arguments are required" << std::endl;
	return(-1);
    }

    TString calibration_folder = argv[1];
    TString MCpath = argv[2];
    float mass_point = std::atof(argv[3]);
    TString out_folder = argv[4];

    float start_fraction = std::atof(argv[5]);
    float end_fraction = std::atof(argv[6]);

    TString engine = "rand_KL";
    if(argc >= 8)
    {
	engine = argv[7];
    }

    TString config_path = calibration_folder + "../settings.conf";
    Classifier* refclass = new Mor18LIClassifier(calibration_folder, config_path, engine);
    Mor18LIClassifier* refclass18 = static_cast<Mor18LIClassifier*>(refclass);
	
    // put intermediate-quality engine settings
    refclass18 -> SetEngineParameter("min_iterations", 50);
    refclass18 -> SetEngineParameter("max_iterations", 150);

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
    float bkg_prior = 1.0;

    if(argc == 9)
    {
	// // the path to the prior file was given -> read the priors that were optimized on the training dataset, and evaluate the classification performance on the validation dataset
	TString prior_path = argv[8];
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
	bkg_prior = handler -> GetField("bkg_prior");
    }

    std::cout << "-------------------------------------------------------" << std::endl;
    std::cout << "start_fraction = " << start_fraction << std::endl;
    std::cout << "end_fraction = " << end_fraction << std::endl;
    std::cout << "mass_point = " << mass_point << std::endl;

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
    std::cout << "bkg_prior = " << bkg_prior << std::endl;

    std::cout << "-------------------------------------------------------" << std::endl;

    refclass18 -> SetPriors(VBF_prior, ggH_prior, WHhadr_prior, ZHhadr_prior, WHlept_prior, ZHlept_prior, ZHMET_prior, ttHhadr_prior, ttHlept_prior, bkg_prior);

    Mor18Config* conf = new Mor18Config(MCpath, 41.37, true, mass_point);
    Mor18Config* conf_S = new Mor18Config(MCpath, 41.37, false, mass_point);

    // generate all plots on the validation dataset only! (i.e. the second half of each datafile)
    PlottingUtils::make_SB_barchart(kTRUE, refclass, out_folder, "categorization_SB", "no_cut_data", "105 GeV < m_{4#font[12]{l}} < 140 GeV", mZZ_cut, conf, start_fraction, end_fraction, false);
    PlottingUtils::make_S_barchart(kFALSE, refclass, out_folder, "categorization_S", "no_cut_data", "105 GeV < m_{4#font[12]{l}} < 140 GeV", mZZ_cut, conf, start_fraction, end_fraction, false);
    PlottingUtils::make_punzi(kFALSE, refclass, out_folder, "punzi", "no_cut_data", mZZ_cut, conf, start_fraction, end_fraction, false);
    PlottingUtils::make_SBfine_ratio(kFALSE, refclass, out_folder, "SB_fine", "no_cut_data", mZZ_cut, conf, start_fraction, end_fraction, false);

    PlottingUtils::make_punzi(kTRUE, refclass, out_folder, "punzi_S", "no_cut_data_S", mZZ_cut, conf_S, start_fraction, end_fraction, false);

    std::cout << "done" << std::endl;

    return(0);
}
