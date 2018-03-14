#include <ZZAnalysis/AnalysisStep/test/classlib/include/ROCPlotter.h>

ROCPlotter::ROCPlotter(Config& conf)
{  
    this -> lumi = conf.lumi();
}

ROCPlotter::~ROCPlotter()
{  }

void ROCPlotter::AddROCCurve(std::vector<TString> H0_files, std::vector<TString> H1_files, const std::function<float(Tree*)>& disc, const std::function<bool(Tree*)>& cut, TString H0_desc, TString H1_desc, TString disc_name)
{
    disc_values.clear();
    weight_values.clear();
    true_values.clear();

    // first, need to evaluate the discriminant on all events contained in the two files: the one consisting purely of H0 events, and the other with H1 events only
    for(auto& H0_file: H0_files)
    {
	IterateThroughFile(H0_file, false, disc, cut);
    }

    for(auto& H1_file: H1_files)
    {
	IterateThroughFile(H1_file, true, disc, cut);
    }
        
    ROCGenerator rg;
    rg.GenerateROC(disc_values, true_values, weight_values, 100);

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
    //auc.push_back(roc.GetROCIntegral(50));
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

    TLegend* leg = new TLegend(0.6, 0.2 + 0.07 * graphs.size(), 0.9, 0.2);
    leg -> SetTextColor(kBlack);
    leg -> SetTextSize(0.035);
    leg -> SetBorderSize(0);
    leg -> SetFillStyle(0);

    for(unsigned int i = 0; i < graphs.size(); i++)
    {
    	tmg -> Add(graphs.at(i));
	leg -> AddEntry(graphs.at(i), names.at(i), "l");
    }

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
    leg -> Draw();

    canv -> cd();
    pad -> Draw();
    gPad -> RedrawAxis();
    canv -> Update();

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

}
