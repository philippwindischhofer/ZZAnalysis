#ifndef ConfigFileUtils_h
#define ConfigFileUtils_h

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <utility>
#include <functional>
#include <algorithm>

#include "TString.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/cuts.h>

class ConfigFileUtils
{
public:
    static std::map<TString, TString> ParsePythonDict(std::string raw);
    static std::vector<TString> ParsePythonList(std::string raw);
    static std::function<bool(Tree*)> ParsePythonCut(std::string raw);
    static std::function<bool(Tree*)> ParsePythonJetCut(std::string raw);

    static std::vector<std::string> Split(std::string raw, std::string delimiter);
};

#endif
