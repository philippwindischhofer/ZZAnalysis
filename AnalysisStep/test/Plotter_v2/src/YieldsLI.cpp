// Include classes
#include <ZZAnalysis/AnalysisStep/test/Plotter_v2/include/YieldsLI.h>

// Constructor
//=======================
YieldsLI::YieldsLI( double lumi ) : Yields(lumi)
{

}
//============================================================



// Destructor
//====================
YieldsLI::~YieldsLI()
{

}
//====================

void YieldsLI::SetPackagePath(TString package_path, TString engine)
{
    config_file_path = package_path + "settings.conf";
    calibration_dir = package_path + "calibration/";
    priors_file_path = package_path + "priors_" + engine + "/priors_bkg.txt";

    refclass = new Mor18Classifier();
    refclass = new Mor18LIClassifier(calibration_dir, config_file_path, engine);

    // load priors here and assign them etc.
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


//=====================================================
void YieldsLI::MakeHistograms( TString input_file_name )
{

    std::cout << "YieldsLI Histogramming" << std::endl;

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

      // Find current process
      gen_assoc_lep_id_.push_back(GenAssocLep1Id);
      gen_assoc_lep_id_.push_back(GenAssocLep2Id);
      _n_gen_assoc_lep = CountAssociatedLeptons();
      _current_process = find_current_process( input_file_name, genExtInfo , _n_gen_assoc_lep);
      gen_assoc_lep_id_.clear();
      
      // Final states
      _current_final_state = FindFinalState();
      _current_category = refclass -> ClassifyThisEvent(this);
      
      // K factors
      if ( APPLY_K_FACTORS ) _k_factor = calculate_K_factor(input_file_name);

      // Final event weight
      _event_weight = (_lumi * 1000 * xsec * _k_factor * overallEventWeight) / gen_sum_weights;

      if ( input_file_name.Contains("ggH") ) _event_weight *= ggH_NNLOPS_weight; // reweight POWHEG ggH to NNLOPS
   
      // Fill M4l histograms
       yields_histos->FillYields( ZZMass, _event_weight, _current_final_state, _current_category, _current_process );
   } // end for loop
   
   cout << "[INFO] Histograms for " << input_file_name << " filled." << endl;
}
//=====================================================



//===============================================================================
void YieldsLI::Calculate_SS_ZX_Yields( TString input_file_data_name, TString  input_file_FR_name )
{
    std::cout << "YieldsLI SSZX" << std::endl;

   FakeRates *FR = new FakeRates( input_file_FR_name );
   
   input_file_data = new TFile(input_file_data_name);
   input_tree_data = (TTree*)input_file_data->Get("ClassTree/candTree");
   Init( input_tree_data, input_file_data_name );
      
   if (fChain == 0) return;
   
   Long64_t nentries = fChain->GetEntriesFast();

   std::cout << "have " << nentries << " events" << std::endl;
   
   Long64_t nbytes = 0, nb = 0;
   
   for (Long64_t jentry=0; jentry<nentries;jentry++)
   {
      
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
      
      if ( !CRflag ) continue;
      if ( !test_bit(CRflag, CRZLLss) ) continue;
      
      _current_final_state = FindFinalStateZX();            
      _current_category = refclass -> ClassifyThisEvent(this);
      
      // Calculate yield
      _yield_SR = overallEventWeight * _fs_ROS_SS.at(_current_final_state)*FR->GetFakeRate(LepPt->at(2),LepEta->at(2),LepLepId->at(2))*FR->GetFakeRate(LepPt->at(3),LepEta->at(3),LepLepId->at(3));
      
      _expected_yield_SR[_current_final_state][_current_category] += _yield_SR; // this number needs to be used when renormalizing histograms that have some cut/blinding
      _number_of_events_CR[_current_final_state][_current_category]++;
      
   } // END events loop
   
   for (  int i_cat = 0; i_cat < num_of_categories - 1; i_cat++  )
   {
      for ( int i_fs = 0; i_fs < num_of_final_states - 1; i_fs++  )
      {
         _expected_yield_SR[Settings::fs4l][i_cat]       += _expected_yield_SR[i_fs][i_cat];   //calculate expected yield for inclusive 4l final state
         _number_of_events_CR[Settings::fs4l][i_cat]     += _number_of_events_CR[i_fs][i_cat];
         _expected_yield_SR[i_fs][Settings::inclusive]   += _expected_yield_SR[i_fs][i_cat];   //calculate expected yield for inclusive category
         _number_of_events_CR[i_fs][Settings::inclusive] += _number_of_events_CR[i_fs][i_cat];
         
         if ( MERGE_2E2MU )
         {
            _expected_yield_SR[Settings::fs2e2mu][i_cat]       += _expected_yield_SR[Settings::fs2mu2e][i_cat];   //merge 2e2mu and 2mu2e final state
            _number_of_events_CR[Settings::fs2e2mu][i_cat]     += _number_of_events_CR[Settings::fs2mu2e][i_cat];
            _expected_yield_SR[Settings::fs2mu2e][i_cat]        = 0.;
            _number_of_events_CR[Settings::fs2mu2e][i_cat]      = 0.;
         }
         
      }
   }
   for ( int i_fs = 0; i_fs < num_of_final_states - 1; i_fs++  )
   {
      _expected_yield_SR[Settings::fs4l][Settings::inclusive] += _expected_yield_SR[i_fs][Settings::inclusive];
   }
   
   // Print Z + X expected yields for inclusive category
   cout << endl;
   cout << "========================================================================================" << endl;
   cout << "[INFO] Control printout." << endl << "!!! Numbers shoud be identical to yields from SS method !!!" << endl;
   for ( int i_fs = 0; i_fs < num_of_final_states - 1; i_fs++ )
   {
      if ( MERGE_2E2MU && i_fs == Settings::fs2mu2e) continue;
      cout << "Category: " << Settings::inclusive << "   Final state: " << i_fs << endl;
      cout << _expected_yield_SR[i_fs][Settings::inclusive] << " +/- " <<
      _expected_yield_SR[i_fs][Settings::inclusive]/sqrt(_number_of_events_CR[i_fs][Settings::inclusive]) << " (stat., evt: " <<
      _number_of_events_CR[i_fs][Settings::inclusive] << ")" << " +/- " << _expected_yield_SR[i_fs][Settings::inclusive]*0.50 << " (syst.)" << endl;
   }
   
   cout << "[INFO] Total = " << _expected_yield_SR[Settings::fs4l][Settings::inclusive] << endl;
   cout << "========================================================================================" << endl;
   cout << endl;
   
   cout << "[INFO] Z+X yields calculated using SS method." << endl;
}
//===============================================================================
