#include <ZZAnalysis/AnalysisStep/test/Profiler/include/Profiler.h>

Profiler::Profiler():Tree()
{  }

Profiler::~Profiler()
{  }

void Profiler::FillProfile(TString input_file_name, float lumi, TH3F* hist, const std::function<bool(Tree*)>& cut, const std::function<float(Tree*)>& var_x, const std::function<float(Tree*)>& var_y, const std::function<float(Tree*)>& var_z, bool normalize)
{
    auto TH3F_callback =[&](TObject* hist, Tree* in, float weight) -> void {
	TH3F* local_hist = static_cast<TH3F*>(hist);
	local_hist -> Fill(var_x(in), var_y(in), var_z(in), weight);
    };

    FillProfile(input_file_name, lumi, hist, cut, TH3F_callback);

    // TODO: implement the "normalize" flag
}

void Profiler::FillProfile(TString input_file_name, float lumi, TH2F* hist, const std::function<bool(Tree*)>& cut, const std::function<float(Tree*)>& var_x, const std::function<float(Tree*)>& var_y, bool normalize)
{
    auto TH2F_callback = [&](TObject* hist, Tree* in, float weight) -> void {
	TH2F* local_hist = static_cast<TH2F*>(hist);
	local_hist -> Fill(var_x(in), var_y(in), weight);
    };

    FillProfile(input_file_name, lumi, hist, cut, TH2F_callback);

    // TODO: implement the "normalize" flag
}

void Profiler::FillProfile(TString input_file_name, float lumi, TH1F* hist, const std::function<bool(Tree*)>& cut, const std::function<float(Tree*)>& var, bool normalize)
{
    auto TH1F_callback = [&](TObject* hist, Tree* in, float weight) -> void {
	TH1F* local_hist = static_cast<TH1F*>(hist);
	local_hist -> Fill(var(in), weight);
    };
    
    FillProfile(input_file_name, lumi, hist, cut, TH1F_callback);
    
    if(normalize)
    {
	hist -> Scale(1.0 / hist -> Integral("width"));
    }
}

void Profiler::FillDataWeights(TString input_file_name, float lumi, const std::function<bool(Tree*)>& cut, const std::function<float(Tree*)>& var, std::vector<double>* data_vec, std::vector<double>* weight_vec)
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
    for(Long64_t j_entry = 0; j_entry < n_entries; j_entry++)
    {
	// get the correct tree in the chain that contains this event
	Long64_t i_entry = LoadTree(j_entry);
	if(i_entry < 0) break;

	// now actually read this entry
	fChain -> GetEntry(j_entry);

	float event_weight = (lumi * xsec * 1000. * overallEventWeight) / gen_sum_weights;
	
	if(cut(this))
	{
	    data_vec -> push_back(var(this));
	    weight_vec -> push_back(event_weight);
	}
    }    

    delete input_tree;
    delete input_file;
}

void Profiler::FillProfile(TString input_file_name, float lumi, TObject* hist, const std::function<bool(Tree*)>& cut, const std::function<void(TObject*, Tree*, float)>& fill_callback)
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

    float weight_sum = 0;
    int fill_cnt = 0;

    // loop over the entries in chain
    for(Long64_t j_entry = 0; j_entry < n_entries; j_entry++)
    {
	// get the correct tree in the chain that contains this event
	Long64_t i_entry = LoadTree(j_entry);
	if(i_entry < 0) break;

	// now actually read this entry
	fChain -> GetEntry(j_entry);

	float event_weight = (lumi * xsec * 1000. * overallEventWeight) / gen_sum_weights;

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

