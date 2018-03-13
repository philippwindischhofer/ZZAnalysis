#ifndef utils_h
#define utils_h

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
#include "TMath.h"

#include "Tree.h"

// to save and load back the histograms
void save_histos(TString file, std::vector<TH1F*>& hist_vec);
std::vector<TH1F*> read_histos(TString file, std::vector<TString> hist_names);

// to renormalize a collection of histograms such that they represent fractions (i.e. the sum over the set of each bin will add up to 1)
std::vector<float> get_category_sums(std::vector<TH1F*> hist_vec);
std::vector<float> renormalize_histograms(std::vector<TH1F*> hist_vec);

// returns the total number of events contained in the given histogram
float get_total_events(TH1F* hist);

// some metrics to quantify purity of the categorization

// computes the Punzi metric for a specific category (i.e. histogram bin)
float get_punzi_purity(float B_cat, float S_cat, float S_tot, float a, float b);

#endif
