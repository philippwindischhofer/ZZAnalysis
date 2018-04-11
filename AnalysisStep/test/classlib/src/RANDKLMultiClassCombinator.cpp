#include <ZZAnalysis/AnalysisStep/test/classlib/include/RANDKLMultiClassCombinator.h>

RANDKLMultiClassCombinator::RANDKLMultiClassCombinator(bool use_KL): use_KL(use_KL)
{
    std::srand ( unsigned ( std::time(0) ) );

    // set the default parameters
    engine_parameters["min_iterations"] = 10;
    engine_parameters["max_iterations"] = 5000;
}

RANDKLMultiClassCombinator::~RANDKLMultiClassCombinator()
{

}

void RANDKLMultiClassCombinator::UseFlatPriorsInKL(bool use_flat_priors)
{
    this -> use_flat_priors = use_flat_priors;
}

std::map<TString, float> RANDKLMultiClassCombinator::Evaluate(Tree* in, DiscriminantCollection* coll)
{
    int min_iterations = engine_parameters["min_iterations"];
    int max_iterations = engine_parameters["max_iterations"];

    // try the low-resolution mode first
    std::map<TString, float> retval = Evaluate(in, coll, min_iterations);
    float margin = GetWinningMargin();

    //std::cout << "margin = " << margin << std::endl;

    if(margin < min_iterations / 2)
    {
	// win was not very pronounced, switch to high-resolution mode
	retval = Evaluate(in, coll, max_iterations);
	// std::cout << "margin (high res) = " << GetWinningMargin() << std::endl;
	// for(auto cur: retval)
	//     std::cout << cur.first << " : " << cur.second << std::endl;
    }

    return retval;
}

std::map<TString, float> RANDKLMultiClassCombinator::Evaluate(Tree* in, DiscriminantCollection* coll, int iterations)
{
    std::map<TString, float> retval;

    // repeat the tournament several times, because of the intransitivity
    for(int i = 0; i < iterations; i++)
    {
	// first get the list of categories that are playing against each other
	std::vector<TString> categories = coll -> GetCategories();

	// start with a random category order
	std::random_shuffle(categories.begin(), categories.end());

	// std::cout << "playing with category order ";
	// for(auto cat: categories)
	// {
	//     std::cout << cat << " ";
	// }

	// std::cout << std::endl;

	// now have pairs of categories play against each other, in the random order decided upon above
	TString cur_winner = categories.at(0);
	for(unsigned int cat = 1; cat < categories.size(); cat++)
	{
	    // have cur_winner play against the next one in the line
	    TString player = categories.at(cat);
	    
	    std::pair<TString, TString> combination = std::make_pair(cur_winner, player);
	    float log_LR = coll -> EvaluateLog(combination, in);
	    float KL_corr = use_KL ? coll -> EvaluateKLCorrection(combination, in, use_flat_priors) : 0.0;
	    float game_result = log_LR + KL_corr;

	    //std::cout << combination.first << " vs. " << combination.second << ": log(LR) = " << log_LR << ", KL_corr = " << KL_corr << std::endl;

	    // 'player' wins, if the 'game_result' is negative, i.e. a likelihood ratio < 1 is computed
	    if(game_result < 0.0)
	    {
		cur_winner = player;
	    }
	    
	    //std::cout << cur_winner << " won in this game (" << game_result << ")" << std::endl;
	}

	//std::cout << cur_winner << " is the tournament winner" << std::endl;

	// 'cur_winner' now holds the tournament winner for this round
	retval[cur_winner]++;
    }
    
    last_result = retval;

    // std::cout << " --------------------------------" << std::endl;

    // for(auto res: last_result)
    // {
    // 	std::cout << res.first << ": " << res.second << std::endl;;
    // }

    // std::cout << " --------------------------------" << std::endl;

    return retval;
}
