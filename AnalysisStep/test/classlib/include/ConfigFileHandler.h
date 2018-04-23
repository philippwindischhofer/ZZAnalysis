#ifndef ConfigFileHandler_h
#define ConfigFileHandler_h

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include <boost/algorithm/string.hpp>

#include "TString.h"

class ConfigFileHandler
{
public:
    ConfigFileHandler(TString filepath, TString mode = "write");
    ~ConfigFileHandler();
    void AddSection(TString section);
    void AddField(TString section, float value);
    std::vector<TString> GetSections();
    void SaveConfiguration();
    float GetField(TString field);
    TString GetField(TString section, TString field);

private:
    std::ofstream outfile;
    std::ifstream infile;
};

#endif
