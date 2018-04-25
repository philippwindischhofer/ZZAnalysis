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
#include "TMath.h"
#include "TRandom3.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/ROCPlotter.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/cuts.h>

#include <ZZAnalysis/AnalysisStep/interface/Discriminants.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/DiscriminantCollection.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/MEDiscriminantFactory.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/MLDiscriminantFactory.h>

int main( int argc, char *argv[] )
{
    TString MCpath = "/data_CMS/cms/wind/CJLST_NTuples/";
    TString out_path = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/ROCMor18Optimization/";

    Mor18Config conf(MCpath);

    DiscriminantCollection* ME_coll = MEDiscriminantFactory::GenerateRawDiscriminantCollectionSequentialCut(conf, false);

    auto ME_discs = ME_coll -> GetDiscs();

    std::map<std::pair<TString, TString>, std::vector<float>> markers;

    std::vector<float> VBF2j_ggH_WPs = {0.46386, 0.627177, 0.637169};
    markers.insert(std::make_pair(std::make_pair("VBF2j", "ggH"), VBF2j_ggH_WPs));

    std::vector<float> VBF1j_ggH_WPs = {0.37605, 0.046854, 0.000042};
    markers.insert(std::make_pair(std::make_pair("VBF1j", "ggH"), VBF1j_ggH_WPs));

    std::vector<float> WHhadr_ggH_WPs = {0.88384, 0.948885, 0.955341};
    markers.insert(std::make_pair(std::make_pair("WH-hadr", "ggH"), WHhadr_ggH_WPs));

    std::vector<float> ZHhadr_ggH_WPs = {0.91315, 0.896969, 0.931576};
    markers.insert(std::make_pair(std::make_pair("ZH-hadr", "ggH"), ZHhadr_ggH_WPs));

    for(auto& ME_disc: ME_discs)
    {
	std::pair<TString, TString> combination = ME_disc.first;
	Discriminant* cur_ME_disc = ME_disc.second;

	// make sure to only use the second half of each file, i.e. the portion reserved for the validation data!
	float start_fraction = 0.0;
	float end_fraction = 1.0;
	ROCPlotter rp(conf, start_fraction, end_fraction); 

	rp.AddROCCurve(cur_ME_disc,
		       combination.second + " efficiency",
		       combination.first + " efficiency",
		       cur_ME_disc -> GetDiscriminantName(),
		       markers[combination]
	    );
    
	rp.Construct();

	if(combination.first == "VBF1j")
	    rp.SetLabel("nCleanedJetsPt30 == 1, not selected upstream", "");
	else
	    rp.SetLabel("nCleanedJetsPt30 >= 2, not selected upstream", "");

	auto AUCs = rp.GetAUC();

	for(auto AUC: AUCs)
	    std::cout << AUC << std::endl;
    
	rp.SaveAs(out_path + "ROC_" + combination.first + "_" + combination.second + ".pdf");
    }

    return(0);
}
