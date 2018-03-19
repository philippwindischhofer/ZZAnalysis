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

int main( int argc, char *argv[] )
{
    Mor18Config conf;
    ROCPlotter rp(conf);

    // H0 files
    std::vector<TString> H0_files = {
	"ggH125"
    };

    // H1 files
    std::vector<TString> H1_files = {
	"VBFH125"
    };

    std::vector<TString> H0_paths;
    std::vector<TString> H1_paths;

    for(auto& H0_file: H0_files)
    {
	H0_paths.push_back(conf.MC_path() + H0_file + conf.MC_filename());
    }

    for(auto& H1_file: H1_files)
    {
	H1_paths.push_back(conf.MC_path() + H1_file + conf.MC_filename());
    }
    
    TString out_path = "../../src/ZZAnalysis/ROCs/";

    TRandom3* gen = new TRandom3();
    gen -> SetSeed(0);

    auto disc = [&](Tree* in) -> float {return gen -> Rndm();}; 

    auto disc2 = [&](Tree* in) -> float {
	return 2. * DVBF2j_ME(in -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal, in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, in -> ZZMass);
    }; 

    auto disc3 = [&](Tree* in) -> float {
    	float jetQGLikelihood[99];
    	float jetPhi[99];

    	for(int i = 0; i < in -> nCleanedJetsPt30; i++)
    	{
    	    jetQGLikelihood[i] = in -> JetQGLikelihood -> at(i);
    	    jetPhi[i] = in -> JetPhi -> at(i);
    	}

    	return DVBF2j_ME_QG(
    	    in -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal,
    	    in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
    	    in -> ZZMass,
    	    jetQGLikelihood,
    	    jetPhi);
    };

    // auto disc2 = [&](Tree* in) -> float {    
    // 	return DWHh_ME(
    // 	    in -> p_HadWH_SIG_ghw1_1_JHUGen_JECNominal,
    // 	    in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
    // 	    in -> p_HadWH_mavjj_JECNominal,
    // 	    in -> p_HadWH_mavjj_true_JECNominal,
    // 	    in -> ZZMass);
    // };

    // auto disc3 = [&](Tree* in) -> float {
    // 	float jetQGLikelihood[99];
    // 	float jetPhi[99];

    // 	for(int i = 0; i < in -> nCleanedJetsPt30; i++)
    // 	{
    // 	    jetQGLikelihood[i] = in -> JetQGLikelihood -> at(i);
    // 	    jetPhi[i] = in -> JetPhi -> at(i);
    // 	}

    // 	return DWHh_ME_QG(
    // 	    in -> p_HadWH_SIG_ghw1_1_JHUGen_JECNominal,
    // 	    in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
    // 	    in -> p_HadWH_mavjj_JECNominal,
    // 	    in -> p_HadWH_mavjj_true_JECNominal,
    // 	    in -> ZZMass,
    // 	    jetQGLikelihood,
    // 	    jetPhi
    // 	    );
    // };

    //auto cut = [&](Tree* in) -> bool {return mZZ_cut(in);};
    auto cut2 = [&](Tree* in) -> bool {
	if(mZZ_cut(in) && (in -> nCleanedJetsPt30 >= 2))
	    return kTRUE;
	else
	    return kFALSE;
    };

    rp.AddROCCurve(H0_paths,
    		   H1_paths,
    		   disc,
    		   cut2,
    		   "ggH efficiency",
    		   "VBF efficiency",
    		   "random"
    	);

    rp.AddROCCurve(H0_paths,
    		   H1_paths,
    		   disc2,
    		   cut2,
		   "ggH efficiency",
    		   "WBF efficiency",
		   "D_{VBF2j}^{ME}"
	);
    
    rp.AddROCCurve(H0_paths,
    		   H1_paths,
    		   disc3,
    		   cut2,
    		   "ggH efficiency",
    		   "VBF efficiency",
    		   "D_{VBF2j}^{ME, QG}"
    	);

    rp.Construct();

    auto AUCs = rp.GetAUC();

    for(auto AUC: AUCs)
    {
	std::cout << AUC << std::endl;
    }
    
    rp.SaveAs(out_path + "ROC_VBF.pdf");

    return(0);
}
