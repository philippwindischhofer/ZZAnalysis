#ifndef Config_h
#define Config_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <functional>

#include "TString.h"
#include "Tree.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/cuts.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Routing.h>

class Config
{
public:
    Config();

    virtual std::vector<TString> cat_labels() { std::vector<TString> retval; return retval; };
    virtual std::vector<TString> cat_labels_text() { std::vector<TString> retval; return retval; };
    virtual int bin_index(TString desc) { return 0; };
    virtual TString storage_prefix() { TString retval; return retval; };
    virtual TString id() { TString id; return id; };

    std::vector<TString> file_names();
    std::vector<TString> signal_file_names();
    std::vector<TString> signal_file_paths();
    std::vector<TString> file_paths();

    virtual std::vector<std::pair<TString, Routing*>> get_routing() { return routing; };
    virtual TString source_label(TString histname);
    virtual int source_color(TString histname);
    virtual int source_style(TString histname);

    virtual std::vector<TString> signal_source_labels();
    virtual std::vector<int> signal_source_styles();
    virtual std::vector<TString> signal_source_labels_text();
    virtual std::vector<TString> signal_source_labels_merged_text();

    virtual std::vector<TString> hist_names();
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

private:
    std::vector<std::pair<TString, Routing*>> routing;
};

#endif
