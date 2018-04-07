#include <ZZAnalysis/AnalysisStep/test/classlib/include/ConfigFileHandler.h>

ConfigFileHandler::ConfigFileHandler(TString filepath, TString mode)
{
    if(mode == "write")
    {
	outfile.open(filepath);
    }
    else if(mode == "read")
    {
	infile.open(filepath);
    }
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

float ConfigFileHandler::GetField(TString field)
{
    float retval = 0.0;

    std::string line;
    std::string identifier = field.Data();
    identifier += " = ";

    infile.clear();
    infile.seekg(0);

    while(std::getline(infile, line))
    {
	if(!line.compare(0, identifier.size(), identifier))
	{
	    retval = atof(line.substr(identifier.size()).c_str());
	    break;
	}
    }

    return retval;
}
