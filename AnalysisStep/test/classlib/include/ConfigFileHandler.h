#ifndef ConfigFileHandler_h
#define ConfigFileHandler_h

#include <iostream>
#include <fstream>
#include <string>

#include "TString.h"

class ConfigFileHandler
{
public:
    ConfigFileHandler(TString filepath);
    void AddSection(TString section);
    void AddField(TString section, float value);
    void SaveConfiguration();

private:
    std::ofstream outfile;
};

#endif
