#ifndef Mor18Config_h
#define Mor18Config_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <functional>

#include "TString.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Routing.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/cuts.h>

class Mor18Config: public Config
{
public:
    Mor18Config();

    virtual std::vector<TString> cat_labels();
    virtual std::vector<TString> cat_labels_text();
    virtual int bin_index(TString desc);
    virtual TString storage_prefix();
    virtual TString id();

private:
    std::vector<std::pair<TString, Routing*>> routing;
};

#endif
