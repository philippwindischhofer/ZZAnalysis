#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/Mor17Classifier.h>
#include <ZZAnalysis/AnalysisStep/interface/Category.h>
#include <ZZAnalysis/AnalysisStep/interface/Discriminants.h>

Mor17Classifier::Mor17Classifier()
{ }

Mor17Classifier::~Mor17Classifier()
{ }

int Mor17Classifier::ClassifyEvent()
{
    float jetQGLikelihood[99];
    float jetPhi[99];

    for(int i = 0; i < nCleanedJetsPt30; i++)
    {
	jetQGLikelihood[i] = JetQGLikelihood -> at(i);
	jetPhi[i] = JetPhi -> at(i);
    }

    return categoryMor17(
	nExtraLep,
	nExtraZ,
	nCleanedJetsPt30, 
	nCleanedJetsPt30BTagged_bTagSF,
	jetQGLikelihood,
	p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
	p_JQCD_SIG_ghg2_1_JHUGen_JECNominal,
	p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal,
	p_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
	pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
	p_HadWH_SIG_ghw1_1_JHUGen_JECNominal,
	p_HadZH_SIG_ghz1_1_JHUGen_JECNominal,
	p_HadWH_mavjj_JECNominal,
	p_HadWH_mavjj_true_JECNominal,
	p_HadZH_mavjj_JECNominal,
	p_HadZH_mavjj_true_JECNominal,
	jetPhi,
	ZZMass,
	PFMET,
	true,
	false);
}
