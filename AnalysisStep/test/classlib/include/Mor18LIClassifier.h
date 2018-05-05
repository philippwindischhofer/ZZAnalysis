#ifndef Mor18LIClassifier_h
#define Mor18LIClassifier_h

// C++
#include <iostream>
#include <fstream>
#include <string>

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
#include "Classifier.h"
#include "Tree.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/DiscriminantCollection.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/MEDiscriminantFactory.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/MLDiscriminantFactory.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/MLDiscriminantFactoryFullCategorySetDynamic.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/VotingMultiClassCombinator.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/RANDKLMultiClassCombinator.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/TreeKLMultiClassCombinator.h>

#include <ZZAnalysis/AnalysisStep/interface/Category.h>
#include <ZZAnalysis/AnalysisStep/interface/Discriminants.h>
#include <ZZAnalysis/AnalysisStep/interface/cConstants.h>

class Mor18LIClassifier: public Classifier
{
public:
    Mor18LIClassifier(TString calibration_folder, TString config_path, TString engine = "voting");
    ~Mor18LIClassifier();

    int ClassifyThisEvent(Tree* in);

    void SetPriors(float VBF_prior, float ggH_prior, float WHhadr_prior, float ZHhadr_prior, float WHlept_prior, float ZHlept_prior, float ZHMET_prior, float ttHhadr_prior, float ttHlept_prior, float bkg_prior);

    void SetEngineParameter(TString parameter_name, float parameter_value);

private:
    TString calibration_folder;
    TString config_path;

    DiscriminantCollection* coll;
    MultiClassCombinator* comb;

};

#endif
