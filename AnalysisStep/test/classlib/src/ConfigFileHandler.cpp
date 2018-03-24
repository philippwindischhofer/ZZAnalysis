#include <ZZAnalysis/AnalysisStep/test/classlib/include/ConfigFileHandler.h>

ConfigFileHandler::ConfigFileHandler(TString filepath)
{
    outfile.open(filepath);
}

void ConfigFileHandler::AddSection(TString section)
{
    outfile << "[" << section << "]" << "\n";
}

void ConfigFileHandler::AddField(TString section, float value)
{
    outfile << section << " = " << std::to_string(value) << "\n";
}

void ConfigFileHandler::SaveConfiguration()
{
    outfile.close();
}
