#include <ZZAnalysis/AnalysisStep/test/classlib/include/me_discriminants.h>

// need the raw MELA kinematic discriminants
float DVBF2j_ME_disc(Tree* in)
{
    return DVBF2j_ME(
	in -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal, 
	in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, 
	in -> ZZMass
	);
};

float DVBF1j_ME_disc(Tree* in)
{
    return DVBF1j_ME(
	in -> p_JVBF_SIG_ghv1_1_JHUGen_JECNominal, 
	in -> pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
	in -> p_JQCD_SIG_ghg2_1_JHUGen_JECNominal,
	in -> ZZMass
	);
};

float DWHh_ME_disc(Tree* in)
{
    return DWHh_ME(
	in -> p_HadWH_SIG_ghw1_1_JHUGen_JECNominal,
	in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
	in -> p_HadWH_mavjj_JECNominal,
	in -> p_HadWH_mavjj_true_JECNominal,
	in -> ZZMass
	);
};

float DZHh_ME_disc(Tree* in)
{
    return DZHh_ME(
	in -> p_HadZH_SIG_ghz1_1_JHUGen_JECNominal,
	in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
	in -> p_HadZH_mavjj_JECNominal,
	in -> p_HadZH_mavjj_true_JECNominal,
	in -> ZZMass
	);
};

// here come the "non-standard" discriminants between several *tagged* categories
float DWHZH_ME_disc(Tree* in)
{
    float c_MelaWH = getDWHhConstant(in -> ZZMass);
    float c_MelaZH = getDZHhConstant(in -> ZZMass);
    return generic_disc(
	(in -> p_HadWH_SIG_ghw1_1_JHUGen_JECNominal) * (in -> p_HadWH_mavjj_JECNominal) / 
	(c_MelaWH * (in -> p_HadWH_mavjj_true_JECNominal)),
	(in -> p_HadZH_SIG_ghz1_1_JHUGen_JECNominal) * (in -> p_HadZH_mavjj_JECNominal) /
	(c_MelaZH * (in -> p_HadZH_mavjj_true_JECNominal))
	);
};

float DVBFWH_ME_disc(Tree* in)
{
    float c_Mela2j = getDVBF2jetsConstant(in -> ZZMass);
    float c_MelaWH = getDWHhConstant(in -> ZZMass);
    return generic_disc(
	(in -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal) / c_Mela2j,
	(in -> p_HadWH_SIG_ghw1_1_JHUGen_JECNominal) * (in -> p_HadWH_mavjj_JECNominal) / 
	(c_MelaWH * (in -> p_HadWH_mavjj_true_JECNominal))
	);
};

float DVBFZH_ME_disc(Tree* in)
{
    float c_Mela2j = getDVBF2jetsConstant(in -> ZZMass);
    float c_MelaZH = getDZHhConstant(in -> ZZMass);
    return generic_disc(
	(in -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal) / c_Mela2j,
	(in -> p_HadZH_SIG_ghz1_1_JHUGen_JECNominal) * (in -> p_HadZH_mavjj_JECNominal) /
	(c_MelaZH * (in -> p_HadZH_mavjj_true_JECNominal))
	);
};

float generic_disc(float H1_ME, float H0_ME)
{
    return 1.0 / (1.0 + H0_ME / H1_ME);
};
