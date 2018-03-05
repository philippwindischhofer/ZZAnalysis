#ifndef Profiler_h
#define Profiler_h

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

class Profiler: public Tree
{
public:
    Profiler();
    ~Profiler();

    void FillProfile(TString input_file_name, float lumi, TH1F* hist, const std::function<bool(Tree*)>& cut, const std::function<float(Tree*)>& var);


private:
    TFile* input_file;
    TTree* input_tree;

    TH1F* hCounters;
    Long64_t n_gen_events;
    Long64_t gen_sum_weights;

};

#endif
