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

    // make sure to only use the second half of each file, i.e. the portion reserved for the validation data!
    float start_fraction = 0.5;
    float end_fraction = 1.0;
    ROCPlotter rp(conf, start_fraction, end_fraction); 

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

    auto disc2 = [&](Tree* in) -> float {
	return in -> D_VBF2j_ggH_ME;
    }; 

    auto cut2 = [&](Tree* in) -> bool {
	if(mZZ_cut(in) && (in -> nCleanedJetsPt30 >= 2))
	    return kTRUE;
	else
	    return kFALSE;
    };

    Discriminant* sample_disc = new Discriminant("insert_calib_dir_here");
    EventStream* H1Stream = new EventStream();
    H1Stream -> AddEventSource(H1_paths[0], cut2);

    EventStream* H0Stream = new EventStream();
    H0Stream -> AddEventSource(H0_paths[0], cut2);

    sample_disc -> SetH1Source(H1Stream);
    sample_disc -> SetH0Source(H0Stream);

    sample_disc -> AddComponent("VBF2j", no_cut, disc2);

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

    // rp.AddROCCurve(H0_paths,
    // 		   H1_paths,
    // 		   disc2,
    // 		   cut2,
    // 		   "ggH efficiency",
    // 		   "WBF efficiency",
    // 		   "D_{VBF2j}^{ME}"
    // 	);

    rp.AddROCCurve(sample_disc,
		   "ggH efficiency",
    		   "VBF efficiency",
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
	std::cout << AUC << std::endl;
    
    rp.SaveAs(out_path + "ROC_VBF.pdf");

    return(0);
}
