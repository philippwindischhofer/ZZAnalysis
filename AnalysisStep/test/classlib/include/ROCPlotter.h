#ifndef ROCPlotter_h
#define ROCPlotter_h

// C++
#include <iostream>
#include <fstream>
#include <string>

#include <boost/range/combine.hpp>

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
#include "TGraph.h"
#include "TGaxis.h"
#include "TMultiGraph.h"
#include "TMarker.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Tree.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/ROCGenerator.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/EventStream.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Discriminant.h>

class ROCPlotter: public Tree
{
public:
    ROCPlotter(Config& conf, float start_fraction, float end_fraction);
    ~ROCPlotter();

    void AddROCCurve(Discriminant* disc, TString H0_desc, TString H1_desc, TString disc_name);

    void AddROCCurve(Discriminant* disc, TString H0_desc, TString H1_desc, TString disc_name, std::vector<float> marker_thresholds);

    // this is the intermediate-level signature
    void AddROCCurve(EventStream* H0_stream, EventStream* H1_stream, const std::function<float(Tree*)>& disc, TString H0_desc, TString H1_desc, TString disc_name, std::vector<float> marker_thresholds);

    // this is the low-level signature
    void AddROCCurve(std::vector<TString> H0_files, std::vector<TString> H1_files, const std::function<float(Tree*)>& disc, const std::function<bool(Tree*)>& cut, TString H0_desc, TString H1_desc, TString disc_name, std::vector<float> marker_thresholds);

    std::vector<float> GetAUC();

    void Construct();
    void SetLabel(TString label_left, TString label_right);
    void SaveAs(TString path);

private:
    float lumi;

    float start_fraction;
    float end_fraction;

    TFile* input_file;
    TTree* input_tree;

    TH1F* hCounters;
    Long64_t n_gen_events;
    Long64_t gen_sum_weights;

    std::vector<float> disc_values;
    std::vector<float> weight_values;
    std::vector<bool> true_values;

    std::vector<TGraph*> graphs;
    std::vector<float> auc;
    std::vector<TString> names;
    std::vector<TMarker*> markers;

    std::vector<int> ROC_colors = {
	    kBlue - 9,
	    kGreen - 6,
	    kRed - 7,
	    kYellow - 7,
	    kCyan - 6
    };

    std::vector<int> marker_styles = {
	20, 26, 22, 21
    };

    TCanvas* canv;
    TPad* pad;
    TLegend* leg;
    TMultiGraph* tmg;

    TString xlabel;
    TString ylabel;
    TString label_left;
    TString label_right;

    void IterateThroughFile(TString input_file_name, bool truth, const std::function<float(Tree*)>& disc, const std::function<bool(Tree*)>& cut);
    void Redraw();
    void Reset();
};

#endif
