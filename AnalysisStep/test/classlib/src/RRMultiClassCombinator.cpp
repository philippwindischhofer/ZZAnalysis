#include <ZZAnalysis/AnalysisStep/test/classlib/include/RRMultiClassCombinator.h>

RRMultiClassCombinator::RRMultiClassCombinator()
{

}

RRMultiClassCombinator::~RRMultiClassCombinator()
{

}

std::map<TString, float> RRMultiClassCombinator::Evaluate(Tree* in, DiscriminantCollection* coll)
{

    //std::cout << " -------------------------------------------------- " << std::endl;

    // start with the full tournament:
    std::map<TString, float> retval = Evaluate(in, coll, coll -> GetCategories());
    std::vector<TString> winners = GetWinningCategories();

    // if there is a unique winner, return the score of the full tournament, otherwise, repeat the tournament only using the winners themselves
    // std::cout << "winners of the full round: " << std::endl;
    // for(auto cur_winner: winners)
    // {
    // 	std::cout << cur_winner << std::endl;
    // }

    if(winners.size() > 1)
    {
	//std::cout << "need to arbitrate" << std::endl;

	std::vector<TString> playing_categories;

	do
	{
	    // the winners from the previous step play now
	    playing_categories = winners;
	    // std::cout << "now playing: " << std::endl;
	    // for(auto cur: playing_categories)
	    // 	std::cout << cur << std::endl;

	    retval = Evaluate(in, coll, playing_categories);
	    winners = GetWinningCategories();

	    // std::cout << "retval: " << std::endl;
	    // for(auto cur: retval)
	    // 	std::cout << cur.first << " : " << cur.second << std::endl;

	    // std::cout << "winners: " << std::endl;
	    // for(auto cur: winners)
	    // 	std::cout << cur << std::endl;
	}
	while(winners.size() < playing_categories.size() && winners.size() > 1);

	// detect if there was an intransitive loop, i.e. if all players win exactly once
	if(winners.size() == playing_categories.size())
	{
	    retval.clear();

	    //std::cout << "intransitive loop detected, try to pick winner from priors" << std::endl;

	    //while(1);
	    
	    float max_prior = 0.0;
	    for(auto cur: winners)
	    {
		float cur_prior = coll -> EvaluatePrior(cur);
		if(cur_prior > max_prior)
		    max_prior = cur_prior;
	    }

	    std::vector<TString> winners_prior;

	    for(auto cur: winners)
	    {
		if(coll -> EvaluatePrior(cur) == max_prior)
		{
		    winners_prior.push_back(cur);
		}
	    }

	    TString chosen_winner;
	    if(winners_prior.size() > 1)
	    {
		//std::cout << "try to pick winner randomly" << std::endl;
		// choose one category from the loop at random and return it as the winner
		std::random_device random_device;
		std::mt19937 engine{random_device()};
		std::uniform_int_distribution<int> dist(0, winners.size() - 1);
		
		chosen_winner = winners[dist(engine)];
	    }
	    else
	    {
		chosen_winner = winners_prior[0];
	    }

	    retval[chosen_winner] = 1.0;
	}

	// std::cout << "final retval: " << std::endl;
	// for(auto& cur: retval)
	// {
	//     std::cout << cur.first << " : " << cur.second << std::endl;
	// }
    }

    // otherwise, this contains the winner already
    return retval;
}

// perform a round-robin tournament on the given categories
std::map<TString, float> RRMultiClassCombinator::Evaluate(Tree* in, DiscriminantCollection* coll, std::vector<TString> categories)
{
    std::map<TString, float> retval;

    for(auto player_a = categories.begin(); player_a != categories.end() - 1; player_a++)
    {
	for(auto player_b = player_a + 1; player_b != categories.end(); player_b++) 
	{
	    //std::cout << "playing " << *player_a << " vs. " << *player_b << std::endl;
	    std::pair<TString, TString> category_pair = std::make_pair(*player_a, *player_b);

	    float LR = coll -> Evaluate(category_pair, in);

	    if(LR > 1.0)
	    {
		retval[category_pair.first]++;
		//std::cout << category_pair.first << " won" << std::endl;
	    }
	    else
	    {
		retval[category_pair.second]++;
		//std::cout << category_pair.second << " won" << std::endl;
	    }
	}
    }    

    last_result = retval;
    return retval;
}
