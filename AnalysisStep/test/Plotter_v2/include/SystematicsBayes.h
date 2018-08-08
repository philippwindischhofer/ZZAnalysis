#ifndef SystematicsBayes_h
#define SystematicsBayes_h

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
#include <ZZAnalysis/AnalysisStep/test/classlib/include/BayesClassifier.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Classifier.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/FakeRates.h>

class SystematicsBayes: public Systematics
{
public:
    SystematicsBayes();
    ~SystematicsBayes();

    void SetPackagePath(TString, TString);

    void FillSystematics( TString );
    void FillSystematics_tuneUpDn( TString );

    void PrintSystematics_LEC(TString file);

private:
    // for the new contribution of the lepton energy scale
    std::vector<std::vector<float>> _expected_yield_LEC, _expected_yield_LEC_UP, _expected_yield_LEC_DN;
    int _current_category_LEC_UP, _current_category_LEC_DN;

    TString ChangeFolder(TString, TString);
    TString GetFolder(TString);

    TString config_file_path;
    TString calibration_dir;
    TString priors_file_path;

    Classifier* refclass;
};

#endif
