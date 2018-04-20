#ifndef ConfigFileUtils_h
#define ConfigFileUtils_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <functional>
#include <algorithm>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "TString.h"

class ConfigFileUtils
{
public:
    static std::map<TString, TString> ParsePythonDict(std::string raw);
};

#endif
