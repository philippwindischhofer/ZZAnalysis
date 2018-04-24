#include <ZZAnalysis/AnalysisStep/test/classlib/include/TreeKLMultiClassCombinator.h>

TreeKLMultiClassCombinator::TreeKLMultiClassCombinator(bool use_KL): use_KL(use_KL)
{
    std::srand ( unsigned ( std::time(0) ) );

    // set the default parameters
    engine_parameters["min_iterations"] = 10;
    engine_parameters["max_iterations"] = 5000;
}

TreeKLMultiClassCombinator::~TreeKLMultiClassCombinator()
{

}

void TreeKLMultiClassCombinator::UseFlatPriorsInKL(bool use_priors)
{
    this -> use_flat_priors = use_flat_priors;
}

std::map<TString, float> TreeKLMultiClassCombinator::Evaluate(Tree* in, DiscriminantCollection* coll)
{
    int min_iterations = engine_parameters["min_iterations"];
    int max_iterations = engine_parameters["max_iterations"];

    // std::cout << "min_iterations = " << min_iterations << std::endl;
    // std::cout << "max_iterations = " << max_iterations << std::endl;

    // try the low-resolution mode first
    std::map<TString, float> retval = Evaluate(in, coll, min_iterations);
    float margin = GetWinningMargin();

    //std::cout << "margin = " << margin << std::endl;

    if(margin < min_iterations / 2)
    {
	// win was not very pronounced, switch to high-resolution mode
	retval = Evaluate(in, coll, max_iterations);
    }

    return retval;
}

std::map<TString, float> TreeKLMultiClassCombinator::Evaluate(Tree* in, DiscriminantCollection* coll, int iterations)
{
    std::map<TString, float> retval;

    // repeat the tournament several times, because of the intransitivity
    for(int i = 0; i < iterations; i++)
    {
	// first get the list of categories that are playing against each other
	std::vector<TString> categories = coll -> GetCategories();

	// start with a random category order
	std::random_shuffle(categories.begin(), categories.end());

	// now have a randomly ordered list of classes, have them play in a tree-tournament
	TString winner = TreeTournament(categories, in, coll).at(0);
	retval[winner]++;
    }

    // std::cout << " --------------------------------" << std::endl;

    // for(auto cur: retval)
    // {
    // 	std::cout << cur.first << ": " << cur.second << std::endl;
    // }

    // std::cout << " --------------------------------" << std::endl;
    
    last_result = retval;
    return retval;
}

std::vector<TString> TreeKLMultiClassCombinator::TreeTournament(std::vector<TString> players, Tree* in, DiscriminantCollection* coll)
{
    std::vector<TString> retval;
    std::vector<TString> round_winning_players;

    // std::cout << "players: ";
    // for(auto cat: players)
    // {
    // 	std::cout << cat << " ";
    // }
    // std::cout << std::endl;

    if(players.size() > 1)
    {
	// there is still a nontrivial number of players left over in the game

	// invert the order of the players in the current round of the game (avoid that for an odd number of players, one player could reach the final round without ever playing once before)
	std::reverse(players.begin(), players.end());

	// std::cout << "players after reversing: ";
	// for(auto cat: players)
	// {
	//     std::cout << cat << " ";
	// }
	// std::cout << std::endl;

	// perform one round of the tournament
	for(unsigned int cur = 0; cur < players.size(); cur += 2)
	{
	    if(cur + 1 < players.size())
	    {
		//std::cout << "playing pair: " << cur << " / " << cur + 1 << std::endl;
		// can have these two players play against each other
		TString player_a = players.at(cur);
		TString player_b = players.at(cur + 1);

		std::pair<TString, TString> combination = std::make_pair(player_a, player_b);
		float log_LR = coll -> EvaluateLog(combination, in);
		float KL_corr = use_KL ? coll -> EvaluateKLCorrection(combination, in, use_flat_priors) : 0.0;
		float game_result = log_LR + KL_corr;

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
		//std::cout << "playing single: " << cur << std::endl;
		round_winning_players.push_back(players.at(cur));
	    }
	    //std::cout << "done" << std::endl;
	}

	// std::cout << "forwarding ";
	// for(auto cat: round_winning_players)
	// {
	//     std::cout << cat;
	// }
	// std::cout << " into the next round" << std::endl;

	// have the remaining players play among themselves to find the actual winner
	retval = TreeTournament(round_winning_players, in, coll);
    }
    else
    {
	// everything is actually already over,
	//std::cout << "tournament is over, winner: " << players.at(0) << std::endl;
	retval.push_back(players.at(0));
    }

    return retval;
}
