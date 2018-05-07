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

    TString MCpath = argv[1];
    TString out_folder = argv[2];
    float start_fraction = std::atof(argv[3]);
    float end_fraction = std::atof(argv[4]);
    float lumi = std::atof(argv[5]);
    float mass_point = std::atof(argv[6]);

    //TString MCpath = "/data_CMS/cms/wind/CJLST_NTuples_prepared/";
    //TString MCpath = "/data_CMS/cms/wind/CJLST_NTuples/";

    // switch here between the classifier based on trained discriminants or the one based only on the available MELA values
    Classifier* refclass = new Mor18Classifier();

    Mor18Config* conf = new Mor18Config(MCpath, lumi, true, mass_point);
    Mor18Config* conf_S = new Mor18Config(MCpath, lumi, false, mass_point);

    std::cout << "-------------------------------------------------------" << std::endl;
    std::cout << "start_fraction = " << start_fraction << std::endl;
    std::cout << "end_fraction = " << end_fraction << std::endl;

    if(argc == 8)
    {
	// the path to the prior file was given -> read the priors that were optimized on the training dataset, and evaluate the classification performance on the validation dataset
	TString WP_path = argv[7];
	ConfigFileHandler* handler = new ConfigFileHandler(WP_path, "read");
	float WP_VBF2j = handler -> GetField("WP_VBF2j");
	float WP_VBF1j = handler -> GetField("WP_VBF1j");
	float WP_WHh = handler -> GetField("WP_WHh");
	float WP_ZHh = handler -> GetField("WP_ZHh");

	std::cout << "using the following non-standard working points:" << std::endl;

	std::cout << "WP_VBF2j = " << WP_VBF2j << std::endl;
	std::cout << "WP_VBF1j = " << WP_VBF1j << std::endl;
	std::cout << "WP_WHh = " << WP_WHh << std::endl;
	std::cout << "WP_ZHh = " << WP_ZHh << std::endl;

	Mor18Classifier* refclass18 = static_cast<Mor18Classifier*>(refclass);
	refclass18 -> SetWPs(WP_VBF2j, WP_VBF1j, WP_WHh, WP_ZHh);
    }
    else
    {
	std::cout << "using default working points" << std::endl;
    }

    std::cout << "-------------------------------------------------------" << std::endl;    

    PlottingUtils::make_SB_barchart(kTRUE, refclass, out_folder, "categorization_SB", "no_cut_data", "105 GeV < m_{4#font[12]{l}} < 140 GeV", mZZ_cut, conf, start_fraction, end_fraction, false);
    PlottingUtils::make_S_barchart(kFALSE, refclass, out_folder, "categorization_S", "no_cut_data", "105 GeV < m_{4#font[12]{l}} < 140 GeV", mZZ_cut, conf, start_fraction, end_fraction, false);
    PlottingUtils::make_punzi(kFALSE, refclass, out_folder, "punzi", "no_cut_data", mZZ_cut, conf, start_fraction, end_fraction, false);
    PlottingUtils::make_SBfine_ratio(kFALSE, refclass, out_folder, "SB_fine", "no_cut_data", mZZ_cut, conf, start_fraction, end_fraction, false);

    PlottingUtils::make_punzi(kTRUE, refclass, out_folder, "punzi_S", "no_cut_data_S", mZZ_cut, conf_S, start_fraction, end_fraction, false);

    std::cout << "done" << std::endl;

    return(0);
}
