#ifndef Config_h
#define Config_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <functional>
#include <algorithm>

#include "TH1F.h"
#include "TString.h"
#include "Tree.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/cuts.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Routing.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/SignalAssignment.h>

class Config
{
public:
    // the most fundamental quantity attached to a histogram is its name
    virtual std::vector<TString> hist_names();
    virtual std::vector<TString> signal_hist_names();
    virtual std::vector<TString> background_hist_names();

    virtual TString source_label(TString histname);
    virtual int source_color(TString histname);
    virtual int source_style(TString histname);

    virtual std::map<TString, TH1F*> generate_empty_histmap(int number_bins, float lower, float upper);

    virtual std::vector<std::pair<TString, Routing*>> get_routing() {return routing;};

    // the most fundamental quantity attached to a categorization is the number (and names) of the individual categories
    virtual std::vector<int> categories() = 0;
    virtual TString cat_label(int category) = 0;
    virtual int number_categories() {return categories().size();};

    virtual std::vector<TString> abstract_categories() = 0;
    virtual int number_abstract_categories() {return abstract_categories().size();};
    virtual int abs_cat_number(TString category);
    virtual std::vector<TString> ordered_abs_cat_labels();

    virtual TString abs_cat_label(TString abs_cat) = 0;

    virtual int bin_number(int category);

    virtual std::vector<TString> ordered_cat_labels();

    virtual std::vector<SignalAssignment*> signal_assignment() = 0;
    virtual std::vector<std::pair<TString, int>> bin_assignment();
    virtual std::vector<std::pair<TString, TString>> histogram_assignment();

    virtual TString storage_prefix() = 0;
    virtual TString id() = 0;

    float lumi();
    TString MC_path();
    TString MC_filename();

protected:
    Config(TString MCpath = "/data_CMS/cms/wind/processed/");
    std::vector<std::pair<TString, Routing*>> routing;
    TString MCpath;
};

#endif
