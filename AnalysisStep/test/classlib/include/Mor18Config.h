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
    Mor18Config();

    virtual std::vector<int> categories();
    virtual int number_categories();

    virtual std::vector<TString> abstract_categories();
    virtual int number_abstract_categories();
    virtual int abs_cat_number(TString category);

    virtual TString cat_label(int category);
    virtual TString cat_label_text(int category);

    virtual TString abs_cat_label(TString abs_cat);

    virtual std::vector<TString> ordered_cat_labels();
    virtual int bin_number(int category);

    virtual std::vector<std::pair<TString, int>> bin_assignment();
    virtual std::vector<std::pair<TString, TString>> histogram_assignment();
    virtual std::vector<SignalAssignment*> signal_assignment();

    virtual std::vector<TString> cat_labels();
    virtual std::vector<TString> cat_labels_text();
    virtual int bin_index(TString desc);
    virtual TString storage_prefix();
    virtual TString id();

};

#endif
