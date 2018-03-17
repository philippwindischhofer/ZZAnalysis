#ifndef utils_h
#define utils_h

// C++
#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <boost/range/combine.hpp>

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

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Config.h>

// to save and load back the histograms
std::vector<TH1F*> read_histos(TString file, std::vector<TString> hist_names);
std::map<TString, TH1F*> read_histmap(TString file, std::vector<TString> hist_names);

void save_histmap(TString file, std::map<TString, TH1F*> histos);
void save_histos(TString file, std::vector<TH1F*>& hist_vec);

std::map<int, float> get_bin_sums(std::map<TString, TH1F*> histmap, Config* conf);

// to renormalize a collection of histograms such that they represent fractions (i.e. the sum over the set of each bin will add up to 1)
std::vector<float> get_category_sums(std::vector<TH1F*> hist_vec);
std::vector<float> renormalize_histograms(std::vector<TH1F*> hist_vec);
std::vector<float> unitize_histmap(std::map<TString, TH1F*> histmap);

// returns the total number of events contained in the given histogram
float get_total_events(TH1F* hist);

// some metrics to quantify purity of the categorization

// computes the Punzi metric for a specific category (i.e. histogram bin)
float get_punzi_purity(float B_cat, float S_cat, float S_tot, float a, float b);

#endif
