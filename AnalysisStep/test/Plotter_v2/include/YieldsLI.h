#ifndef YieldsLI_h
#define YieldsLI_h

// C++
#include <iostream>
#include <fstream>
#include <iomanip> // For setprecision
#include <vector>
#include <map>
#include <cstdlib>

// ROOT
#include "TApplication.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TGraphAsymmErrors.h"
#include "TMultiGraph.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TColor.h"

// Include classes
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Tree.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/ConfigFileHandler.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18LIClassifier.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Classifier.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/FakeRates.h>

#include "Histograms.h"
#include "Yields.h"
#include <ZZAnalysis/AnalysisStep/interface/Category.h>
#include <ZZAnalysis/AnalysisStep/interface/FinalStates.h>
#include <ZZAnalysis/AnalysisStep/interface/bitops.h>
#include <ZZAnalysis/AnalysisStep/interface/cConstants.h>
#include <ZZAnalysis/AnalysisStep/interface/Discriminants.h>

// BOOLS
#define APPLY_K_FACTORS 1
#define MERGE_2E2MU 1

using namespace std;

class YieldsLI: public Yields
{

public:

   YieldsLI( double );
   ~YieldsLI();

   void SetPackagePath(TString, TString);
   
   void MakeHistograms( TString );
   void Calculate_SS_ZX_Yields( TString, TString );

private:
   TString config_file_path;
   TString calibration_dir;
   TString priors_file_path;
   
   Classifier* refclass;
};

#endif
