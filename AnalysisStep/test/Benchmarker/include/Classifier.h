#ifndef Classifier_h
#define Classifier_h

#include <iostream>

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

class Classifier: public Tree
{
public:
    Classifier();
    ~Classifier();

    void FillHistogram(TString input_file_name, float lumi, TH1F* hist, const std::function<bool(Tree*)>& cut);

    virtual int ClassifyEvent() { return -1; };

private:
    TFile* input_file;
    TTree* input_tree;

    TH1F* hCounters;
    Long64_t n_gen_events;
    Long64_t gen_sum_weights;

};

#endif
