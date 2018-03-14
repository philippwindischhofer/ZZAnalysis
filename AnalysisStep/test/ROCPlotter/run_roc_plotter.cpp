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

    std::vector<TString> files_a = {
	"ggH125"/*,
	"WplusH125", 
	"WminusH125", 
	"ZH125", 
	"ttH125", 
	"ZZTo4l",     
	"ggTo2e2mu_Contin_MCFM701",
	"ggTo2e2tau_Contin_MCFM701",
	"ggTo2mu2tau_Contin_MCFM701",
	"ggTo4e_Contin_MCFM701",
	"ggTo4mu_Contin_MCFM701",
	"ggTo4tau_Contin_MCFM701"*/
    };

    std::vector<TString> files_b = {
	// "WplusH125", 
	// "WminusH125"
	"VBFH125"
    };

    std::vector<TString> paths_a;
    std::vector<TString> paths_b;

    for(auto& file_a: files_a)
    {
	paths_a.push_back(conf.MC_path() + file_a + conf.MC_filename());
    }

    for(auto& file_b: files_b)
    {
	paths_b.push_back(conf.MC_path() + file_b + conf.MC_filename());
    }
    
    TString out_path = "../../src/ZZAnalysis/ROCs/";

    TRandom3* gen = new TRandom3();
    gen -> SetSeed(0);

    auto disc = [&](Tree* in) -> float {return gen -> Rndm();}; 

    auto disc2 = [&](Tree* in) -> float {
	return DVBF2j_ME(in -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal, in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, in -> ZZMass);
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

    rp.AddROCCurve(paths_a,
    		   paths_b,
    		   disc,
    		   cut2,
    		   "ggH efficiency",
    		   "VBF efficiency",
    		   "random"
    	);

    rp.AddROCCurve(paths_a,
    		   paths_b,
    		   disc2,
    		   cut2,
		   "ggH efficiency",
    		   "WBF efficiency",
		   "D_{VBF2j}^{ME}"
	);
    
    rp.AddROCCurve(paths_a,
    		   paths_b,
    		   disc3,
    		   cut2,
    		   "ggH efficiency",
    		   "VBF efficiency",
    		   "D_{VBF2j}^{ME, QG}"
    	);

    rp.Construct();
    
    std::cout << "before saving" << std::endl;

    rp.SaveAs(out_path + "ROC_VBF.pdf");

    return(0);
}
