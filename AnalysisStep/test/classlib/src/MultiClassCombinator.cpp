#include <ZZAnalysis/AnalysisStep/test/classlib/include/MultiClassCombinator.h>

MultiClassCombinator::MultiClassCombinator()
{ }

MultiClassCombinator::~MultiClassCombinator()
{ }

// default behavior is to do nothing when asked for a classification
std::map<TString, float> MultiClassCombinator::Evaluate(Tree* in, DiscriminantCollection* coll)
{
    std::map<TString, float> retval;

    return retval;
}

TString MultiClassCombinator::GetWinningCategory()
{
    // look at the stored last_result and find the category that scored highest
    return find_max(last_result);
}

float MultiClassCombinator::GetWinningMargin()
{
    std::map<TString, float> last_result_copy(last_result);

    TString winner = find_max(last_result_copy);
    float winner_score = last_result_copy[winner];
    last_result_copy[winner] = std::numeric_limits<float>::min();

    TString next_winner = find_max(last_result_copy);
    float next_winner_score = last_result_copy[next_winner];

    return winner_score - next_winner_score;
}

TString MultiClassCombinator::find_max(std::map<TString, float> mapping)
{
    float max = std::numeric_limits<float>::min();
    TString argmax = "no_cat";

    for(auto& cur: mapping)
    {
	if(cur.second > max)
	{
	    max = cur.second;
	    argmax = cur.first;
	}
    }

    return argmax;
}

