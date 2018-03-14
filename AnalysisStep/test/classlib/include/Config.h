#ifndef Config_h
#define Config_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "TString.h"

class Config
{
public:
    virtual std::vector<TString> cat_labels() { std::vector<TString> retval; return retval; };
    virtual std::vector<TString> cat_labels_text() { std::vector<TString> retval; return retval; };
    virtual int bin_index(TString desc) { return 0; };
    virtual TString storage_prefix() { TString retval; return retval; };
    virtual TString id() { TString id; return id; };

    std::vector<TString> file_names();
    std::vector<TString> signal_file_names();

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
};

#endif
