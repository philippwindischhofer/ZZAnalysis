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
    Mor18Config conf;

    // make sure to only use the second half of each file, i.e. the portion reserved for the validation data!
    float start_fraction = 0.5;
    float end_fraction = 1.0;
    ROCPlotter rp(conf, start_fraction, end_fraction); 

    TString out_path = "../../src/ZZAnalysis/ROCs/";

    DiscriminantCollection* ME_coll = MEDiscriminantFactory::GenerateRawDiscriminantCollection(conf, false);
    DiscriminantCollection* ME_QG_coll = MEDiscriminantFactory::GenerateRawDiscriminantCollection(conf, true);
    DiscriminantCollection* ML_coll = MLDiscriminantFactory::GenerateDiscriminantCollection("no_dir", conf);

    Discriminant* ME_disc = ME_coll -> GetDiscriminant(std::make_pair("VBF2jH125", "ggH125"));
    Discriminant* ME_QG_disc = ME_QG_coll -> GetDiscriminant(std::make_pair("VBF2jH125", "ggH125"));
    Discriminant* ML_disc = ML_coll -> GetDiscriminant(std::make_pair("VBFH125", "ggH125"));

    rp.AddROCCurve(ME_disc,
		   "ggH efficiency",
    		   "VBF efficiency",
    		   "D_{VBF2j}^{ME}"
	);
    
    rp.AddROCCurve(ME_QG_disc,
    		   "ggH efficiency",
    		   "VBF efficiency",
    		   "D_{VBF2j}^{ME, QG}"
    	);

    rp.AddROCCurve(ML_disc,
    		   "ggH efficiency",
    		   "VBF efficiency",
    		   "D_{VBF2j}^{ML}"
    	);

    rp.Construct();
    rp.SetLabel("nCleanedJetsPt30 >= 2", "");

    auto AUCs = rp.GetAUC();

    for(auto AUC: AUCs)
	std::cout << AUC << std::endl;
    
    rp.SaveAs(out_path + "ROC_VBF.pdf");

    return(0);
}
