#ifndef opt_utils_h
#define opt_utils_h

// C++
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

#include <ZZAnalysis/AnalysisStep/test/Optimizer/include/histo_settings.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/Classifier.h>
#include <ZZAnalysis/AnalysisStep/interface/Category.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/CatPlotter.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/utils.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/Tree.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/cuts.h>

std::vector<TH1F*> generate_signal_histvec(int fill_histos, Classifier* classifier, const std::function<bool(Tree*)>& cut, TString data_id, TString local_path);
std::vector<TH1F*> generate_background_histvec(int fill_histos, Classifier* classifier, const std::function<bool(Tree*)>& cut, TString data_id, TString local_path);
void make_punzi(int fill_histos, Classifier* classifier, TString out_file, TString data_id, const std::function<bool(Tree*)>& cut, TString in_path, TString out_path);

#endif