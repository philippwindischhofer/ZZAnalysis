#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18LIClassifier.h>

Mor18LIClassifier::Mor18LIClassifier()
{
    manual_WPs = false;

    TString out_folder = "../../src/ZZAnalysis/CalibratorPlots/";
    Mor18Config conf;
    coll = MEDiscriminantFactory::GenerateDiscriminantCollection(out_folder, conf);
    comb = new VotingMultiClassCombinator();
}

Mor18LIClassifier::~Mor18LIClassifier()
{  }

// after the restructoring, only this method will remain, and it will overload the corresponding virtual method from the base class
int Mor18LIClassifier::ClassifyThisEvent(Tree* in)
{
    float jetQGLikelihood[99];
    float jetPhi[99];

    for(int i = 0; i < in -> nCleanedJetsPt30; i++)
    {
	jetQGLikelihood[i] = in -> JetQGLikelihood -> at(i);
	jetPhi[i] = in -> JetPhi -> at(i);
    }

    return categoryMor18(
	in,
	in -> nExtraLep,
	in -> nExtraZ,
	in -> nCleanedJetsPt30, 
	in -> nCleanedJetsPt30BTagged_bTagSF,
	jetQGLikelihood,
	in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal,
	in -> p_JQCD_SIG_ghg2_1_JHUGen_JECNominal,
	in -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal,
	in -> p_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
	in -> pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
	in -> p_HadWH_SIG_ghw1_1_JHUGen_JECNominal,
	in -> p_HadZH_SIG_ghz1_1_JHUGen_JECNominal,
	in -> p_HadWH_mavjj_JECNominal,
	in -> p_HadWH_mavjj_true_JECNominal,
	in -> p_HadZH_mavjj_JECNominal,
	in -> p_HadZH_mavjj_true_JECNominal,
	jetPhi,
	in -> ZZMass,
	in -> PFMET,
	true,
	false);
}

void Mor18LIClassifier::SetWPs(float WP_VBF2j, float WP_VBF1j, float WP_WHh, float WP_ZHh)
{
    manual_WPs = true;
    WP_VBF2j_man = WP_VBF2j;
    WP_VBF1j_man = WP_VBF1j;
    WP_WHh_man = WP_WHh;
    WP_ZHh_man = WP_ZHh;
}

float Mor18LIClassifier::disc(float H1_ME, float H0_ME)
{
    return 1.0 / (1.0 + H0_ME / H1_ME);
}

int Mor18LIClassifier::categoryMor18(
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
			     )
{

    // try first the likelihood approach
    comb -> Evaluate(in, coll);
    TString winner = comb -> GetWinningCategory();
    float margin = comb -> GetWinningMargin();

    //std::cout << "winner = " << winner << " / " << "margin = " << margin << std::endl;

    // VBF splitting
    int VBF_cat = (nCleanedJetsPt30 == 1) ? VBF1jTaggedMor18 : VBF2jTaggedMor18;

    std::map<TString, int> conversion = {
	{"VBFH125", VBF_cat},
	{"ZHhadr", VHHadrTaggedMor18},
	{"WHhadr", VHHadrTaggedMor18},
	{"ggH125", UntaggedMor18}
    };

    if(margin > 0.0)
    {
	return conversion[winner];
    }

    // doesn't give a clear candidate, fall back to the standard categorization

  float D_VBF2j = -2;
  float D_VBF1j = -2;
  float D_WHh   = -2;
  float D_ZHh   = -2;

  if(nCleanedJetsPt30==1)
  {
      D_VBF1j = DVBF1j_ME(p_JVBF_SIG_ghv1_1_JHUGen_JECNominal, pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal, p_JQCD_SIG_ghg2_1_JHUGen_JECNominal, ZZMass);
      //D_VBF1j = disc(p_JVBF_SIG_ghv1_1_JHUGen_JECNominal, p_JQCD_SIG_ghg2_1_JHUGen_JECNominal);
  }
  else if(nCleanedJetsPt30>=2)
  {
      D_VBF2j = DVBF2j_ME(p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, ZZMass);
      D_WHh   = DWHh_ME(p_HadWH_SIG_ghw1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, p_HadWH_mavjj_JECNominal, p_HadWH_mavjj_true_JECNominal, ZZMass);
      D_ZHh   = DZHh_ME(p_HadZH_SIG_ghz1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, p_HadZH_mavjj_JECNominal, p_HadZH_mavjj_true_JECNominal, ZZMass); 

      //D_VBF2j = disc(p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal);
      //D_WHh = disc(p_HadWH_SIG_ghw1_1_JHUGen_JECNominal * p_HadWH_mavjj_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal * p_HadWH_mavjj_true_JECNominal);
      //D_ZHh = disc(p_HadZH_SIG_ghz1_1_JHUGen_JECNominal * p_HadZH_mavjj_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal * p_HadZH_mavjj_true_JECNominal);
  }

  // the working points / cuts for the kinematic discriminants
  float WP_VBF2j = 0;
  float WP_VBF1j = 0;
  float WP_WHh = 0;
  float WP_ZHh = 0;

  if(manual_WPs)
  {
      WP_VBF2j = WP_VBF2j_man;
      WP_VBF1j = WP_VBF1j_man;
      WP_WHh = WP_WHh_man;
      WP_ZHh = WP_ZHh_man;
  }
  else
  {
      WP_VBF2j = getDVBF2jetsWP(ZZMass, useQGTagging);
      WP_VBF1j = getDVBF1jetWP(ZZMass, useQGTagging);
      WP_WHh = getDWHhWP(ZZMass, useQGTagging);
      WP_ZHh = getDZHhWP(ZZMass, useQGTagging);
  }

  int category = -1;

  // now, based on the current approach, get the classification - but then, improve it by looking at the other likelihood ratios to reduce the dominant backgrounds in each category
  if( nExtraLep==0 && (((nCleanedJetsPt30==2||nCleanedJetsPt30==3)&&nCleanedJetsPt30BTagged_bTagSF<=1)||(nCleanedJetsPt30>=4&&nCleanedJetsPt30BTagged_bTagSF==0)) && D_VBF2j>WP_VBF2j ){

    category = VBF2jTaggedMor18;
    return category;

  }else if( nExtraLep==0 && (nCleanedJetsPt30==2||nCleanedJetsPt30==3||(nCleanedJetsPt30>=4&&nCleanedJetsPt30BTagged_bTagSF==0)) && (D_WHh>WP_WHh||D_ZHh>WP_ZHh) ){

    category = VHHadrTaggedMor18;
    return category;

  }else if(   ( nCleanedJetsPt30<=3 && nCleanedJetsPt30BTagged_bTagSF==0 && (nExtraLep==1||nExtraZ>=1) )
	   || ( nCleanedJetsPt30==0 && nExtraLep>=1 ) ){

    category = VHLeptTaggedMor18;
    return category;

  }else if( nCleanedJetsPt30>=4 && nCleanedJetsPt30BTagged_bTagSF>=1 && nExtraLep ==0){
    category = ttHHadrTaggedMor18;
    return category;
  
  }else if( nExtraLep>=1 ){
  
  	category = ttHLeptTaggedMor18;
	return category;
	
  }else if( useVHMETTagged && nExtraLep==0 && (nCleanedJetsPt30==0||nCleanedJetsPt30==1) && PFMET>100 ){

    category = VHMETTaggedMor18;
    return category;

  }else if( nExtraLep==0 && nCleanedJetsPt30==1 && D_VBF1j>WP_VBF1j ){

    category = VBF1jTaggedMor18;
    return category;

  }else{

    category = UntaggedMor18;
    return category;

  }

  return category;

}

