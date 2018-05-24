#include <ZZAnalysis/AnalysisStep/test/classlib/include/me_discriminants.h>

float D_bkg_ME_disc(Tree* in)
{
    float retval = (in -> p_GG_SIG_ghg2_1_ghz1_1_JHUGen) / (in -> p_GG_SIG_ghg2_1_ghz1_1_JHUGen + (in -> p_QQB_BKG_MCFM) * getDbkgkinConstant((in -> Z1Flav) * (in -> Z2Flav), in -> ZZMass));

    return retval;
}

float DVBF2j_bkg_ME_disc(Tree* in)
{
    int ZZFlav = (in -> Z1Flav) * (in -> Z2Flav);

    float retval = D_bkg_VBFdec(
	in -> p_JJVBF_S_SIG_ghv1_1_MCFM_JECNominal,
	in -> p_HadZH_S_SIG_ghz1_1_MCFM_JECNominal,
	in -> p_HadWH_S_SIG_ghw1_1_MCFM_JECNominal,
	in -> p_JJVBF_BKG_MCFM_JECNominal,
	in -> p_HadZH_BKG_MCFM_JECNominal,
	in -> p_HadWH_BKG_MCFM_JECNominal,
	in -> p_JJQCD_BKG_MCFM_JECNominal,
	in -> p_HadZH_mavjj_JECNominal,
	in -> p_HadZH_mavjj_true_JECNominal,
	in -> p_HadWH_mavjj_JECNominal,
	in -> p_HadWH_mavjj_true_JECNominal,
	in -> pConst_JJVBF_S_SIG_ghv1_1_MCFM_JECNominal,
	in -> pConst_HadZH_S_SIG_ghz1_1_MCFM_JECNominal,
	in -> pConst_HadWH_S_SIG_ghw1_1_MCFM_JECNominal,
	in -> pConst_JJVBF_BKG_MCFM_JECNominal,
	in -> pConst_HadZH_BKG_MCFM_JECNominal,
	in -> pConst_HadWH_BKG_MCFM_JECNominal,
	in -> pConst_JJQCD_BKG_MCFM_JECNominal,
	ZZFlav,
	in -> ZZMass);
    
    return retval;
}

// need the raw MELA kinematic discriminants
float DVBF2j_ME_disc(Tree* in)
{
    float retval = DVBF2j_ME(
	in -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal, 
	in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, 
	in -> ZZMass
	);

    return retval;
};

float DVBF1j_ME_disc(Tree* in)
{
    float retval = DVBF1j_ME(
	in -> p_JVBF_SIG_ghv1_1_JHUGen_JECNominal, 
	in -> pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
	in -> p_JQCD_SIG_ghg2_1_JHUGen_JECNominal,
	in -> ZZMass
	);

    return retval;
};

float DWHh_ME_disc(Tree* in)
{
    float retval = DWHh_ME(
	in -> p_HadWH_SIG_ghw1_1_JHUGen_JECNominal,
	in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
	in -> p_HadWH_mavjj_JECNominal,
	in -> p_HadWH_mavjj_true_JECNominal,
	in -> ZZMass
	);

    return retval;
};

float DZHh_ME_disc(Tree* in)
{
    float retval = DZHh_ME(
	in -> p_HadZH_SIG_ghz1_1_JHUGen_JECNominal,
	in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
	in -> p_HadZH_mavjj_JECNominal,
	in -> p_HadZH_mavjj_true_JECNominal,
	in -> ZZMass
	);

    return retval;
};

float DVH_bkg_ME_disc(Tree* in)
{
    int ZZFlav = (in -> Z1Flav) * (in -> Z2Flav);

    float retval = D_bkg_VHdec(
	in -> p_JJVBF_S_SIG_ghv1_1_MCFM_JECNominal,
	in -> p_HadZH_S_SIG_ghz1_1_MCFM_JECNominal,
	in -> p_HadWH_S_SIG_ghw1_1_MCFM_JECNominal,
	in -> p_JJVBF_BKG_MCFM_JECNominal,
	in -> p_HadZH_BKG_MCFM_JECNominal,
	in -> p_HadWH_BKG_MCFM_JECNominal,
	in -> p_JJQCD_BKG_MCFM_JECNominal,
	in -> p_HadZH_mavjj_JECNominal,
	in -> p_HadZH_mavjj_true_JECNominal,
	in -> p_HadWH_mavjj_JECNominal,
	in -> p_HadWH_mavjj_true_JECNominal,
	in -> pConst_JJVBF_S_SIG_ghv1_1_MCFM_JECNominal,
	in -> pConst_HadZH_S_SIG_ghz1_1_MCFM_JECNominal,
	in -> pConst_HadWH_S_SIG_ghw1_1_MCFM_JECNominal,
	in -> pConst_JJVBF_BKG_MCFM_JECNominal,
	in -> pConst_HadZH_BKG_MCFM_JECNominal,
	in -> pConst_HadWH_BKG_MCFM_JECNominal,
	in -> pConst_JJQCD_BKG_MCFM_JECNominal,
	ZZFlav,
	in -> ZZMass);

    return retval;
}

// here come the "non-standard" discriminants between several *tagged* categories
float DWHZH_ME_disc(Tree* in)
{
    float c_MelaWH = getDWHhConstant(in -> ZZMass);
    float c_MelaZH = getDZHhConstant(in -> ZZMass);
    float retval = generic_disc(
	(in -> p_HadWH_SIG_ghw1_1_JHUGen_JECNominal) * (in -> p_HadWH_mavjj_JECNominal) / 
	(c_MelaWH * (in -> p_HadWH_mavjj_true_JECNominal)),
	(in -> p_HadZH_SIG_ghz1_1_JHUGen_JECNominal) * (in -> p_HadZH_mavjj_JECNominal) /
	(c_MelaZH * (in -> p_HadZH_mavjj_true_JECNominal))
	);

    return retval;
};

float DVBFWH_ME_disc(Tree* in)
{
    float c_Mela2j = getDVBF2jetsConstant(in -> ZZMass);
    float c_MelaWH = getDWHhConstant(in -> ZZMass);
    float retval = generic_disc(
	(in -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal) / c_Mela2j,
	(in -> p_HadWH_SIG_ghw1_1_JHUGen_JECNominal) * (in -> p_HadWH_mavjj_JECNominal) / 
	(c_MelaWH * (in -> p_HadWH_mavjj_true_JECNominal))
	);
    
    return retval;
};

float DVBFZH_ME_disc(Tree* in)
{
    float c_Mela2j = getDVBF2jetsConstant(in -> ZZMass);
    float c_MelaZH = getDZHhConstant(in -> ZZMass);
    float retval = generic_disc(
	(in -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal) / c_Mela2j,
	(in -> p_HadZH_SIG_ghz1_1_JHUGen_JECNominal) * (in -> p_HadZH_mavjj_JECNominal) /
	(c_MelaZH * (in -> p_HadZH_mavjj_true_JECNominal))
	);

    return retval;
};

// as well have the discriminants using QG likelihoods
float DVBF2j_ME_QG_disc(Tree* in)
{
    int n_jets = in -> nCleanedJetsPt30;
    float jetQGLikelihood[n_jets];
    float jetPhi[n_jets];
    
    for(int i = 0; i < n_jets; i++)
    {
	jetQGLikelihood[i] = in -> JetQGLikelihood -> at(i);
	jetPhi[i] = in -> JetPhi -> at(i);
    }
    
    float retval = DVBF2j_ME_QG(
	in -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal,
	in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
	in -> ZZMass,
	jetQGLikelihood,
	jetPhi);

    return retval;
}

float DVBF1j_ME_QG_disc(Tree* in)
{
    int n_jets = in -> nCleanedJetsPt30;
    float jetQGLikelihood[n_jets];
    float jetPhi[n_jets];
    
    for(int i = 0; i < n_jets; i++)
    {
	jetQGLikelihood[i] = in -> JetQGLikelihood -> at(i);
	jetPhi[i] = in -> JetPhi -> at(i);
    }
    
    float retval = DVBF1j_ME_QG(
	in -> p_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
	in -> pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
	in -> p_JQCD_SIG_ghg2_1_JHUGen_JECNominal,
	in -> ZZMass,
	jetQGLikelihood,
	jetPhi);

    return retval;
}

float DWHh_ME_QG_disc(Tree* in)
{
    int n_jets = in -> nCleanedJetsPt30;
    float jetQGLikelihood[n_jets];
    float jetPhi[n_jets];
    
    for(int i = 0; i < n_jets; i++)
    {
	jetQGLikelihood[i] = in -> JetQGLikelihood -> at(i);
	jetPhi[i] = in -> JetPhi -> at(i);
    }
    
    float retval = DWHh_ME_QG(
	in -> p_HadWH_SIG_ghw1_1_JHUGen_JECNominal,
	in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
	in -> p_HadWH_mavjj_JECNominal,
	in -> p_HadWH_mavjj_true_JECNominal,
	in -> ZZMass,
	jetQGLikelihood,
	jetPhi);

    return retval;
}

float DZHh_ME_QG_disc(Tree* in)
{
    int n_jets = in -> nCleanedJetsPt30;
    float jetQGLikelihood[n_jets];
    float jetPhi[n_jets];
    
    for(int i = 0; i < n_jets; i++)
    {
	jetQGLikelihood[i] = in -> JetQGLikelihood -> at(i);
	jetPhi[i] = in -> JetPhi -> at(i);
    }
    
    float retval = DZHh_ME_QG(
	in -> p_HadZH_SIG_ghz1_1_JHUGen_JECNominal,
	in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
	in -> p_HadZH_mavjj_JECNominal,
	in -> p_HadZH_mavjj_true_JECNominal,
	in -> ZZMass,
	jetQGLikelihood,
	jetPhi);

    return retval;
}

float DWHZH_ME_QG_disc(Tree* in)
{
    float QG_correction = TMath::Power(generic_Pg_Pq(in), 1. / 3.);
    float raw_disc = DWHZH_ME_disc(in);
    float retval = 1.0 / (1.0 + (1.0 / raw_disc - 1.0) * QG_correction);

    return retval;
}

float DVBFWH_ME_QG_disc(Tree* in)
{
    float QG_correction = TMath::Power(generic_Pg_Pq(in), 1. / 3.);
    float raw_disc = DVBFWH_ME_disc(in);
    float retval = 1.0 / (1.0 + (1.0 / raw_disc - 1.0) * QG_correction);

    return retval;
}

float DVBFZH_ME_QG_disc(Tree* in)
{
    float QG_correction = TMath::Power(generic_Pg_Pq(in), 1. / 3.);
    float raw_disc = DVBFZH_ME_disc(in);
    float retval = 1.0 / (1.0 + (1.0 / raw_disc - 1.0) * QG_correction);

    return retval;
}

float generic_disc(float H1_ME, float H0_ME)
{
    if(H1_ME == 0.0)
	return 0.0;
    else
	return 1.0 / (1.0 + H0_ME / H1_ME);
};

float generic_Pg_Pq(Tree* in)
{
    int n_jets = in -> nCleanedJetsPt30;
    float retval = 1.0;
    
    for(int i = 0; i < n_jets; i++)
    {
	float jetQGLikelihood = in -> JetQGLikelihood -> at(i);
	float jetPhi = in -> JetPhi -> at(i);
	retval *= jetPgOverPq(jetQGLikelihood, jetPhi);
    }

    return retval;
}
