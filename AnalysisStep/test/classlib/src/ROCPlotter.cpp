#include <ZZAnalysis/AnalysisStep/test/classlib/include/ROCPlotter.h>

ROCPlotter::ROCPlotter(Config& conf, float start_fraction, float end_fraction)
{  
    this -> lumi = conf.lumi();
    this -> start_fraction = start_fraction;
    this -> end_fraction = end_fraction;

    this -> label_left = "";
    this -> label_right = "";
}

ROCPlotter::~ROCPlotter()
{  }

void ROCPlotter::AddROCCurve(Discriminant* disc, TString H0_desc, TString H1_desc, TString disc_name)
{
    AddROCCurve(disc, H0_desc, H1_desc, disc_name, std::vector<float>());
}

void ROCPlotter::AddROCCurve(Discriminant* disc, TString H0_desc, TString H1_desc, TString disc_name, std::vector<float> marker_thresholds)
{
    EventStream* H0_stream = disc -> GetH0Source();
    EventStream* H1_stream = disc -> GetH1Source();

    auto disc_wrapper = [&](Tree* in) -> float {
	return disc -> Evaluate(in);
    };

    AddROCCurve(H0_stream, H1_stream, disc_wrapper, H0_desc, H1_desc, disc_name, marker_thresholds);
}

void ROCPlotter::AddROCCurve(std::vector<TString> H0_files, std::vector<TString> H1_files, const std::function<float(Tree*)>& disc, const std::function<bool(Tree*)>& cut, TString H0_desc, TString H1_desc, TString disc_name, std::vector<float> marker_thresholds)
{
    // build the intermediate-level objects and call the other signature
    EventStream* H0_stream = new EventStream();
    EventStream* H1_stream = new EventStream();

    for(auto& H0_file: H0_files)
    {
	H0_stream -> AddEventSource(H0_file, cut);
    }
    
    for(auto& H1_file: H1_files)
    {
	H1_stream -> AddEventSource(H1_file, cut);
    }

    AddROCCurve(H0_stream, H1_stream, disc, H0_desc, H1_desc, disc_name, marker_thresholds);
}

void ROCPlotter::AddROCCurve(EventStream* H0_stream, EventStream* H1_stream, const std::function<float(Tree*)>& disc, TString H0_desc, TString H1_desc, TString disc_name, std::vector<float> marker_thresholds)
{
    disc_values.clear();
    weight_values.clear();
    true_values.clear();

    // first, need to evaluate the discriminant on all events contained in the two files: the one consisting purely of H0 events, and the other with H1 events only
    std::vector<TString> H0_files = H0_stream -> GetPaths();
    std::vector<std::function<bool(Tree*)>> H0_cuts = H0_stream -> GetCuts();
    for(auto tup: boost::combine(H0_files, H0_cuts))
    {
	TString H0_file;
	std::function<bool(Tree*)> cut;
	boost::tie(H0_file, cut) = tup;

	IterateThroughFile(H0_file, false, disc, cut);
    }

    std::vector<TString> H1_files = H1_stream -> GetPaths();
    std::vector<std::function<bool(Tree*)>> H1_cuts = H1_stream -> GetCuts();
    for(auto tup: boost::combine(H1_files, H1_cuts))
    {
	TString H1_file;
	std::function<bool(Tree*)> cut;
	boost::tie(H1_file, cut) = tup;

	IterateThroughFile(H1_file, true, disc, cut);
    }
        
    ROCGenerator rg;
    rg.GenerateROC(disc_values, true_values, weight_values, 100);

    // add the marker_thresholds:
    for(float cur: marker_thresholds)
    {
	std::pair<float, float> marker_coords = rg.GenerateROCPoint(disc_values, true_values, weight_values, cur);
	TMarker* tm = new TMarker(marker_coords.second, marker_coords.first, marker_styles[markers.size()]);
	std::cout << marker_coords.first << " / " << marker_coords.second << std::endl;
	tm -> SetMarkerColor(ROC_colors[graphs.size()]);
	tm -> SetMarkerSize(3);
	markers.push_back(tm);
    }

    float* H0_eff = rg.GetH0Efficiency();
    float* H1_eff = rg.GetH1Efficiency();
    int num_pts = rg.GetNumPoints();

    TGraph* tg = new TGraph(num_pts, H0_eff, H1_eff);

    tg -> GetXaxis() -> SetRangeUser(0.0, 1.0);
    tg -> GetYaxis() -> SetRangeUser(0.0, 1.0);
    xlabel = H0_desc;
    ylabel = H1_desc;
    tg -> SetLineColor(ROC_colors[graphs.size()]);

    graphs.push_back(tg);
    names.push_back(disc_name);
    auc.push_back(rg.GetAUC());
}

std::vector<float> ROCPlotter::GetAUC()
{
    return auc;
}

// adds a single file to the queue on which the ROC is going to be evaluated
void ROCPlotter::IterateThroughFile(TString input_file_name, bool truth, const std::function<float(Tree*)>& disc, const std::function<bool(Tree*)>& cut)
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

    // determine the weights for the reduced part w.r.t. the full dataset *exactly*
    float full_gen_sum_weights = 0.0;
    float reduced_gen_sum_weights = 0.0;

    for(Long64_t j_entry = (Long64_t)(n_entries * start_fraction); j_entry < (Long64_t)(n_entries * end_fraction); j_entry++)
    {
	LoadTree(j_entry);
	fChain -> GetEntry(j_entry);
	reduced_gen_sum_weights += overallEventWeight;	
    }

    full_gen_sum_weights = reduced_gen_sum_weights;

    // for the full weight sum, need to add the part in the beginning
    for(Long64_t j_entry = 0; j_entry < (Long64_t)(n_entries * start_fraction); j_entry++)
    {
	LoadTree(j_entry);
	fChain -> GetEntry(j_entry);
	full_gen_sum_weights += overallEventWeight;
    }

    // and in the end
    for(Long64_t j_entry = (Long64_t)(n_entries * end_fraction); j_entry < n_entries; j_entry++)
    {
	LoadTree(j_entry);
	fChain -> GetEntry(j_entry);
	full_gen_sum_weights += overallEventWeight;
    }

    float reweighting_factor = reduced_gen_sum_weights / full_gen_sum_weights;

    // loop over the entries in chain
    for(Long64_t j_entry = (Long64_t)(n_entries * start_fraction); j_entry < (Long64_t)(n_entries * end_fraction); j_entry++)
    {
    	// get the correct tree in the chain that contains this event
    	Long64_t i_entry = LoadTree(j_entry);
	if(i_entry < 0) break;

    	// now actually read this entry
    	fChain -> GetEntry(j_entry);

    	float event_weight = (lumi * xsec * 1000. * overallEventWeight) / (gen_sum_weights * reweighting_factor);
	
    	if(cut(this))
    	{
    	    weight_values.push_back(event_weight);
    	    true_values.push_back(truth);
    	    disc_values.push_back(disc(this));
    	}
    }        
}

void ROCPlotter::Construct()
{
    // construct the plot
    canv = new TCanvas("canv", "canv", 800, 800);
    pad = new TPad("pad", "pad", 0.0, 0.0, 1.0, 1.0, kWhite, 0, 0);
    pad -> SetLeftMargin(0.1);
    pad -> SetRightMargin(0.1);
    pad -> SetTopMargin(0.1);
    pad -> SetBottomMargin(0.1);
    tmg = new TMultiGraph();

    TGaxis::SetMaxDigits(2);

    leg = new TLegend(0.6, 0.2 + 0.07 * graphs.size(), 0.9, 0.2);
    leg -> SetTextColor(kBlack);
    leg -> SetTextSize(0.035);
    leg -> SetBorderSize(0);
    leg -> SetFillStyle(0);

    for(unsigned int i = 0; i < graphs.size(); i++)
    {
    	tmg -> Add(graphs.at(i));
	leg -> AddEntry(graphs.at(i), names.at(i), "l");
    }

    Redraw();
}

void ROCPlotter::SetLabel(TString label_left, TString label_right)
{
    this -> label_left = label_left;
    this -> label_right = label_right;
    Redraw();
}

void ROCPlotter::SaveAs(TString path)
{
    canv -> SaveAs(path);
    Reset();
}

void ROCPlotter::Reset()
{
    disc_values.clear();
    weight_values.clear();
    true_values.clear();

    for(auto* graph: graphs)
    {
	delete graph;
    }

    graphs.clear();
    auc.clear();
}

void ROCPlotter::Redraw()
{
    canv -> cd();
    pad -> Draw();
    pad -> cd();

    tmg -> Draw("AC");
    gPad -> SetTickx();
    gPad -> SetTicky();
    tmg -> GetXaxis() -> SetRangeUser(0.0, 1.0);
    tmg -> GetYaxis() -> SetRangeUser(0.0, 1.0);
    tmg -> GetXaxis() -> SetNdivisions(10, 5, 0, kFALSE);
    tmg -> GetYaxis() -> SetNdivisions(10, 5, 0, kFALSE);
    tmg -> GetXaxis() -> SetTitle(xlabel);
    tmg -> GetYaxis() -> SetTitle(ylabel);
    pad -> SetGrid();
    
    tmg -> Draw("AC");

    // draw markers
    for(auto marker: markers)
    {
	std::cout << "drawing marker" << std::endl;
	marker -> Draw("same");
    }

    leg -> Draw();

    TLatex* Tl = new TLatex();
    Tl -> SetTextSize(0.025);
    Tl -> SetTextColor(kBlack);
    float upper_bound = (pad -> GetUymax() - pad -> GetUymin()) * 1.02 + pad -> GetUymin();
    float side_length = pad -> GetUxmax() - pad -> GetUxmin();
    Tl -> SetTextAlign(31);
    Tl -> DrawLatex(pad -> GetUxmax() - 0.01 * side_length, upper_bound, label_right);
    Tl -> SetTextAlign(11);
    Tl -> DrawLatex(pad -> GetUxmin() + 0.01 * side_length, upper_bound, label_left);

    canv -> cd();
    pad -> Draw();

    gPad -> RedrawAxis();
    canv -> Update();
}
