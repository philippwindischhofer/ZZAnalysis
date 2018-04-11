#ifndef Mor18LIClassifier_h
#define Mor18LIClassifier_h

// C++
#include <iostream>
#include <fstream>
#include <string>

// ROOT
#include "TApplication.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TGraphAsymmErrors.h"
#include "TMultiGraph.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TColor.h"
#include "Classifier.h"
#include "Tree.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/DiscriminantCollection.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/MEDiscriminantFactory.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/MLDiscriminantFactory.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/MLDiscriminantFactoryFullCategorySet.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/VotingMultiClassCombinator.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/RANDKLMultiClassCombinator.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/TreeKLMultiClassCombinator.h>

#include <ZZAnalysis/AnalysisStep/interface/Category.h>
#include <ZZAnalysis/AnalysisStep/interface/Discriminants.h>
#include <ZZAnalysis/AnalysisStep/interface/cConstants.h>

class Mor18LIClassifier: public Classifier
{
public:
    Mor18LIClassifier(TString out_folder = "../../src/ZZAnalysis/CalibratorPlots/", TString engine = "voting");
    ~Mor18LIClassifier();

    int ClassifyThisEvent(Tree* in);

    void SetWPs(float WP_VBF2j, float WP_VBF1j, float WP_WHh, float WP_ZHh);

    void SetPriors(float VBF_prior, float ggH_prior, float WHhadr_prior, float ZHhadr_prior, float WHlept_prior, float ZHlept_prior, float ZHMET_prior, float ttHhadr_prior, float ttHlept_prior);

    void SetEngineParameter(TString parameter_name, float parameter_value);

private:
    TString calibration_folder;

    DiscriminantCollection* coll;
    MultiClassCombinator* comb;

    bool manual_WPs;
    float WP_VBF2j_man;
    float WP_VBF1j_man;
    float WP_WHh_man;
    float WP_ZHh_man;

    float disc(float H1_ME, float H0_ME);

    int categoryMor18(
	Tree* in,
	int nExtraLep,
	int nExtraZ,
	int nCleanedJetsPt30,
	int nCleanedJetsPt30BTagged_bTagSF,
	float* jetQGLikelihood,
	float p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
	float p_JQCD_SIG_ghg2_1_JHUGen_JECNominal,
	float p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal,
	float p_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
	float pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
	float p_HadWH_SIG_ghw1_1_JHUGen_JECNominal,
	float p_HadZH_SIG_ghz1_1_JHUGen_JECNominal,
	float p_HadWH_mavjj_JECNominal,
	float p_HadWH_mavjj_true_JECNominal,
	float p_HadZH_mavjj_JECNominal,
	float p_HadZH_mavjj_true_JECNominal,
	float* jetPhi,
	float ZZMass,
	float PFMET,
	bool useVHMETTagged,
	bool useQGTagging
	);	
};

#endif
