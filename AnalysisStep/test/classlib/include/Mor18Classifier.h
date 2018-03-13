#ifndef Mor18Classifier_h
#define Mor18Classifier_h

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

#include <ZZAnalysis/AnalysisStep/interface/Category.h>
#include <ZZAnalysis/AnalysisStep/interface/Discriminants.h>
#include <ZZAnalysis/AnalysisStep/interface/cConstants.h>
#include <ZZAnalysis/AnalysisStep/interface/Category.h>

class Mor18Classifier: public Classifier
{
public:
    Mor18Classifier();
    ~Mor18Classifier();

    int ClassifyEvent();

    void SetWPs(float WP_VBF2j, float WP_VBF1j, float WP_WHh, float WP_ZHh);

private:
    bool manual_WPs;
    float WP_VBF2j_man;
    float WP_VBF1j_man;
    float WP_WHh_man;
    float WP_ZHh_man;

    int categoryMor18(
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
