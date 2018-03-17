#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Classifier.h>

Mor18Classifier::Mor18Classifier()
{
    manual_WPs = false;
}

Mor18Classifier::~Mor18Classifier()
{ }

int Mor18Classifier::ClassifyThisEvent(Tree* in)
{
    float jetQGLikelihood[99];
    float jetPhi[99];

    for(int i = 0; i < in -> nCleanedJetsPt30; i++)
    {
	jetQGLikelihood[i] = in -> JetQGLikelihood -> at(i);
	jetPhi[i] = in -> JetPhi -> at(i);
    }

    return categoryMor18(
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

void Mor18Classifier::SetWPs(float WP_VBF2j, float WP_VBF1j, float WP_WHh, float WP_ZHh)
{
    manual_WPs = true;
    WP_VBF2j_man = WP_VBF2j;
    WP_VBF1j_man = WP_VBF1j;
    WP_WHh_man = WP_WHh;
    WP_ZHh_man = WP_ZHh;
}

int Mor18Classifier::categoryMor18(
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

  float D_VBF2j = -2;
  float D_VBF1j = -2;
  float D_WHh   = -2;
  float D_ZHh   = -2;
  if(useQGTagging){
    if(nCleanedJetsPt30==1){
      D_VBF1j = DVBF1j_ME_QG(p_JVBF_SIG_ghv1_1_JHUGen_JECNominal, pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal, p_JQCD_SIG_ghg2_1_JHUGen_JECNominal, ZZMass, jetQGLikelihood, jetPhi);
    }else if(nCleanedJetsPt30>=2){
      D_VBF2j = DVBF2j_ME_QG(p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, ZZMass, jetQGLikelihood, jetPhi);
      D_WHh   = DWHh_ME_QG(p_HadWH_SIG_ghw1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, p_HadWH_mavjj_JECNominal, p_HadWH_mavjj_true_JECNominal, ZZMass, jetQGLikelihood, jetPhi);
      D_ZHh   = DZHh_ME_QG(p_HadZH_SIG_ghz1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, p_HadZH_mavjj_JECNominal, p_HadZH_mavjj_true_JECNominal, ZZMass, jetQGLikelihood, jetPhi);
    }
  }else{
    if(nCleanedJetsPt30==1){
      D_VBF1j = DVBF1j_ME(p_JVBF_SIG_ghv1_1_JHUGen_JECNominal, pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal, p_JQCD_SIG_ghg2_1_JHUGen_JECNominal, ZZMass);
    }else if(nCleanedJetsPt30>=2){
      D_VBF2j = DVBF2j_ME(p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, ZZMass);
      D_WHh   = DWHh_ME(p_HadWH_SIG_ghw1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, p_HadWH_mavjj_JECNominal, p_HadWH_mavjj_true_JECNominal, ZZMass);
      D_ZHh   = DZHh_ME(p_HadZH_SIG_ghz1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, p_HadZH_mavjj_JECNominal, p_HadZH_mavjj_true_JECNominal, ZZMass);
    }
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

  if( nExtraLep==0 && (((nCleanedJetsPt30==2||nCleanedJetsPt30==3)&&nCleanedJetsPt30BTagged_bTagSF<=1)||(nCleanedJetsPt30>=4&&nCleanedJetsPt30BTagged_bTagSF==0)) && D_VBF2j>WP_VBF2j ){

    return VBF2jTaggedMor18;

  }else if( nExtraLep==0 && (nCleanedJetsPt30==2||nCleanedJetsPt30==3||(nCleanedJetsPt30>=4&&nCleanedJetsPt30BTagged_bTagSF==0)) && (D_WHh>WP_WHh||D_ZHh>WP_ZHh) ){

    return VHHadrTaggedMor18;

  }else if(   ( nCleanedJetsPt30<=3 && nCleanedJetsPt30BTagged_bTagSF==0 && (nExtraLep==1||nExtraZ>=1) )
	   || ( nCleanedJetsPt30==0 && nExtraLep>=1 ) ){

    return VHLeptTaggedMor18;

  }else if( nCleanedJetsPt30>=4 && nCleanedJetsPt30BTagged_bTagSF>=1 && nExtraLep ==0){

    return ttHHadrTaggedMor18;
  
  }else if( nExtraLep>=1 ){
  
  	return ttHLeptTaggedMor18;
	
  }else if( useVHMETTagged && nExtraLep==0 && (nCleanedJetsPt30==0||nCleanedJetsPt30==1) && PFMET>100 ){

    return VHMETTaggedMor18;

  }else if( nExtraLep==0 && nCleanedJetsPt30==1 && D_VBF1j>WP_VBF1j ){

    return VBF1jTaggedMor18;

  }else{

    return UntaggedMor18;

  }

}
