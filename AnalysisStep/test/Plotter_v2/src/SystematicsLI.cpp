#include <ZZAnalysis/AnalysisStep/test/Plotter_v2/include/SystematicsLI.h>

SystematicsLI::SystematicsLI()
{ }

SystematicsLI::~SystematicsLI()
{ }

TString SystematicsLI::GetFolder(TString original)
{
    std::vector<std::string> components = ConfigFileUtils::Split(original.Data(), "/");
    return components.end()[-2];
}

TString SystematicsLI::ChangeFolder(TString original, TString new_folder)
{
    std::vector<std::string> components = ConfigFileUtils::Split(original.Data(), "/");
    std::string retval = "/";

    for(auto iter = components.begin(); iter != components.end() - 2; iter++)
    {
	retval += *iter;
	retval += "/";
    }

    retval += new_folder + "/";
    retval += components.end()[-1];

    return retval;      
}

void SystematicsLI::SetPackagePath(TString package_path, TString engine)
{
    config_file_path = package_path + "settings.conf";
    calibration_dir = package_path + "calibration/";
    priors_file_path = package_path + "priors_" + engine + "/priors_bkg.txt";

    //refclass = new Mor18Classifier();
    refclass = new Mor18LIClassifier(calibration_dir, config_file_path, engine);

    // // load priors here and assign them etc.
    ConfigFileHandler* handler = new ConfigFileHandler(priors_file_path, "read");
    float VBF_prior = handler -> GetField("VBF_prior");
    float ggH_prior = handler -> GetField("ggH_prior");
    float WHhadr_prior = handler -> GetField("WHhadr_prior");
    float ZHhadr_prior = handler -> GetField("ZHhadr_prior");
    float WHlept_prior = handler -> GetField("WHlept_prior");
    float ZHlept_prior = handler -> GetField("ZHlept_prior");
    float ZHMET_prior = handler -> GetField("ZHMET_prior");
    float ttHhadr_prior = handler -> GetField("ttHhadr_prior");
    float ttHlept_prior = handler -> GetField("ttHlept_prior");
    float bkg_prior = handler -> GetField("ZX_prior");
    float qq_prior = handler -> GetField("qq_prior");

    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << " using the following priors: " << std::endl;
    std::cout << " VBF_prior = " << VBF_prior << std::endl;
    std::cout << " ggH_prior = " << ggH_prior << std::endl;
    std::cout << " WHhadr_prior = " << WHhadr_prior << std::endl;
    std::cout << " ZHhadr_prior = " << ZHhadr_prior << std::endl;
    std::cout << " WHlept_prior = " << WHlept_prior << std::endl;
    std::cout << " ZHlept_prior = " << ZHlept_prior << std::endl;
    std::cout << " ZHMET_prior = " << ZHMET_prior << std::endl;
    std::cout << " ttHhadr_prior = " << ttHhadr_prior << std::endl;
    std::cout << " ttHlept_prior = " << ttHlept_prior << std::endl;
    std::cout << " ZX_prior = " << bkg_prior << std::endl;
    std::cout << " qq_prior = " << qq_prior << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;

    Mor18LIClassifier* refclass18 = static_cast<Mor18LIClassifier*>(refclass);
    refclass18 -> SetEngineParameter("min_iterations", 25);
    refclass18 -> SetEngineParameter("max_iterations", 125);

    refclass18 -> SetPriors(VBF_prior, ggH_prior, WHhadr_prior, ZHhadr_prior, WHlept_prior, ZHlept_prior, ZHMET_prior, ttHhadr_prior, ttHlept_prior, bkg_prior, qq_prior);
}

void SystematicsLI::FillSystematics(TString input_file_name)
{
    //std::cout << "SystematicsLI::FillSystematics" << std::endl;
    
    // --------------------------------------------------------------------
    //  read here the original file without and modifications of its parameters
    // --------------------------------------------------------------------

    //std::cout << "original input file name: " << input_file_name << std::endl;

    TString cur_input_file_name = input_file_name;
    input_file = new TFile(cur_input_file_name);

    //std::cout << "currently reading COMMON from: " << cur_input_file_name;

    hCounters = (TH1F*)input_file->Get("ClassTree/Counters");
    n_gen_events = (Long64_t)hCounters->GetBinContent(1);
    gen_sum_weights = (Long64_t)hCounters->GetBinContent(40);
	
    input_tree = (TTree*)input_file->Get("ClassTree/candTree");
    Init( input_tree, input_file_name );
	
    if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntriesFast();

    Long64_t nbytes = 0, nb = 0;

    for (Long64_t jentry=0; jentry<nentries;jentry++)
    {
	Long64_t ientry = LoadTree(jentry);
	if (ientry < 0) break;
	nb = fChain->GetEntry(jentry);
	nbytes += nb;
		
	if ( LepEta->size() != 4 )
	{
	    cout << "[ERROR] in event " << RunNumber << ":" << LumiNumber << ":" << EventNumber << ", stored " << LepEta->size() << " leptons instead of 4" << endl;
	    continue;
	}

	if ( !(ZZsel >= 90) ) continue;
	if ( ZZMass < 105. || ZZMass > 140. ) continue;

	// Find current production mode
	gen_assoc_lep_id_.push_back(GenAssocLep1Id);
   	gen_assoc_lep_id_.push_back(GenAssocLep2Id);
	_n_gen_assoc_lep = CountAssociatedLeptons();
	_current_production_mode = find_current_production_mode( input_file_name, genExtInfo , _n_gen_assoc_lep);
	gen_assoc_lep_id_.clear();
		
	// Final states
	_current_final_state = FindFinalState();
		
	// Categories
	for ( int j = 0; j < nCleanedJetsPt30; j++)
	{
	    jetPt[j] = JetPt->at(j);
	    jetEta[j] = JetEta->at(j);
	    jetPhi[j] = JetPhi->at(j);
	    jetMass[j] = JetMass->at(j);
	    jetQGL[j] = JetQGLikelihood->at(j);
	    jetPgOverPq[j] = 1./JetQGLikelihood->at(j) - 1.;
	}

	_current_category = refclass -> ClassifyThisEvent(this);
		
	//============================================================
	// PileUp
	//============================================================
		
	// K factors
	_k_factor 			    = calculate_K_factor(input_file_name);
		
	// Final event weight
	_event_weight    = ( xsec * _k_factor * overallEventWeight * ( 1. )                  						     ) / gen_sum_weights;
	_event_weight_UP = ( xsec * _k_factor * overallEventWeight * (PUWeight == 0. ? 0. : PUWeight_Up/PUWeight)  ) / gen_sum_weights;
	_event_weight_DN = ( xsec * _k_factor * overallEventWeight * (PUWeight == 0. ? 0. : PUWeight_Dn/PUWeight)  ) / gen_sum_weights;
	//if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	// Sum yields
	_expected_yield_PU[_current_production_mode][_current_category]           += _event_weight;
	_expected_yield_PU_UP[_current_production_mode][_current_category]        += _event_weight_UP;
	_expected_yield_PU_DN[_current_production_mode][_current_category]        += _event_weight_DN;
		
	_expected_yield_PU[_current_production_mode][Settings::inclusive]         += _event_weight;
	_expected_yield_PU_UP[_current_production_mode][Settings::inclusive]      += _event_weight_UP;
	_expected_yield_PU_DN[_current_production_mode][Settings::inclusive]      += _event_weight_DN;

	//============================================================
	// prepare inclusive yields for the remaining runs
	//============================================================

	_k_factor = calculate_K_factor(input_file_name);
	_event_weight    = ( xsec * _k_factor * overallEventWeight) / gen_sum_weights;
	_expected_yield_JEC[_current_production_mode][_current_category]           += _event_weight;
	_expected_yield_JEC[_current_production_mode][Settings::inclusive]    += _event_weight;	

	_k_factor = calculate_K_factor(input_file_name);
	_event_weight    = ( xsec * _k_factor * overallEventWeight) / gen_sum_weights;
	_expected_yield_BTag[_current_production_mode][_current_category]           += _event_weight;
	_expected_yield_BTag[_current_production_mode][Settings::inclusive]           += _event_weight;

	//============================================================
	// muR Scale
	//============================================================
		
	// K factors
	_k_factor 			    = calculate_K_factor(input_file_name);
		
	// Final event weight
	_event_weight    = ( xsec * _k_factor * overallEventWeight * ( 1. ) ) / gen_sum_weights;
	_event_weight_UP = ( xsec * _k_factor * overallEventWeight * (LHEweight_QCDscale_muR2_muF1/LHEweight_QCDscale_muR1_muF1)  ) / gen_sum_weights;
	_event_weight_DN = ( xsec * _k_factor * overallEventWeight * (LHEweight_QCDscale_muR0p5_muF1/LHEweight_QCDscale_muR1_muF1)) / gen_sum_weights;
	//if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	// Sum yields
	_expected_yield_muR[_current_production_mode][_current_category]           += _event_weight;
	_expected_yield_muR_UP[_current_production_mode][_current_category]        += _event_weight_UP;
	_expected_yield_muR_DN[_current_production_mode][_current_category]        += _event_weight_DN;
		
	_expected_yield_muR[_current_production_mode][Settings::inclusive]           += _event_weight;
	_expected_yield_muR_UP[_current_production_mode][Settings::inclusive]        += _event_weight_UP;
	_expected_yield_muR_DN[_current_production_mode][Settings::inclusive]        += _event_weight_DN;
		
	//============================================================
	// muF Scale
	//============================================================
		
	// K factors
	_k_factor 			    = calculate_K_factor(input_file_name);
		
	// Final event weight
	_event_weight    = ( xsec * _k_factor * overallEventWeight * ( 1. ) ) / gen_sum_weights;
	_event_weight_UP = ( xsec * _k_factor * overallEventWeight * (LHEweight_QCDscale_muR1_muF2/LHEweight_QCDscale_muR1_muF1)  ) / gen_sum_weights;
	_event_weight_DN = ( xsec * _k_factor * overallEventWeight * (LHEweight_QCDscale_muR1_muF0p5/LHEweight_QCDscale_muR1_muF1)) / gen_sum_weights;
	//if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	// Sum yields
	_expected_yield_muF[_current_production_mode][_current_category]           += _event_weight;
	_expected_yield_muF_UP[_current_production_mode][_current_category]        += _event_weight_UP;
	_expected_yield_muF_DN[_current_production_mode][_current_category]        += _event_weight_DN;
		
	_expected_yield_muF[_current_production_mode][Settings::inclusive]           += _event_weight;
	_expected_yield_muF_UP[_current_production_mode][Settings::inclusive]        += _event_weight_UP;
	_expected_yield_muF_DN[_current_production_mode][Settings::inclusive]        += _event_weight_DN;
		
	//============================================================
	// Alpha strong
	//============================================================
		
	// K factors
	_k_factor 			    = calculate_K_factor(input_file_name);
		
	// Final event weight
	_event_weight    = ( xsec * _k_factor * overallEventWeight * ( 1. )              ) / gen_sum_weights;
	_event_weight_UP = ( xsec * _k_factor * overallEventWeight * (LHEweight_AsMZ_Up) ) / gen_sum_weights;
	_event_weight_DN = ( xsec * _k_factor * overallEventWeight * (LHEweight_AsMZ_Dn) ) / gen_sum_weights;
	//if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	// Sum yields
	_expected_yield_As[_current_production_mode][_current_category]           += _event_weight;
	_expected_yield_As_UP[_current_production_mode][_current_category]        += _event_weight_UP;
	_expected_yield_As_DN[_current_production_mode][_current_category]        += _event_weight_DN;
		
	_expected_yield_As[_current_production_mode][Settings::inclusive]         += _event_weight;
	_expected_yield_As_UP[_current_production_mode][Settings::inclusive]      += _event_weight_UP;
	_expected_yield_As_DN[_current_production_mode][Settings::inclusive]      += _event_weight_DN;
		
	//============================================================
	// PDF Variation
	//============================================================
		
	// K factors
	_k_factor 			    = calculate_K_factor(input_file_name);
		
	// Final event weight
	_event_weight    = ( xsec * _k_factor * overallEventWeight * ( 1. )                      ) / gen_sum_weights;
	_event_weight_UP = ( xsec * _k_factor * overallEventWeight * (LHEweight_PDFVariation_Up) ) / gen_sum_weights;
	_event_weight_DN = ( xsec * _k_factor * overallEventWeight * (LHEweight_PDFVariation_Dn) ) / gen_sum_weights;
	//if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	// Sum yields
	_expected_yield_PDF[_current_production_mode][_current_category]           += _event_weight;
	_expected_yield_PDF_UP[_current_production_mode][_current_category]        += _event_weight_UP;
	_expected_yield_PDF_DN[_current_production_mode][_current_category]        += _event_weight_DN;
		
	_expected_yield_PDF[_current_production_mode][Settings::inclusive]         += _event_weight;
	_expected_yield_PDF_UP[_current_production_mode][Settings::inclusive]      += _event_weight_UP;
	_expected_yield_PDF_DN[_current_production_mode][Settings::inclusive]      += _event_weight_DN;
		
	//============================================================
	// EW corrections
	//============================================================
		
	// K factors
	_k_factor 			    = calculate_K_factor(input_file_name);
	if(input_file_name.Contains("ZZTo4l"))
	{
	    _k_factor_EWCorr_UP = KFactor_EW_qqZZ * ( 1. + KFactor_EW_qqZZ_unc ) * KFactor_QCD_qqZZ_M;
	    _k_factor_EWCorr_DN = KFactor_EW_qqZZ * ( 1. - KFactor_EW_qqZZ_unc ) * KFactor_QCD_qqZZ_M;
	}
		
	// Final event weight
	_event_weight    = ( xsec * _k_factor           * overallEventWeight ) / gen_sum_weights;
	_event_weight_UP = ( xsec * _k_factor_EWCorr_UP * overallEventWeight ) / gen_sum_weights;
	_event_weight_DN = ( xsec * _k_factor_EWCorr_DN * overallEventWeight ) / gen_sum_weights;
	//if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	// Sum yields
	_expected_yield_EWCorr[_current_production_mode][_current_category]           += _event_weight;
	_expected_yield_EWCorr_UP[_current_production_mode][_current_category]        += _event_weight_UP;
	_expected_yield_EWCorr_DN[_current_production_mode][_current_category]        += _event_weight_DN;
		
	_expected_yield_EWCorr[_current_production_mode][Settings::inclusive]         += _event_weight;
	_expected_yield_EWCorr_UP[_current_production_mode][Settings::inclusive]      += _event_weight_UP;
	_expected_yield_EWCorr_DN[_current_production_mode][Settings::inclusive]      += _event_weight_DN;
		
		
	//============================================================
	// Pythia Scale
	//============================================================
		
	// K factors
	_k_factor 			    = calculate_K_factor(input_file_name);
		
	// Final event weight
	_event_weight    = ( xsec * _k_factor * overallEventWeight * ( 1. )                                                ) / gen_sum_weights;
	_event_weight_UP = ( xsec * _k_factor * overallEventWeight * (PythiaWeight_isr_muR4    * PythiaWeight_fsr_muR4   ) ) / gen_sum_weights;
	_event_weight_DN = ( xsec * _k_factor * overallEventWeight * (PythiaWeight_isr_muR0p25 * PythiaWeight_fsr_muR0p25) ) / gen_sum_weights;
	//if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	// Sum yields
	_expected_yield_PythiaScale[_current_production_mode][_current_category]           += _event_weight;
	_expected_yield_PythiaScale_UP[_current_production_mode][_current_category]        += _event_weight_UP;
	_expected_yield_PythiaScale_DN[_current_production_mode][_current_category]        += _event_weight_DN;
		
	_expected_yield_PythiaScale[_current_production_mode][Settings::inclusive]         += _event_weight;
	_expected_yield_PythiaScale_UP[_current_production_mode][Settings::inclusive]      += _event_weight_UP;
	_expected_yield_PythiaScale_DN[_current_production_mode][Settings::inclusive]      += _event_weight_DN;
		
	//============================================================
	// Pythia Tune
	//============================================================
		
	// K factors
	_k_factor 			    = calculate_K_factor(input_file_name);
		
	// Final event weight
	_event_weight    = ( xsec * _k_factor * overallEventWeight * ( 1. )                                                ) / gen_sum_weights;
	//if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	// Sum yields
	_expected_yield_PythiaTune[_current_production_mode][_current_category]           += _event_weight;
	_expected_yield_PythiaTune[_current_production_mode][Settings::inclusive]         += _event_weight;
	//Up/Dn variations have dedicated samples and are filled in FillSystematics_tuneUpDn() function

		
	//============================================================
	// THU_ggH uncertainties
	//============================================================
	if(input_file_name.Contains("ggH125"))
	{
	    // K factors
	    _k_factor 			    = calculate_K_factor(input_file_name);
			
	    // Final event weight
	    _event_weight    = ( xsec * _k_factor           * overallEventWeight ) / gen_sum_weights;
	    _event_weight_UP = ( xsec * _k_factor * overallEventWeight * (qcd_ggF_uncertSF->at(0))) / gen_sum_weights;
	    _event_weight_DN = ( xsec * _k_factor * overallEventWeight * (1. - (qcd_ggF_uncertSF->at(0) - 1.))) / gen_sum_weights;
			
	    //if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	    // Sum yields
	    _expected_yield_THU_ggH_Mu[_current_production_mode][_current_category]           += _event_weight;
	    _expected_yield_THU_ggH_Mu_UP[_current_production_mode][_current_category]        += _event_weight_UP;
	    _expected_yield_THU_ggH_Mu_DN[_current_production_mode][_current_category]        += _event_weight_DN;
			
	    _expected_yield_THU_ggH_Mu[_current_production_mode][Settings::inclusive]         += _event_weight;
	    _expected_yield_THU_ggH_Mu_UP[_current_production_mode][Settings::inclusive]      += _event_weight_UP;
	    _expected_yield_THU_ggH_Mu_DN[_current_production_mode][Settings::inclusive]      += _event_weight_DN;
			
	    // K factors
	    _k_factor 			    = calculate_K_factor(input_file_name);
			
	    // Final event weight
	    _event_weight    = ( xsec * _k_factor           * overallEventWeight ) / gen_sum_weights;
	    _event_weight_UP = ( xsec * _k_factor * overallEventWeight * (qcd_ggF_uncertSF->at(1))) / gen_sum_weights;
	    _event_weight_DN = ( xsec * _k_factor * overallEventWeight * (1. - (qcd_ggF_uncertSF->at(1) - 1.))) / gen_sum_weights;
			
	    //if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	    // Sum yields
	    _expected_yield_THU_ggH_Res[_current_production_mode][_current_category]           += _event_weight;
	    _expected_yield_THU_ggH_Res_UP[_current_production_mode][_current_category]        += _event_weight_UP;
	    _expected_yield_THU_ggH_Res_DN[_current_production_mode][_current_category]        += _event_weight_DN;
			
	    _expected_yield_THU_ggH_Res[_current_production_mode][Settings::inclusive]         += _event_weight;
	    _expected_yield_THU_ggH_Res_UP[_current_production_mode][Settings::inclusive]      += _event_weight_UP;
	    _expected_yield_THU_ggH_Res_DN[_current_production_mode][Settings::inclusive]      += _event_weight_DN;
			
	    // K factors
	    _k_factor 			    = calculate_K_factor(input_file_name);
			
	    // Final event weight
	    _event_weight    = ( xsec * _k_factor           * overallEventWeight ) / gen_sum_weights;
	    _event_weight_UP = ( xsec * _k_factor * overallEventWeight * (qcd_ggF_uncertSF->at(2))) / gen_sum_weights;
	    _event_weight_DN = ( xsec * _k_factor * overallEventWeight * (1. - (qcd_ggF_uncertSF->at(2) - 1.))) / gen_sum_weights;
			
	    //if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	    // Sum yields
	    _expected_yield_THU_ggH_Mig01[_current_production_mode][_current_category]           += _event_weight;
	    _expected_yield_THU_ggH_Mig01_UP[_current_production_mode][_current_category]        += _event_weight_UP;
	    _expected_yield_THU_ggH_Mig01_DN[_current_production_mode][_current_category]        += _event_weight_DN;
			
	    _expected_yield_THU_ggH_Mig01[_current_production_mode][Settings::inclusive]         += _event_weight;
	    _expected_yield_THU_ggH_Mig01_UP[_current_production_mode][Settings::inclusive]      += _event_weight_UP;
	    _expected_yield_THU_ggH_Mig01_DN[_current_production_mode][Settings::inclusive]      += _event_weight_DN;
			
	    // K factors
	    _k_factor 			    = calculate_K_factor(input_file_name);
			
	    // Final event weight
	    _event_weight    = ( xsec * _k_factor           * overallEventWeight ) / gen_sum_weights;
	    _event_weight_UP = ( xsec * _k_factor * overallEventWeight * (qcd_ggF_uncertSF->at(3))) / gen_sum_weights;
	    _event_weight_DN = ( xsec * _k_factor * overallEventWeight * (1. - (qcd_ggF_uncertSF->at(3) - 1.))) / gen_sum_weights;
			
	    //if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	    // Sum yields
	    _expected_yield_THU_ggH_Mig12[_current_production_mode][_current_category]           += _event_weight;
	    _expected_yield_THU_ggH_Mig12_UP[_current_production_mode][_current_category]        += _event_weight_UP;
	    _expected_yield_THU_ggH_Mig12_DN[_current_production_mode][_current_category]        += _event_weight_DN;
			
	    _expected_yield_THU_ggH_Mig12[_current_production_mode][Settings::inclusive]         += _event_weight;
	    _expected_yield_THU_ggH_Mig12_UP[_current_production_mode][Settings::inclusive]      += _event_weight_UP;
	    _expected_yield_THU_ggH_Mig12_DN[_current_production_mode][Settings::inclusive]      += _event_weight_DN;
			
	    // K factors
	    _k_factor 			    = calculate_K_factor(input_file_name);
			
	    // Final event weight
	    _event_weight    = ( xsec * _k_factor           * overallEventWeight ) / gen_sum_weights;
	    _event_weight_UP = ( xsec * _k_factor * overallEventWeight * (qcd_ggF_uncertSF->at(4))) / gen_sum_weights;
	    _event_weight_DN = ( xsec * _k_factor * overallEventWeight * (1. - (qcd_ggF_uncertSF->at(4) - 1.))) / gen_sum_weights;
			
	    //if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	    // Sum yields
	    _expected_yield_THU_ggH_VBF2j[_current_production_mode][_current_category]           += _event_weight;
	    _expected_yield_THU_ggH_VBF2j_UP[_current_production_mode][_current_category]        += _event_weight_UP;
	    _expected_yield_THU_ggH_VBF2j_DN[_current_production_mode][_current_category]        += _event_weight_DN;
			
	    _expected_yield_THU_ggH_VBF2j[_current_production_mode][Settings::inclusive]         += _event_weight;
	    _expected_yield_THU_ggH_VBF2j_UP[_current_production_mode][Settings::inclusive]      += _event_weight_UP;
	    _expected_yield_THU_ggH_VBF2j_DN[_current_production_mode][Settings::inclusive]      += _event_weight_DN;
			
	    // K factors
	    _k_factor 			    = calculate_K_factor(input_file_name);
			
	    // Final event weight
	    _event_weight    = ( xsec * _k_factor           * overallEventWeight ) / gen_sum_weights;
	    _event_weight_UP = ( xsec * _k_factor * overallEventWeight * (qcd_ggF_uncertSF->at(5))) / gen_sum_weights;
	    _event_weight_DN = ( xsec * _k_factor * overallEventWeight * (1. - (qcd_ggF_uncertSF->at(5) - 1.))) / gen_sum_weights;
			
	    //if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	    // Sum yields
	    _expected_yield_THU_ggH_VBF3j[_current_production_mode][_current_category]           += _event_weight;
	    _expected_yield_THU_ggH_VBF3j_UP[_current_production_mode][_current_category]        += _event_weight_UP;
	    _expected_yield_THU_ggH_VBF3j_DN[_current_production_mode][_current_category]        += _event_weight_DN;
			
	    _expected_yield_THU_ggH_VBF3j[_current_production_mode][Settings::inclusive]         += _event_weight;
	    _expected_yield_THU_ggH_VBF3j_UP[_current_production_mode][Settings::inclusive]      += _event_weight_UP;
	    _expected_yield_THU_ggH_VBF3j_DN[_current_production_mode][Settings::inclusive]      += _event_weight_DN;
			
	    // K factors
	    _k_factor 			    = calculate_K_factor(input_file_name);
			
	    // Final event weight
	    _event_weight    = ( xsec * _k_factor           * overallEventWeight ) / gen_sum_weights;
	    _event_weight_UP = ( xsec * _k_factor * overallEventWeight * (qcd_ggF_uncertSF->at(6))) / gen_sum_weights;
	    _event_weight_DN = ( xsec * _k_factor * overallEventWeight * (1. - (qcd_ggF_uncertSF->at(6) - 1.))) / gen_sum_weights;
			
	    //if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	    // Sum yields
	    _expected_yield_THU_ggH_PT60[_current_production_mode][_current_category]           += _event_weight;
	    _expected_yield_THU_ggH_PT60_UP[_current_production_mode][_current_category]        += _event_weight_UP;
	    _expected_yield_THU_ggH_PT60_DN[_current_production_mode][_current_category]        += _event_weight_DN;
			
	    _expected_yield_THU_ggH_PT60[_current_production_mode][Settings::inclusive]         += _event_weight;
	    _expected_yield_THU_ggH_PT60_UP[_current_production_mode][Settings::inclusive]      += _event_weight_UP;
	    _expected_yield_THU_ggH_PT60_DN[_current_production_mode][Settings::inclusive]      += _event_weight_DN;
			
	    // K factors
	    _k_factor 			    = calculate_K_factor(input_file_name);
			
	    // Final event weight
	    _event_weight    = ( xsec * _k_factor           * overallEventWeight ) / gen_sum_weights;
	    _event_weight_UP = ( xsec * _k_factor * overallEventWeight * (qcd_ggF_uncertSF->at(7))) / gen_sum_weights;
	    _event_weight_DN = ( xsec * _k_factor * overallEventWeight * (1. - (qcd_ggF_uncertSF->at(7) - 1.))) / gen_sum_weights;
			
	    //if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	    // Sum yields
	    _expected_yield_THU_ggH_PT120[_current_production_mode][_current_category]           += _event_weight;
	    _expected_yield_THU_ggH_PT120_UP[_current_production_mode][_current_category]        += _event_weight_UP;
	    _expected_yield_THU_ggH_PT120_DN[_current_production_mode][_current_category]        += _event_weight_DN;
			
	    _expected_yield_THU_ggH_PT120[_current_production_mode][Settings::inclusive]         += _event_weight;
	    _expected_yield_THU_ggH_PT120_UP[_current_production_mode][Settings::inclusive]      += _event_weight_UP;
	    _expected_yield_THU_ggH_PT120_DN[_current_production_mode][Settings::inclusive]      += _event_weight_DN;
			
	    // K factors
	    _k_factor 			    = calculate_K_factor(input_file_name);
			
	    // Final event weight
	    _event_weight    = ( xsec * _k_factor           * overallEventWeight ) / gen_sum_weights;
	    _event_weight_UP = ( xsec * _k_factor * overallEventWeight * (qcd_ggF_uncertSF->at(8))) / gen_sum_weights;
	    _event_weight_DN = ( xsec * _k_factor * overallEventWeight * (1. - (qcd_ggF_uncertSF->at(8) - 1.))) / gen_sum_weights;
			
	    //if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	    // Sum yields
	    _expected_yield_THU_ggH_qmtop[_current_production_mode][_current_category]           += _event_weight;
	    _expected_yield_THU_ggH_qmtop_UP[_current_production_mode][_current_category]        += _event_weight_UP;
	    _expected_yield_THU_ggH_qmtop_DN[_current_production_mode][_current_category]        += _event_weight_DN;
			
	    _expected_yield_THU_ggH_qmtop[_current_production_mode][Settings::inclusive]         += _event_weight;
	    _expected_yield_THU_ggH_qmtop_UP[_current_production_mode][Settings::inclusive]      += _event_weight_UP;
	    _expected_yield_THU_ggH_qmtop_DN[_current_production_mode][Settings::inclusive]      += _event_weight_DN;
	}
    }		

    

    // --------------------------------------------------------------------

    //============================================================
    // read here the file with JEC_UP
    //============================================================
    
    cur_input_file_name = ChangeFolder(input_file_name, GetFolder(input_file_name) + "_JEC_UP");
    input_file = new TFile(cur_input_file_name);

    //std::cout << "currently reading JEC_UP from: " << cur_input_file_name << std::endl;

    hCounters = (TH1F*)input_file->Get("ClassTree/Counters");
    n_gen_events = (Long64_t)hCounters->GetBinContent(1);
    gen_sum_weights = (Long64_t)hCounters->GetBinContent(40);
	
    input_tree = (TTree*)input_file->Get("ClassTree/candTree");
    Init( input_tree, input_file_name );
	
    if (fChain == 0) return;

    nentries = fChain->GetEntriesFast();

    nbytes = 0, nb = 0;
	
    for (Long64_t jentry=0; jentry<nentries;jentry++)
    {
	Long64_t ientry = LoadTree(jentry);
	if (ientry < 0) break;
	nb = fChain->GetEntry(jentry);
	nbytes += nb;
		
	if ( LepEta->size() != 4 )
	{
	    cout << "[ERROR] in event " << RunNumber << ":" << LumiNumber << ":" << EventNumber << ", stored " << LepEta->size() << " leptons instead of 4" << endl;
	    continue;
	}

	if ( !(ZZsel >= 90) ) continue;
	if ( ZZMass < 105. || ZZMass > 140. ) continue;

	// Find current production mode
	gen_assoc_lep_id_.push_back(GenAssocLep1Id);
   	gen_assoc_lep_id_.push_back(GenAssocLep2Id);
	_n_gen_assoc_lep = CountAssociatedLeptons();
	_current_production_mode = find_current_production_mode( input_file_name, genExtInfo , _n_gen_assoc_lep);
	gen_assoc_lep_id_.clear();
		
	// Final states
	_current_final_state = FindFinalState();
		
	// Categories
	for ( int j = 0; j < nCleanedJetsPt30; j++)
	{
	    jetPt[j] = JetPt->at(j);
	    jetEta[j] = JetEta->at(j);
	    jetPhi[j] = JetPhi->at(j);
	    jetMass[j] = JetMass->at(j);
	    jetQGL[j] = JetQGLikelihood->at(j);
	    jetPgOverPq[j] = 1./JetQGLikelihood->at(j) - 1.;
	}
	
	_current_category_JEC_UP = refclass -> ClassifyThisEvent(this);

	// K factors
	_k_factor = calculate_K_factor(input_file_name);

	_event_weight_UP = ( xsec * _k_factor * overallEventWeight) / gen_sum_weights;
	_expected_yield_JEC_UP[_current_production_mode][_current_category_JEC_UP] += _event_weight_UP;
	_expected_yield_JEC_UP[_current_production_mode][Settings::inclusive] += _event_weight_UP;

    }

    //============================================================
    // read here the file with JEC_DN
    //============================================================

    cur_input_file_name = ChangeFolder(input_file_name, GetFolder(input_file_name) + "_JEC_DN");
    input_file = new TFile(cur_input_file_name);

    //std::cout << "currently reading JEC_DN from: " << cur_input_file_name << std::endl;

    hCounters = (TH1F*)input_file->Get("ClassTree/Counters");
    n_gen_events = (Long64_t)hCounters->GetBinContent(1);
    gen_sum_weights = (Long64_t)hCounters->GetBinContent(40);
	
    input_tree = (TTree*)input_file->Get("ClassTree/candTree");
    Init( input_tree, input_file_name );
	
    if (fChain == 0) return;

    nentries = fChain->GetEntriesFast();

    nbytes = 0, nb = 0;
	
    for (Long64_t jentry=0; jentry<nentries;jentry++)
    {
	Long64_t ientry = LoadTree(jentry);
	if (ientry < 0) break;
	nb = fChain->GetEntry(jentry);
	nbytes += nb;
		
	if ( LepEta->size() != 4 )
	{
	    cout << "[ERROR] in event " << RunNumber << ":" << LumiNumber << ":" << EventNumber << ", stored " << LepEta->size() << " leptons instead of 4" << endl;
	    continue;
	}

	if ( !(ZZsel >= 90) ) continue;
	if ( ZZMass < 105. || ZZMass > 140. ) continue;

	// Find current production mode
	gen_assoc_lep_id_.push_back(GenAssocLep1Id);
   	gen_assoc_lep_id_.push_back(GenAssocLep2Id);
	_n_gen_assoc_lep = CountAssociatedLeptons();
	_current_production_mode = find_current_production_mode( input_file_name, genExtInfo , _n_gen_assoc_lep);
	gen_assoc_lep_id_.clear();
		
	// Final states
	_current_final_state = FindFinalState();
		
	// Categories
	for ( int j = 0; j < nCleanedJetsPt30; j++)
	{
	    jetPt[j] = JetPt->at(j);
	    jetEta[j] = JetEta->at(j);
	    jetPhi[j] = JetPhi->at(j);
	    jetMass[j] = JetMass->at(j);
	    jetQGL[j] = JetQGLikelihood->at(j);
	    jetPgOverPq[j] = 1./JetQGLikelihood->at(j) - 1.;
	}

	_current_category_JEC_DN = refclass -> ClassifyThisEvent(this);
		
	// K factors
	_k_factor = calculate_K_factor(input_file_name);

	_event_weight_DN = ( xsec * _k_factor * overallEventWeight) / gen_sum_weights;
	//if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	// Sum yields
	_expected_yield_JEC_DN[_current_production_mode][_current_category_JEC_DN] += _event_weight_DN;
	_expected_yield_JEC_DN[_current_production_mode][Settings::inclusive] += _event_weight_DN;
    }

    //============================================================
    // read here the file with BTag_UP
    //============================================================

    cur_input_file_name = ChangeFolder(input_file_name, GetFolder(input_file_name) + "_BTag_UP");
    input_file = new TFile(cur_input_file_name);

    //std::cout << "currently reading BTag_UP from: " << cur_input_file_name << std::endl;

    hCounters = (TH1F*)input_file->Get("ClassTree/Counters");
    n_gen_events = (Long64_t)hCounters->GetBinContent(1);
    gen_sum_weights = (Long64_t)hCounters->GetBinContent(40);
	
    input_tree = (TTree*)input_file->Get("ClassTree/candTree");
    Init( input_tree, input_file_name );
	
    if (fChain == 0) return;

    nentries = fChain->GetEntriesFast();

    nbytes = 0, nb = 0;
	
    for (Long64_t jentry=0; jentry<nentries;jentry++)
    {
	Long64_t ientry = LoadTree(jentry);
	if (ientry < 0) break;
	nb = fChain->GetEntry(jentry);
	nbytes += nb;
		
	if ( LepEta->size() != 4 )
	{
	    cout << "[ERROR] in event " << RunNumber << ":" << LumiNumber << ":" << EventNumber << ", stored " << LepEta->size() << " leptons instead of 4" << endl;
	    continue;
	}

	if ( !(ZZsel >= 90) ) continue;
	if ( ZZMass < 105. || ZZMass > 140. ) continue;

	// Find current production mode
	gen_assoc_lep_id_.push_back(GenAssocLep1Id);
   	gen_assoc_lep_id_.push_back(GenAssocLep2Id);
	_n_gen_assoc_lep = CountAssociatedLeptons();
	_current_production_mode = find_current_production_mode( input_file_name, genExtInfo , _n_gen_assoc_lep);
	gen_assoc_lep_id_.clear();
		
	// Final states
	_current_final_state = FindFinalState();
		
	// Categories
	for ( int j = 0; j < nCleanedJetsPt30; j++)
	{
	    jetPt[j] = JetPt->at(j);
	    jetEta[j] = JetEta->at(j);
	    jetPhi[j] = JetPhi->at(j);
	    jetMass[j] = JetMass->at(j);
	    jetQGL[j] = JetQGLikelihood->at(j);
	    jetPgOverPq[j] = 1./JetQGLikelihood->at(j) - 1.;
	}

	_current_category_BTag_UP = refclass -> ClassifyThisEvent(this);

	// K factors
	_k_factor = calculate_K_factor(input_file_name);
	_event_weight_UP = ( xsec * _k_factor * overallEventWeight) / gen_sum_weights;

	_expected_yield_BTag_UP[_current_production_mode][_current_category_BTag_UP] += _event_weight_UP;
	_expected_yield_BTag_UP[_current_production_mode][Settings::inclusive] += _event_weight_UP;

    }		

    //============================================================
    // read here the file with BTag_DN
    //============================================================

    cur_input_file_name = ChangeFolder(input_file_name, GetFolder(input_file_name) + "_BTag_DN");
    input_file = new TFile(cur_input_file_name);

    //std::cout << "currently reading BTag_DN from: " << cur_input_file_name << std::endl;

    hCounters = (TH1F*)input_file->Get("ClassTree/Counters");
    n_gen_events = (Long64_t)hCounters->GetBinContent(1);
    gen_sum_weights = (Long64_t)hCounters->GetBinContent(40);
	
    input_tree = (TTree*)input_file->Get("ClassTree/candTree");
    Init( input_tree, input_file_name );
	
    if (fChain == 0) return;

    nentries = fChain->GetEntriesFast();

    nbytes = 0, nb = 0;
	
    for (Long64_t jentry=0; jentry<nentries;jentry++)
    {
	Long64_t ientry = LoadTree(jentry);
	if (ientry < 0) break;
	nb = fChain->GetEntry(jentry);
	nbytes += nb;
		
	if ( LepEta->size() != 4 )
	{
	    cout << "[ERROR] in event " << RunNumber << ":" << LumiNumber << ":" << EventNumber << ", stored " << LepEta->size() << " leptons instead of 4" << endl;
	    continue;
	}

	if ( !(ZZsel >= 90) ) continue;
	if ( ZZMass < 105. || ZZMass > 140. ) continue;

	// Find current production mode
	gen_assoc_lep_id_.push_back(GenAssocLep1Id);
   	gen_assoc_lep_id_.push_back(GenAssocLep2Id);
	_n_gen_assoc_lep = CountAssociatedLeptons();
	_current_production_mode = find_current_production_mode( input_file_name, genExtInfo , _n_gen_assoc_lep);
	gen_assoc_lep_id_.clear();
		
	// Final states
	_current_final_state = FindFinalState();
		
	// Categories
	for ( int j = 0; j < nCleanedJetsPt30; j++)
	{
	    jetPt[j] = JetPt->at(j);
	    jetEta[j] = JetEta->at(j);
	    jetPhi[j] = JetPhi->at(j);
	    jetMass[j] = JetMass->at(j);
	    jetQGL[j] = JetQGLikelihood->at(j);
	    jetPgOverPq[j] = 1./JetQGLikelihood->at(j) - 1.;
	}

	_current_category_BTag_DN = refclass -> ClassifyThisEvent(this);

	// Final event weight
	_event_weight    = ( xsec * _k_factor * overallEventWeight) / gen_sum_weights;

	_event_weight_DN = ( xsec * _k_factor * overallEventWeight) / gen_sum_weights;
	//if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS

	// Sum yields
	_expected_yield_BTag_DN[_current_production_mode][_current_category_BTag_DN] += _event_weight_DN;
	_expected_yield_BTag_DN[_current_production_mode][Settings::inclusive] += _event_weight_DN;
    }

    //cout << "[INFO] Systematics for " << input_file_name << " filled." << endl;
}

void SystematicsLI::FillSystematics_tuneUpDn(TString input_file_name)
{
    //std::cout << "SystematicsLI::FillSystematics_tuneUpDn" << std::endl;

    input_file = new TFile(input_file_name);

    hCounters = (TH1F*)input_file->Get("ClassTree/Counters");
    n_gen_events = (Long64_t)hCounters->GetBinContent(1);
    gen_sum_weights = (Long64_t)hCounters->GetBinContent(40);
    
    input_tree = (TTree*)input_file->Get("ClassTree/candTree");
    Init( input_tree, input_file_name );
    
    if (fChain == 0) return;
    
    Long64_t nentries = fChain->GetEntriesFast();
    
    Long64_t nbytes = 0, nb = 0;
	
    for (Long64_t jentry=0; jentry<nentries;jentry++)
    {
	Long64_t ientry = LoadTree(jentry);
	if (ientry < 0) break;
	nb = fChain->GetEntry(jentry);
	nbytes += nb;
	
	if ( LepEta->size() != 4 )
	{
	    cout << "[ERROR] in event " << RunNumber << ":" << LumiNumber << ":" << EventNumber << ", stored " << LepEta->size() << " leptons instead of 4" << endl;
	    continue;
	}
	
	if ( !(ZZsel >= 90) ) continue;
	if ( ZZMass < 105. || ZZMass > 140. ) continue;
	
	// Find current production mode
	gen_assoc_lep_id_.push_back(GenAssocLep1Id);
   	gen_assoc_lep_id_.push_back(GenAssocLep2Id);
	_n_gen_assoc_lep = CountAssociatedLeptons();
	_current_production_mode = find_current_production_mode( input_file_name, genExtInfo , _n_gen_assoc_lep);
	gen_assoc_lep_id_.clear();
	
	// Final states
	_current_final_state = FindFinalState();
	
	// Categories
	for ( int j = 0; j < nCleanedJetsPt30; j++)
	{
	    jetPt[j] = JetPt->at(j);
	    jetEta[j] = JetEta->at(j);
	    jetPhi[j] = JetPhi->at(j);
	    jetMass[j] = JetMass->at(j);
	    jetQGL[j] = JetQGLikelihood->at(j);
	    jetPgOverPq[j] = 1./JetQGLikelihood->at(j) - 1.;
	}

	_current_category = refclass -> ClassifyThisEvent(this);
		
	//============================================================
	// Pythia Tune
	//============================================================
	
	// K factors
	_k_factor 			    = calculate_K_factor(input_file_name);
	
	// Final event weight
	_event_weight    = ( xsec * _k_factor * overallEventWeight * ( 1. )                                                ) / gen_sum_weights;
	//if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS
	
	// Sum yields
	if(input_file_name.Contains("tuneup"))   _expected_yield_PythiaTune_UP[_current_production_mode][_current_category]        += _event_weight;
	if(input_file_name.Contains("tunedown")) _expected_yield_PythiaTune_DN[_current_production_mode][_current_category]        += _event_weight;
	
	if(input_file_name.Contains("tuneup"))   _expected_yield_PythiaTune_UP[_current_production_mode][Settings::inclusive]      += _event_weight;
	if(input_file_name.Contains("tunedown")) _expected_yield_PythiaTune_DN[_current_production_mode][Settings::inclusive]      += _event_weight;
	
    } // end for loop
    
    //cout << "[INFO] Systematics for " << input_file_name << " filled." << endl;
}
