#ifndef Mor18Config_h
#define Mor18Config_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <functional>
#include <algorithm>

#include "TString.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Routing.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/cuts.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/SignalAssignment.h>

#include <ZZAnalysis/AnalysisStep/interface/Category.h>

class Mor18Config: public Config
{
public:
    Mor18Config(TString MCpath = "/data_CMS/cms/wind/processed/");

    virtual std::vector<int> categories();
    virtual TString cat_label(int category);
    virtual TString cat_label_text(int category);

    virtual std::vector<TString> abstract_categories();
    virtual TString abs_cat_label(TString abs_cat);

    virtual std::vector<SignalAssignment*> signal_assignment();

    virtual TString storage_prefix();
    virtual TString id();
};

#endif
