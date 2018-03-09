#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/LikClassifier.h>

LikClassifier::LikClassifier()
{  
    TString hist_path = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/KDEPlots/";
    TFile* infile = TFile::Open(hist_path + "lik_hist.root");

    lik_hist = (TH1F*)(infile -> Get("lik_hist"));

    //working_point = lik_hist -> GetBinContent(1000 * 0.46386);
    working_point = 0.2;
    std::cout << "current working point = " << working_point << std::endl;
}

LikClassifier::~LikClassifier()
{  }

int LikClassifier::ClassifyEvent()
{
    float jetQGLikelihood[99];
    float jetPhi[99];

    for(int i = 0; i < nCleanedJetsPt30; i++)
    {
	jetQGLikelihood[i] = JetQGLikelihood -> at(i);
	jetPhi[i] = JetPhi -> at(i);
    }

    return categoryMor18Mod(
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
	false); // use QG tagging?
}

int LikClassifier::ClassifyThisEvent(Tree* in)
{
    return -1;
}

float LikClassifier::GetDiscriminant(Tree* in)
{
    float raw_value = DVBF2j_ME(in -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal, in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, in -> ZZMass);

    return lik_hist -> GetBinContent(raw_value * 1000);
}

int LikClassifier::categoryMor18Mod(
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
	//D_VBF2j = DVBF2j_ME(p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, ZZMass);

      // put the likelihood-enhancement here
      D_VBF2j = GetDiscriminant(this);

      D_WHh   = DWHh_ME(p_HadWH_SIG_ghw1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, p_HadWH_mavjj_JECNominal, p_HadWH_mavjj_true_JECNominal, ZZMass);
      D_ZHh   = DZHh_ME(p_HadZH_SIG_ghz1_1_JHUGen_JECNominal, p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, p_HadZH_mavjj_JECNominal, p_HadZH_mavjj_true_JECNominal, ZZMass);
    }
  }

  float WP_VBF2j = working_point;//getDVBF2jetsWP(ZZMass, useQGTagging); // need to change here the working point based on the output of the plot
  float WP_VBF1j = getDVBF1jetWP(ZZMass, useQGTagging);
  float WP_WHh = getDWHhWP(ZZMass, useQGTagging);
  float WP_ZHh = getDZHhWP(ZZMass, useQGTagging);

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
