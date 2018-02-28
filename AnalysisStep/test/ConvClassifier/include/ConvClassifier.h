#ifndef ConvClassifier_h
#define ConvClassifier_h

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

#include "Tree.h"

class ConvClassifier: public Tree
{
public:
    ConvClassifier();
    ~ConvClassifier();

    /* returns the number of events put into each category (in the order prescribed by the selection)
     * 0 ... VBF-2jet tagged
     * 1 ... VH-hadronic tagged
     * 2 ... VH-leptonic tagged
     * 3 ... ttH tagged
     * 4 ... VH-ETmiss tagged
     * 5 ... VBF-1jet tagged
     * 6 ... untagged
     */
    void FillHistogram(TString input_file_name, float lumi, float xsec, TH1F* hist);

private:
    TFile* input_file;
    TTree* input_tree;

    TH1F* hCounters;
    Long64_t n_gen_events;
    Long64_t gen_sum_weights;

    int ClassifyEvent();

    int IsVBF2JetTagged();
    int IsVHHadronicTagged();
    int IsVHLeptonicTagged();
    int IsttHTagged();
    int IsVHEtMissTagged();
    int IsVBF1JetTagged();

    int NumberBTaggedJets();
};

#endif