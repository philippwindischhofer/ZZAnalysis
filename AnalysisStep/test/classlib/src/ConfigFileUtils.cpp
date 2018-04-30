#include <ZZAnalysis/AnalysisStep/test/classlib/include/ConfigFileUtils.h>

std::map<TString, TString> ConfigFileUtils::ParsePythonDict(std::string raw)
{
    std::map<TString, TString> retval;

    std::vector<std::string> keyvals = ConfigFileUtils::Split(raw, ",");

    for(auto& keyval: keyvals)
    {
	std::vector<std::string> entry = ConfigFileUtils::Split(keyval, ": ");
	
	TString key(entry[0].c_str());
	TString value(entry[1].c_str());

	retval[key] = value;	
    }

    return retval;
}

std::vector<TString> ConfigFileUtils::ParsePythonList(std::string raw)
{
    std::vector<TString> retval;

    std::vector<std::string> entries = ConfigFileUtils::Split(raw, "; ");

    for(auto& entry: entries)
    {
	retval.push_back(entry);
    }

    return retval;
}

std::vector<std::string> ConfigFileUtils::Split(std::string raw, std::string delimiters)
{
    std::vector<std::string> split_components;

    const char* delimiters_str = delimiters.c_str();

    char* raw_str = strdup(raw.c_str());
    char* pch;

    pch = strtok(raw_str, delimiters_str);
    while(pch != NULL)
    {
	std::string split_component(pch);
	split_components.push_back(split_component);

	pch = strtok(NULL, delimiters_str);
    }

    return split_components;
}

std::function<bool(Tree*)> ConfigFileUtils::ParsePythonJetCut(std::string raw)
{
    if(raw.find("[\"nCleanedJetsPt30\"] >= 2") != string::npos)    
    {
	auto j2cut = [&](Tree* in) -> bool {
	    return ((in -> nCleanedJetsPt30 >= 2) ? kTRUE : kFALSE);
	};
	
	std::cout << "j2cut" << std::endl;

	return j2cut;
    }
    else if(raw.find("[\"nCleanedJetsPt30\"] == 1") != string::npos)
    {
	auto j1cut = [&](Tree* in) -> bool {
	    return ((in -> nCleanedJetsPt30 == 1) ? kTRUE : kFALSE);
	};

	std::cout << "j1cut" << std::endl;

	return j1cut;
    }
    else if(raw.find("[\"nCleanedJetsPt30\"] == 0") != string::npos)
    {
	auto j0cut = [&](Tree* in) -> bool {
	    return ((in -> nCleanedJetsPt30 == 0) ? kTRUE : kFALSE);
	};

	std::cout << "j0cut" << std::endl;
	
	return j0cut;
    }
    else if(raw.find("[\"nCleanedJetsPt30\"] < 2") != string::npos)
    {
	auto lj2cut = [&](Tree* in) -> bool {
	    return ((in -> nCleanedJetsPt30 < 2) ? kTRUE : kFALSE);
	};

	std::cout << "j01cut" << std::endl;
	
	return lj2cut;
    }

    return no_cut;    
}

std::function<bool(Tree*)> ConfigFileUtils::ParsePythonCut(std::string raw)
{
    if(raw.find("mZZ_cut") != string::npos)
    {
	std::cout << "mZZ_cut" << std::endl;
	return mZZ_cut;
    }
    else if(raw.find("WHhadr_cut") != string::npos)
    {
	std::cout << "WHhadr_cut" << std::endl;
	return extraLeptons_0_cut;
    }
    else if(raw.find("ZHhadr_cut") != string::npos)
    {
	std::cout << "ZHhadr_cut" << std::endl;
	return extraNeutrinos_0_Leptons_0_cut;
    }
    else if(raw.find("WHlept_cut") != string::npos)
    {
	std::cout << "WHlept_cut" << std::endl;
	return extraLeptons_1_cut;
    }
    else if(raw.find("ZHlept_cut") != string::npos)
    {
	std::cout << "ZHlept_cut" << std::endl;
	return extraLeptons_2_cut;
    }
    else if(raw.find("ZHMET_cut") != string::npos)
    {
	std::cout << "ZHMET_cut" << std::endl;
	return extraNeutrinos_2_cut;
    }
    else if(raw.find("ttHlept_cut") != string::npos)
    {
	std::cout << "ttHlept_cut" << std::endl;
	return extraLeptons_12_cut;
    }
    else if(raw.find("ttHhadr_cut") != string::npos)
    {
	std::cout << "ttHhadr_cut" << std::endl;
	return extraLeptons_0_cut;
    }
    else if(raw.find("no_cut") != string::npos)
    {
	std::cout << "no_cut" << std::endl;
	return no_cut;
    }

    return no_cut;
}
