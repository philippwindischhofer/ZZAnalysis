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
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"

#include "Tree.h"

class Profiler: public Tree
{
public:
    Profiler();
    ~Profiler();

    void FillProfile(TString input_file_name, float lumi, TH1F* hist, const std::function<bool(Tree*)>& cut, const std::function<float(Tree*)>& var);

    void FillProfile(TString input_file_name, float lumi, TH2F* hist, const std::function<bool(Tree*)>& cut, const std::function<float(Tree*)>& var_x, const std::function<float(Tree*)>& var_y);

    void FillProfile(TString input_file_name, float lumi, TH3F* hist, const std::function<bool(Tree*)>& cut, const std::function<float(Tree*)>& var_x, const std::function<float(Tree*)>& var_y, const std::function<float(Tree*)>& var_z);

private:
    TFile* input_file;
    TTree* input_tree;

    TH1F* hCounters;
    Long64_t n_gen_events;
    Long64_t gen_sum_weights;

    void FillProfile(TString input_file_name, float lumi, TObject* hist, const std::function<bool(Tree*)>& cut, const std::function<void(TObject*, Tree*, float)>& fill_callback);
};

#endif
