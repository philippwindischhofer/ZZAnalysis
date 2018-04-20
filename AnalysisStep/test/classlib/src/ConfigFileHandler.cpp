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

std::vector<TString> ConfigFileHandler::GetSections()
{
    std::vector<TString> retval;
    std::string line;
    
    infile.clear();
    infile.seekg(0);

    while(std::getline(infile, line))
    {
	// check if this line is a section header
	if((line.front() == '[') && (line.back() == ']'))
	{
	    // add it to the list of sections and strip away the section identifier brackets  
	    line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
	    std::string section_name = line.substr(1, line.length() - 2);

	    retval.push_back(section_name.c_str());
	}
    }    

    return retval;
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
