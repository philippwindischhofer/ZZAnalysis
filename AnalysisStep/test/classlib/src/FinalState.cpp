#include <ZZAnalysis/AnalysisStep/test/classlib/include/FinalState.h>

int FinalState::FindFinalStateZX(Tree* in)
{
    int final_state = -999;
   
    if(in -> Z1Flav == -121)
    {
	if(in -> Z2Flav == +121)
	    final_state = Settings::fs4e;
	else if(in -> Z2Flav == +169)
	    final_state = Settings::fs2e2mu;
    }
    else if(in -> Z1Flav == -169)
    {
	if(in -> Z2Flav == +121)
	    final_state = Settings::fs2mu2e;
	else if(in -> Z2Flav == +169)
	    final_state = Settings::fs4mu;
    }
   
    return final_state;
}
