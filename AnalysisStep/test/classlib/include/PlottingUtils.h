#ifndef PlottingUtils_h
#define PlottingUtils_h

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

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Classifier.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/CatPlotter.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/utils.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Tree.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/cuts.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Profiler.h>
#include <ZZAnalysis/AnalysisStep/interface/Category.h>

class PlottingUtils
{
public:

    static std::map<TString, TH1F*> generate_classifier_histmap(int fill_histos, Classifier* classifier, Config* conf, const std::function<bool(Tree*)>& ext_cut, TString data_id, TString out_path, float start_fraction = 0.0, float end_fraction = 1.0);

    static void make_S_barchart(int fill_histos, Classifier* classifier, TString out_folder, TString out_file, TString data_id, TString label, const std::function<bool(Tree*)>& cut, Config* conf, float start_fraction = 0.0, float end_fraction = 1.0);

    static void make_SB_barchart(int fill_histos, Classifier* classifier, TString out_folder, TString out_file, TString data_id, TString label, const std::function<bool(Tree*)>& cut, Config* conf, float start_fraction = 0.0, float end_fraction = 1.0);

    static void make_SBfine_ratio(int fill_histos, Classifier* classifier, TString out_folder, TString out_file, TString data_id, const std::function<bool(Tree*)>& cut, Config* conf, float start_fraction = 0.0, float end_fraction = 1.0);

    static void make_punzi(int fill_histos, Classifier* classifier, TString out_folder, TString out_file, TString data_id, const std::function<bool(Tree*)>& cut, Config* conf, float start_fraction = 0.0, float end_fraction = 1.0);

private:
    static std::map<TString, float> make_correct_events(int fill_histos, Classifier* classifier, TString out_folder, TString out_file, TString data_id, const std::function<bool(Tree*)>& cut, Config* conf, float start_fraction = 0.0, float end_fraction = 1.0);

    static std::map<TString, float> make_total_events(int fill_histos, Classifier* classifier, TString out_folder, TString out_file, TString data_id, const std::function<bool(Tree*)>& cut, Config* conf, float start_fraction = 0.0, float end_fraction = 1.0);

    static std::map<TString, float> make_desired_events(int fill_histos, Classifier* classifier, TString out_folder, TString out_file, TString data_id, const std::function<bool(Tree*)>& cut, Config* conf, float start_fraction = 0.0, float end_fraction = 1.0);

    static void make_barchart(std::map<TString, TH1F*> histmap, TString out_folder, TString out_file, TString label, Config* conf);

    static std::vector<TH1F*> generate_classifier_histvec(int fill_histos, Classifier* classifier, Config* conf, const std::function<bool(Tree*)>& ext_cut, TString data_id, TString out_path);
};

#endif
