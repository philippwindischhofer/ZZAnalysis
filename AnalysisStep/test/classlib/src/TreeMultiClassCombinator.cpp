#include <ZZAnalysis/AnalysisStep/test/classlib/include/TreeMultiClassCombinator.h>

TreeMultiClassCombinator::TreeMultiClassCombinator()
{
    std::srand ( unsigned ( std::time(0) ) );

    // set the default parameters
    engine_parameters["min_iterations"] = 10;
    engine_parameters["max_iterations"] = 5000;
}

TreeMultiClassCombinator::~TreeMultiClassCombinator()
{

}

std::map<TString, float> TreeMultiClassCombinator::Evaluate(Tree* in, DiscriminantCollection* coll)
{
    int min_iterations = engine_parameters["min_iterations"];
    int max_iterations = engine_parameters["max_iterations"];

    // first get the list of categories that are playing against each other
    std::vector<TString> all_categories = coll -> GetCategories();

    // try the low-resolution mode first
    std::map<TString, float> retval = Evaluate(in, coll, all_categories, min_iterations);
    float margin = GetWinningMargin();

    if(margin < min_iterations / 2)
    {
	// win was not very pronounced, switch to high-resolution mode
	retval = Evaluate(in, coll, all_categories, max_iterations);
    }

    // if this enlarged tournament still did not bring about a winner, repeat the tournament strategy for the set of winners only
    std::vector<TString> winners = GetWinningCategories();

    if(winners.size() > 1)
    {
	std::vector<TString> playing_categories;

	do
	{
	    playing_categories = winners;
	    retval = Evaluate(in, coll, playing_categories, max_iterations);
	    winners = GetWinningCategories();
	}
	while(winners.size() < playing_categories.size() && winners.size() > 1);

	if(winners.size() == playing_categories.size())
	{
	    retval.clear();
	    
	    std::cout << "intransitive loop detected, try to pick winner from priors" << std::endl;
	    
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
		std::cout << "try to pick winner randomly" << std::endl;
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
    }
    
    return retval;
}

std::map<TString, float> TreeMultiClassCombinator::Evaluate(Tree* in, DiscriminantCollection* coll, std::vector<TString> categories, int iterations)
{
    std::map<TString, float> retval;

    // repeat the tournament several times, because of the intransitivity
    for(int i = 0; i < iterations; i++)
    {
     	// start with a random category order
	std::random_shuffle(categories.begin(), categories.end());

	// now have a randomly ordered list of classes, have them play in a tree-tournament
	TString winner = TreeTournament(categories, in, coll).at(0);
	retval[winner]++;
    }
    
    last_result = retval;
    return retval;
}

std::vector<TString> TreeMultiClassCombinator::TreeTournament(std::vector<TString> players, Tree* in, DiscriminantCollection* coll)
{
    std::vector<TString> retval;
    std::vector<TString> round_winning_players;

    if(players.size() > 1)
    {
	// there is still a nontrivial number of players left over in the game

	// invert the order of the players in the current round of the game (avoid that for an odd number of players, one player could reach the final round without ever playing once before)
	std::reverse(players.begin(), players.end());

	// perform one round of the tournament
	for(unsigned int cur = 0; cur < players.size(); cur += 2)
	{
	    if(cur + 1 < players.size())
	    {

		// can have these two players play against each other
		TString player_a = players.at(cur);
		TString player_b = players.at(cur + 1);

		std::pair<TString, TString> combination = std::make_pair(player_a, player_b);
		float log_LR = coll -> EvaluateLog(combination, in);
		float game_result = log_LR;

		if(game_result > 0.0)
		{
		    round_winning_players.push_back(player_a);
		}
		else
		{
		    round_winning_players.push_back(player_b);
		}
	    }
	    else
	    {
		// this is a lonely player that is left over in this round
		round_winning_players.push_back(players.at(cur));
	    }
	}

	// have the remaining players play among themselves to find the actual winner
	retval = TreeTournament(round_winning_players, in, coll);
    }
    else
    {
	// everything is actually already over,
	retval.push_back(players.at(0));
    }

    return retval;
}
