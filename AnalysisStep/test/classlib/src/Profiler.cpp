#include <ZZAnalysis/AnalysisStep/test/classlib/include/Profiler.h>

Profiler::Profiler():Tree()
{  }

Profiler::~Profiler()
{  }

// for TH3F
void Profiler::FillProfile(TString input_file_name, float lumi, TH3F* hist, const std::function<bool(Tree*)>& cut, const std::function<float(Tree*)>& var_x, const std::function<float(Tree*)>& var_y, const std::function<float(Tree*)>& var_z, bool normalize, float start_fraction, float end_fraction)
{
    auto TH3F_callback =[&](TObject* hist, Tree* in, float weight) -> void {
	TH3F* local_hist = static_cast<TH3F*>(hist);
	local_hist -> Fill(var_x(in), var_y(in), var_z(in), weight);
    };

    FillProfile(input_file_name, lumi, hist, cut, TH3F_callback, start_fraction, end_fraction);

    // TODO: implement the "normalize" flag
}

// for TH2F
void Profiler::FillProfile(TString input_file_name, float lumi, TH2F* hist, const std::function<bool(Tree*)>& cut, const std::function<float(Tree*)>& var_x, const std::function<float(Tree*)>& var_y, bool normalize, float start_fraction, float end_fraction)
{
    auto TH2F_callback = [&](TObject* hist, Tree* in, float weight) -> void {
	TH2F* local_hist = static_cast<TH2F*>(hist);
	local_hist -> Fill(var_x(in), var_y(in), weight);
    };

    FillProfile(input_file_name, lumi, hist, cut, TH2F_callback, start_fraction, end_fraction);

    // TODO: implement the "normalize" flag
}

// for TH1F
TH1F* Profiler::FillProfile(TString input_file_name, float lumi, TH1F* hist, const std::function<bool(Tree*)>& cut, const std::function<float(Tree*)>& var, bool normalize, float start_fraction, float end_fraction, bool fast_reweighting, bool MC_weighting)
{
    TH1F* hist_fillcounts = (TH1F*)(hist -> Clone("hist_fillcounts"));
    hist_fillcounts -> Reset();

    auto TH1F_callback = [&](TObject* hist, Tree* in, float weight) -> void {
	TH1F* local_hist = static_cast<TH1F*>(hist);
	float fill_var = var(in);

	if(MC_weighting)
	{
	    local_hist -> Fill(fill_var, weight);
	}
	else
	{
	    local_hist -> Fill(fill_var);
	}

	hist_fillcounts -> Fill(fill_var); // keep the copied histogram in sync, but without any weights attached to it
    };
    
    if(input_file_name.Contains("Data"))
    {
	TString fake_rate_file_name = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/data/FakeRates/FakeRates_SS_Moriond18.root";
	// TODO: remove the mZZ_cut (if existing) in "cut" and pass the resulting function on! Right now it works only if mZZ_cut is the ONLY cut that is passed to this function from the outside world (i.e. NO cut in final state is currently possible)
	FillProfileData(input_file_name, fake_rate_file_name, hist, no_cut, TH1F_callback, start_fraction, end_fraction);
    }
    else
    {
	FillProfile(input_file_name, lumi, hist, cut, TH1F_callback, start_fraction, end_fraction, fast_reweighting);
    }
    
    if(normalize)
	hist -> Scale(1.0 / hist -> Integral("width"));

    return hist_fillcounts;
}

void Profiler::FillDataWeights(TString input_file_name, float lumi, const std::function<bool(Tree*)>& cut, const std::function<float(Tree*)>& var, std::vector<double>* data_vec, std::vector<double>* weight_vec, float start_fraction, float end_fraction)
{
    input_file = new TFile(input_file_name);
    
    std::cout << "reading from " << input_file_name << std::endl;
    
    // read some auxiliary information
    hCounters = (TH1F*)input_file -> Get("ZZTree/Counters");
    n_gen_events = (Long64_t)hCounters -> GetBinContent(1);
    gen_sum_weights = (Long64_t)hCounters -> GetBinContent(40);

    input_tree = (TTree*)input_file -> Get("ZZTree/candTree");

    Init(input_tree, input_file_name);
    
    if(fChain == 0) return;

    // total number of entries stored in the entire chain
    Long64_t n_entries = fChain -> GetEntriesFast();
    std::cout << "total number of entries = " << n_entries << std::endl;

    // loop over the entries in chain
    for(Long64_t j_entry = (Long64_t)(n_entries * start_fraction); j_entry < (Long64_t)(n_entries * end_fraction); j_entry++)
    {
	// get the correct tree in the chain that contains this event
	Long64_t i_entry = LoadTree(j_entry);
	if(i_entry < 0) break;

	// now actually read this entry
	fChain -> GetEntry(j_entry);

	float event_weight = (lumi * xsec * 1000. * overallEventWeight) / (gen_sum_weights * (end_fraction - start_fraction));
	
	if(cut(this))
	{
	    data_vec -> push_back(var(this));
	    weight_vec -> push_back(event_weight);
	}
    }    

    delete input_tree;
    delete input_file;
}

double Profiler::ComputeKFactor(TString input_file_name, Tree* in)
{
    float retval = 1.0;

    if(input_file_name.Contains("ZZTo4l"))
    {
	retval = (in -> KFactor_EW_qqZZ) * (in -> KFactor_QCD_qqZZ_M); // As of Moriond2016
    }
    else if(input_file_name.Contains("ggTo"))
    {
	retval = in -> KFactor_QCD_ggZZ_Nominal; // as of Moriond2016
    }

    return retval;
}

// this iterates over the CR data and compute the expected Z+X yield in the SR
void Profiler::FillProfileData(TString input_file_name, TString input_file_FR_name, TObject* hist, const std::function<bool(Tree*)>& cut, const std::function<void(TObject*, Tree*, float)>& fill_callback, float start_fraction, float end_fraction)
{
    M4lZX* scaler = new M4lZX();
    scaler -> ComputeLandauScalings(mZZ_cut_min(), mZZ_cut_max());
    
    vector<float> fs_ROS_SS;
    fs_ROS_SS.push_back(1.01005); //4e
    fs_ROS_SS.push_back(1.05217); //4mu
    fs_ROS_SS.push_back(1.0024); //2e2mu
    fs_ROS_SS.push_back(1.0052); //2mu2e
    
    input_file = new TFile(input_file_name);

    FakeRates* FR = new FakeRates(input_file_FR_name);
    
    TString tree_name = "CRZLLTree";

    std::cout << "reading DATA from " << input_file_name << std::endl;

    input_tree = (TTree*)input_file -> Get(tree_name + "/candTree");
    Init(input_tree, input_file_name);
    
    if(fChain == 0) return;

    // total number of entries stored in the entire chain
    Long64_t n_entries = fChain -> GetEntriesFast();
    std::cout << "total number of entries = " << n_entries << std::endl;
    std::cout << "reweighting_factor = " << end_fraction - start_fraction << std::endl;

    float weight_sum = 0;
    Long64_t fill_cnt = 0;

    // loop over the entries in chain
    for(Long64_t j_entry = (Long64_t)(n_entries * start_fraction); j_entry < (Long64_t)(n_entries * end_fraction); j_entry++)
    {
	// get the correct tree in the chain that contains this event
	Long64_t i_entry = LoadTree(j_entry);
	if(i_entry < 0) break;

	// now actually read this entry
	fChain -> GetEntry(j_entry);

	// take only events that really *are* in the SS-CR!
	int CRZLLss = 21;
	if (!CRflag) continue;
	if (!test_bit(CRflag, CRZLLss)) continue;
      
	int cur_final_state = FinalState::FindFinalStateZX(this);

	// look up the fake rate for this event, i.e. the weight with which it is expected to contribute to the yield in the signal region <-> this plays the role of the MC event weight
	float yield_SR = fs_ROS_SS.at(cur_final_state) * 
	    FR -> GetFakeRate(LepPt -> at(2), LepEta -> at(2), LepLepId -> at(2)) * 
	    FR -> GetFakeRate(LepPt -> at(3), LepEta -> at(3), LepLepId -> at(3));

	// scale it down to the signal region mass window via the Landau scaling...
	yield_SR *= scaler -> GetLandauScaling(cur_final_state);

	// ... which works only if "cut" below does not contain any mass_cut itself!

	// scale it by the portion of the file actually read, to keep the effective luminosity the same, even if only part of the file was used
	float event_weight = yield_SR / (end_fraction - start_fraction);

	if(cut(this))
	{
	    fill_callback(hist, this, event_weight);
	    weight_sum += event_weight;
	    fill_cnt++;
	}	
    }

    std::cout << "weight_sum = " << weight_sum << std::endl;
    std::cout << "fill_cnt = " << fill_cnt << std::endl;
    
    delete input_tree;
    delete input_file;
}

void Profiler::FillProfile(TString input_file_name, float lumi, TObject* hist, const std::function<bool(Tree*)>& cut, const std::function<void(TObject*, Tree*, float)>& fill_callback, float start_fraction, float end_fraction, bool fast_reweighting)
{
    input_file = new TFile(input_file_name);

    TString tree_name = "ZZTree";
    
    std::cout << "reading from " << input_file_name << std::endl;
    
    // read some auxiliary information
    hCounters = (TH1F*)input_file -> Get(tree_name + "/Counters");
    n_gen_events = (Long64_t)hCounters -> GetBinContent(1);
    gen_sum_weights = (Long64_t)hCounters -> GetBinContent(40);

    input_tree = (TTree*)input_file -> Get(tree_name + "/candTree");

    Init(input_tree, input_file_name);
    
    if(fChain == 0) return;

    // total number of entries stored in the entire chain
    Long64_t n_entries = fChain -> GetEntriesFast();
    std::cout << "total number of entries = " << n_entries << std::endl;

    float weight_sum = 0;
    int fill_cnt = 0;

    std::cout << "start = " << (Long64_t)(n_entries * start_fraction) << std::endl;
    std::cout << "end = " << (Long64_t)(n_entries * end_fraction) << std::endl;

    float reweighting_factor = 0.0;

    if(fast_reweighting)
    {
	reweighting_factor = end_fraction - start_fraction;
    }
    else
    {
	// determine the weights for the reduced part w.r.t. the full dataset *exactly*
	float full_gen_sum_weights = 0.0;
	float reduced_gen_sum_weights = 0.0;

	for(Long64_t j_entry = (Long64_t)(n_entries * start_fraction); j_entry < (Long64_t)(n_entries * end_fraction); j_entry++)
	{
	    LoadTree(j_entry);
	    fChain -> GetEntry(j_entry);
	    reduced_gen_sum_weights += overallEventWeight * ComputeKFactor(input_file_name, this);	
	}

	full_gen_sum_weights = reduced_gen_sum_weights;

	// for the full weight sum, need to add the part in the beginning
	for(Long64_t j_entry = 0; j_entry < (Long64_t)(n_entries * start_fraction); j_entry++)
	{
	    LoadTree(j_entry);
	    fChain -> GetEntry(j_entry);
	    full_gen_sum_weights += overallEventWeight * ComputeKFactor(input_file_name, this);
	}

	// and in the end
	for(Long64_t j_entry = (Long64_t)(n_entries * end_fraction); j_entry < n_entries; j_entry++)
	{
	    LoadTree(j_entry);
	    fChain -> GetEntry(j_entry);
	    full_gen_sum_weights += overallEventWeight * ComputeKFactor(input_file_name, this);
	}

	reweighting_factor = reduced_gen_sum_weights / full_gen_sum_weights;
    }

    std::cout << "reweighting_factor = " << reweighting_factor << std::endl;

    // loop over the entries in chain
    for(Long64_t j_entry = (Long64_t)(n_entries * start_fraction); j_entry < (Long64_t)(n_entries * end_fraction); j_entry++)
    {
	// get the correct tree in the chain that contains this event
	Long64_t i_entry = LoadTree(j_entry);
	if(i_entry < 0) break;

	// now actually read this entry
	fChain -> GetEntry(j_entry);

	if ( !(ZZsel >= 90) ) continue;
	
	float event_weight = (lumi * xsec * 1000. * overallEventWeight * ComputeKFactor(input_file_name, this)) / (gen_sum_weights * reweighting_factor);

	if(input_file_name.Contains("ggH"))
	{
	    event_weight *= ggH_NNLOPS_weight;
	}

	if(cut(this))
	{
	    fill_callback(hist, this, event_weight);
	    weight_sum += event_weight;
	    fill_cnt++;
	}
    }

    std::cout << "gen_sum_weights = " << gen_sum_weights << std::endl;
    std::cout << "weight_sum = " << weight_sum << std::endl;
    std::cout << "fill_cnt = " << fill_cnt << std::endl;

    delete input_tree;
    delete input_file;
}

