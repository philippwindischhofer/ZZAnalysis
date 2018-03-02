#ifndef Classifier_h
#define Classifier_h
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

#include "Tree.h"

class Classifier
{
public:
    virtual void FillHistogram(TString input_file_name, float lumi, TH1F* hist, const std::function<int(Tree*)>& cut) { };
};

#endif
