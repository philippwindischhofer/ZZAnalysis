#include <ZZAnalysis/AnalysisStep/test/classlib/include/cuts.h>

// cuts that are used to separate the individual signal contributions
bool no_cut(Tree* in)
{
    return kTRUE;
}

bool mZZ_cut(Tree* in)
{
    if((in -> ZZMass > 118.) && (in -> ZZMass < 130.))
	return kTRUE;
    else
	return kFALSE;
}

int countAssocLeptons(Tree* in)
{
    if((in -> GenAssocLep1Id == 0) && (in -> GenAssocLep2Id == 0))
    {
	return 0;
    }
    else if( ((in -> GenAssocLep1Id != 0) && (in -> GenAssocLep2Id == 0)) ||
	     ((in -> GenAssocLep1Id == 0) && (in -> GenAssocLep2Id != 0)) )
    {
	return 1;
    }
    else if((in -> GenAssocLep1Id != 0) && (in -> GenAssocLep2Id != 0))
    {
	return 2;
    }

    return 0;
}

bool extraLeptons_0_cut(Tree* in)
{
    if(/*mZZ_cut(in) && */(countAssocLeptons(in) == 0))
	return kTRUE;
    else
	return kFALSE;
}

bool extraLeptons_1_cut(Tree* in)
{
    if(/*mZZ_cut(in) && */(countAssocLeptons(in) == 1))
	return kTRUE;
    else
	return kFALSE;
}

bool extraLeptons_2_cut(Tree* in)
{
    if(/*mZZ_cut(in) && */(countAssocLeptons(in) == 2))
	return kTRUE;
    else
	return kFALSE;
}

bool extraLeptons_12_cut(Tree* in)
{
    if(extraLeptons_1_cut(in) || extraLeptons_2_cut(in))
	return kTRUE;
    else
	return kFALSE;
}

int countNeutrinos(Tree* in)
{
   int number_neutrinos = 0;

   if(in -> LHEAssociatedParticleId)
   {
       for(unsigned int i = 0; i < (in -> LHEAssociatedParticleId -> size()); i++)
       {
	   short pdg_code = in -> LHEAssociatedParticleId -> at(i);
	   if((TMath::Abs(pdg_code) == 12) || (TMath::Abs(pdg_code) == 14) || (TMath::Abs(pdg_code) == 16))
	       number_neutrinos++;
       }
   }

   return number_neutrinos;
}

bool extraNeutrinos_0_cut(Tree* in)
{
    if(/*mZZ_cut(in) && */countNeutrinos(in) == 0)
	return kTRUE;
    else
	return kFALSE;
}

bool extraNeutrinos_2_cut(Tree* in)
{
    if(/*mZZ_cut(in) && */countNeutrinos(in) == 2)
	return kTRUE;
    else
	return kFALSE;
}

bool extraNeutrinos_0_Leptons_0_cut(Tree* in)
{
    if(/*mZZ_cut(in) && */(countAssocLeptons(in) == 0) && (countNeutrinos(in) == 0))
	return kTRUE;
    else
	return kFALSE;
}

bool final_state_4e_cut(Tree* in)
{
    if(((in -> Z1Flav) == -121) && ((in -> Z2Flav) == -121))
	return kTRUE;
    else
	return kFALSE;
}

bool final_state_2e2mu_cut(Tree* in)
{
    if( (((in -> Z1Flav) == -121) && ((in -> Z2Flav) == -169)) ||
	(((in -> Z1Flav) == -169) && ((in -> Z2Flav) == -121)) )
	return kTRUE;
    else
	return kFALSE;
}

bool final_state_4mu_cut(Tree* in)
{
    if(((in -> Z1Flav) == -169) && ((in -> Z2Flav) == -169))
	return kTRUE;
    else
	return kFALSE;
}
