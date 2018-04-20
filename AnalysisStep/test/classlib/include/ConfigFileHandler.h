#ifndef ConfigFileHandler_h
#define ConfigFileHandler_h

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include "TString.h"

class ConfigFileHandler
{
public:
    ConfigFileHandler(TString filepath, TString mode = "write");
    void AddSection(TString section);
    void AddField(TString section, float value);
    std::vector<TString> GetSections();
    void SaveConfiguration();
    float GetField(TString field);

private:
    std::ofstream outfile;
    std::ifstream infile;
};

#endif
