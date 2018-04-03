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
    if(argc != 3)
    {
	std::cerr << "Error: exactly 2 arguments are required" << std::endl;
	return(-1);
    }

    TString MCpath = argv[1];
    TString out_path = argv[2];

    Mor18Config conf(MCpath);

    //TString out_path = "../../src/ZZAnalysis/ROCs/";

    DiscriminantCollection* ME_coll = MEDiscriminantFactory::GenerateRawDiscriminantCollectionSequentialCut(conf, false);
    DiscriminantCollection* ME_QG_coll = MEDiscriminantFactory::GenerateRawDiscriminantCollectionSequentialCut(conf, true);
    DiscriminantCollection* ML_coll = MLDiscriminantFactory::GenerateMorCompatibleDiscriminantCollection("no_dir", conf);

    // Discriminant* ME_disc = ME_coll -> GetDiscriminant(std::make_pair("VBF2jH125", "ggH125"));
    // Discriminant* ME_QG_disc = ME_QG_coll -> GetDiscriminant(std::make_pair("VBF2jH125", "ggH125"));
    // Discriminant* ML_disc = ML_coll -> GetDiscriminant(std::make_pair("VBFH125", "ggH125"));

    auto ME_discs = ME_coll -> GetDiscs();

    for(auto& ME_disc: ME_discs)
    {
	std::pair<TString, TString> combination = ME_disc.first;
	Discriminant* cur_ME_disc = ME_disc.second;
	Discriminant* cur_ME_QG_disc = ME_QG_coll -> GetDiscriminant(combination);
	Discriminant* cur_ML_disc = ML_coll -> GetDiscriminant(combination);

	// make sure to only use the second half of each file, i.e. the portion reserved for the validation data!
	float start_fraction = 0.0;
	float end_fraction = 1.0;
	ROCPlotter rp(conf, start_fraction, end_fraction); 

	rp.AddROCCurve(cur_ME_disc,
		       combination.second + " efficiency",
		       combination.first + " efficiency",
		       cur_ME_disc -> GetDiscriminantName()
	    );
    
	rp.AddROCCurve(cur_ME_QG_disc,
		       combination.second + " efficiency",
		       combination.first + " efficiency",
		       cur_ME_QG_disc -> GetDiscriminantName()
	    );

	rp.AddROCCurve(cur_ML_disc,
		       combination.second + " efficiency",
		       combination.first + " efficiency",
		       cur_ML_disc -> GetDiscriminantName()
	    );

	rp.Construct();

	if(combination.first == "VBF1j")
	    rp.SetLabel("nCleanedJetsPt30 == 1, ME: not selected upstream", "");
	else
	    rp.SetLabel("nCleanedJetsPt30 >= 2, ME: not selected upstream", "");

	auto AUCs = rp.GetAUC();

	for(auto AUC: AUCs)
	    std::cout << AUC << std::endl;
    
	rp.SaveAs(out_path + "ROC_" + combination.first + "_" + combination.second + ".pdf");
    }

    return(0);
}
