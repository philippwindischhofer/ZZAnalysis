#ifndef SystematicsLI_h
#define SystematicsLI_h

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
#include "Systematics.h"
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Tree.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/ConfigFileHandler.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/ConfigFileUtils.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18LIClassifier.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Classifier.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/FakeRates.h>

class SystematicsLI: public Systematics
{
public:
    SystematicsLI();
    ~SystematicsLI();

    void SetPackagePath(TString, TString);

    void FillSystematics( TString );
    void FillSystematics_tuneUpDn( TString );

private:
    TString ChangeFolder(TString, TString);
    TString GetFolder(TString);

    TString config_file_path;
    TString calibration_dir;
    TString priors_file_path;

    Classifier* refclass;
};

#endif
