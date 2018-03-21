#include <ZZAnalysis/AnalysisStep/test/classlib/include/DiscriminantCollection.h>

float DiscriminantCollection::Evaluate(std::pair<TString, TString> combination, Tree* in)
{
    float retval = 0.0;

    // try if there exists a discriminant that can compare these two cases
    try
    {
	retval = discs.at(combination) -> Evaluate(in);
    }
    catch(const std::out_of_range& e)
    {
	try
	{
	    // in this case, perhaps the opposite direction exists
	    retval = discs.at(std::make_pair(combination.second, combination.first)) -> Evaluate(in);
	    retval = 1.0 / retval;
	}
	catch(const std::out_of_range& e)
	{
	    std::cerr << "requested discriminant does not exist!" << std::endl;
	}
    }

    return retval;
}

Discriminant* DiscriminantCollection::GetDiscriminant(std::pair<TString, TString> combination)
{
    return discs.at(combination);
}

void DiscriminantCollection::AddDiscriminant(std::pair<TString, TString> combination, Discriminant* disc)
{
    discs[combination] = disc;
}

std::map<std::pair<TString, TString>, Discriminant*> DiscriminantCollection::GetDiscs()
{
    return discs;
}

std::vector<std::pair<TString, TString>> DiscriminantCollection::GetCategoryPairs()
{
    std::vector<std::pair<TString, TString>> retval;

    for(auto& cur: discs)
    {
	retval.push_back(cur.first);
    }

    return retval;
}
