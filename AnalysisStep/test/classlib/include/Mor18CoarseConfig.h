#ifndef Mor18CoarseConfig_h
#define Mor18CoarseConfig_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "TString.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>

class Mor18CoarseConfig: public Mor18Config
{
public:
    virtual std::vector<TString> signal_source_labels();
    virtual std::vector<int> signal_source_styles();

    virtual std::vector<TString> signal_hist_names();
    virtual std::vector<int> signal_source_colors();

    virtual std::vector<TString> background_source_labels();
    virtual std::vector<int> background_source_styles();

    virtual std::vector<TString> background_hist_names();
    virtual std::vector<int> background_source_colors();
    virtual int hist_index(TString desc);
};

#endif
