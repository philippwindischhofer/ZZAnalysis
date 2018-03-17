#ifndef Config_h
#define Config_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <functional>

#include "TH1F.h"
#include "TString.h"
#include "Tree.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/cuts.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Routing.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/SignalAssignment.h>

class Config
{
public:
    Config();

    // the most fundamental quantity attached to a histogram is its name
    virtual std::vector<TString> hist_names();

    virtual std::vector<std::pair<TString, Routing*>> get_routing() { return routing; };
    virtual TString source_label(TString histname);
    virtual int source_color(TString histname);
    virtual int source_style(TString histname);

    virtual std::map<TString, TH1F*> generate_empty_histmap(int number_bins, float lower, float upper);

    // the most fundamental quantity attached to a categorization is the number (and names) of the individual categories
    virtual std::vector<int> categories() {return std::vector<int>(); };
    virtual int number_categories() {return 0; };

    virtual std::vector<TString> abstract_categories() {return std::vector<TString>();};
    virtual int number_abstract_categories() {return 0; };
    virtual int abs_cat_number(TString category) {return 0;};

    virtual TString abs_cat_label(TString abs_cat) {return "";};

    virtual std::vector<TString> cat_labels() { std::vector<TString> retval; return retval; };
    virtual std::vector<TString> cat_labels_text() { std::vector<TString> retval; return retval; };
    virtual int bin_number(int category) {return 0;};

    virtual std::vector<TString> ordered_cat_labels() { return std::vector<TString>(); };
    virtual int bin_index(TString desc) {return 0;};

    virtual std::vector<SignalAssignment*> signal_assignment() { return std::vector<SignalAssignment*>(); };
    virtual std::vector<std::pair<TString, int>> bin_assignment() {return std::vector<std::pair<TString, int>>(); };
    virtual std::vector<std::pair<TString, TString>> histogram_assignment() {return std::vector<std::pair<TString, TString>>();};

    virtual TString storage_prefix() { TString retval; return retval; };
    virtual TString id() { TString id; return id; };

    std::vector<TString> file_names();
    std::vector<TString> signal_file_names();
    std::vector<TString> signal_file_paths();
    std::vector<TString> file_paths();

    virtual std::vector<TString> signal_source_labels();
    virtual std::vector<int> signal_source_styles();
    virtual std::vector<TString> signal_source_labels_text();
    virtual std::vector<TString> signal_source_labels_merged_text();

    virtual std::vector<int> source_colors();
    virtual std::vector<int> source_styles();
    virtual std::vector<TString> source_labels();
    virtual std::vector<TString> signal_hist_names();
    virtual std::vector<int> signal_source_colors();

    std::vector<TString> background_file_names();
    std::vector<TString> background_file_paths();
    virtual std::vector<TString> background_source_labels();
    virtual std::vector<int> background_source_styles();
    virtual std::vector<TString> background_source_labels_text();

    virtual std::vector<TString> background_hist_names();
    virtual std::vector<int> background_source_colors();
    virtual int hist_index(TString desc);

    float lumi();
    TString MC_path();
    TString MC_filename();

protected:
    std::vector<std::pair<TString, Routing*>> routing;
};

#endif
