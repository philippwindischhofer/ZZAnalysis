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

// My own files
#define Mor18

#include <ZZAnalysis/AnalysisStep/test/classlib/include/SignalAssignment.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Classifier.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/opt_utils.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>
#include <ZZAnalysis/AnalysisStep/interface/Category.h>
#include <ZZAnalysis/AnalysisStep/test/Plotter_v2/src/setTDRStyle.cpp>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/CatPlotter.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/utils.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Tree.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/cuts.h>

#ifdef Mor17
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor17Classifier.h>
#endif

#ifdef Mor18
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Classifier.h>
//#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18LIClassifier.h>
#endif

int main(int argc, char *argv[])
{

//TString out_folder = "../../src/ZZAnalysis/BenchmarkerPlotsReferenceForOptimization/";
//TString out_folder = "../../src/ZZAnalysis/BenchmarkerPlotsLI/";
//TString out_folder = "../../src/ZZAnalysis/BenchmarkerPlotsReduced/";
    TString out_folder = "../../src/ZZAnalysis/BenchmarkerPlotsSandbox/";

#ifdef Mor17
    Classifier* refclass = new Mor17Classifier();
#endif

#ifdef Mor18
    //Classifier* refclass = new Mor18LIClassifier();
    Classifier* refclass = new Mor18Classifier();
#endif

    Mor18Config* conf = new Mor18Config();

    make_SB_barchart(kTRUE, refclass, out_folder, "categorization_SB", "no_cut_data", "", no_cut, conf);
    make_S_barchart(kFALSE, refclass, out_folder, "categorization_S", "no_cut_data", "", no_cut, conf);
    make_punzi(kFALSE, refclass, out_folder, "punzi", "no_cut_data", no_cut, conf);
    make_SBfine_ratio(kFALSE, refclass, out_folder, "SB_fine", "no_cut_data", no_cut, conf);

    make_SB_barchart(kTRUE, refclass, out_folder, "categorization_SB_4mu", "4mu_data", "ZZ #rightarrow 4#mu", final_state_4mu_cut, conf);
    make_SB_barchart(kTRUE, refclass, out_folder, "categorization_SB_2mu2e", "2e2mu_data", "ZZ #rightarrow 2#mu2e", final_state_2e2mu_cut, conf);
    make_SB_barchart(kTRUE, refclass, out_folder, "categorization_SB_4e", "4e_data", "ZZ #rightarrow 4e", final_state_4e_cut, conf);

    make_S_barchart(kTRUE, refclass, out_folder, "categorization_S_4mu", "4mu_data", "ZZ #rightarrow 4#mu", final_state_4mu_cut, conf);
    make_S_barchart(kTRUE, refclass, out_folder, "categorization_S_2mu2e", "2e2mu_data", "ZZ #rightarrow 2#mu2e", final_state_2e2mu_cut, conf);
    make_S_barchart(kTRUE, refclass, out_folder, "categorization_S_4e", "4e_data", "ZZ #rightarrow 4e", final_state_4e_cut, conf);

    return(0);
}
