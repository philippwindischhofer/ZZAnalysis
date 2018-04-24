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

    void FillDataWeights(TString input_file_name, float lumi, const std::function<bool(Tree*)>& cut, const std::function<float(Tree*)>& var, std::vector<double>* data_vec, std::vector<double>* weight_vec, float start_fraction = 0.0, float end_fraction = 1.0);

    TH1F* FillProfile(TString input_file_name, float lumi, TH1F* hist, const std::function<bool(Tree*)>& cut, const std::function<float(Tree*)>& var, bool normalize, float start_fraction = 0.0, float end_fraction = 1.0, bool fast_reweighting = false, bool MC_weighting = true);

    void FillProfile(TString input_file_name, float lumi, TH2F* hist, const std::function<bool(Tree*)>& cut, const std::function<float(Tree*)>& var_x, const std::function<float(Tree*)>& var_y, bool normalize, float start_fraction = 0.0, float end_fraction = 1.0);

    void FillProfile(TString input_file_name, float lumi, TH3F* hist, const std::function<bool(Tree*)>& cut, const std::function<float(Tree*)>& var_x, const std::function<float(Tree*)>& var_y, const std::function<float(Tree*)>& var_z, bool normalize, float start_fraction = 0.0, float end_fraction = 1.0);

private:
    TFile* input_file;
    TTree* input_tree;

    TH1F* hCounters;
    Long64_t n_gen_events;
    Long64_t gen_sum_weights;

    void FillProfile(TString input_file_name, float lumi, TObject* hist, const std::function<bool(Tree*)>& cut, const std::function<void(TObject*, Tree*, float)>& fill_callback, float start_fraction = 0.0, float end_fraction = 1.0, bool fast_reweighting = false);
};

#endif
