#ifndef Mor18Config_h
#define Mor18Config_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "TString.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Config.h>

class Mor18Config: public Config
{
public:
    std::vector<TString> cat_labels();
    std::vector<TString> cat_labels_text();
    int bin_index(TString desc);
    TString storage_prefix();
    TString id();
};

#endif
