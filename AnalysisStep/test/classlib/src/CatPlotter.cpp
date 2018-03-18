#include <ZZAnalysis/AnalysisStep/test/classlib/include/CatPlotter.h>

CatPlotter::CatPlotter()
{  }

CatPlotter::~CatPlotter()
{  }

void CatPlotter::Construct(std::map<TString, TH1F*> histmap, Config* conf, std::vector<float> yields, TString title)
{
    std::vector<TH1F*> hists;
    std::vector<TString> source_labels;

    std::vector<TString> cat_labels = conf -> ordered_cat_labels();

    for(auto hist_name: conf -> hist_names())
    {
	std::map<TString, TH1F*>::iterator it = histmap.find(hist_name);

	if(it != histmap.end())
	{
	    std::cout << "plotter::construct" << hist_name << std::endl;
	    hists.push_back(histmap[hist_name]);
	    source_labels.push_back(conf -> source_label(hist_name));
	}
    }

    float lumi = conf -> lumi();

    Construct(hists, cat_labels, source_labels, yields, title, lumi);
}

void CatPlotter::Construct(std::vector<TH1F*> hists, std::vector<TString> cat_labels, std::vector<TString> source_labels, std::vector<float> yields, TString title, float lumi)
{
    this -> cat_labels = cat_labels;
    this -> source_labels = source_labels;
    this -> yields = yields;
    this -> lumi = lumi;
    this -> title = title;

    draw_legend = (source_labels.size() > 0) ? true : false;

    this -> hs = new THStack("hs","");

    for(unsigned int i = 0; i < hists.size(); i++)
    {
	hists[i] -> SetBarOffset(0.15);
	hists[i] -> SetBarWidth(0.7);
    }

    for(unsigned int i = 0; i < hists.size(); i++)
    {
	hs -> Add(hists[i]);
	hists[i] -> GetXaxis() -> SetNdivisions(cat_labels.size(), 0, 0, kFALSE);
	hists[i] -> GetYaxis() -> SetNdivisions(10, 2, 0, kTRUE);    
	hists[i] -> GetYaxis() -> SetTickLength(0.01);
	hists[i] -> GetXaxis() -> SetTickLength(0.01);    
    }

    canv = new TCanvas("canv", "canv", 10, 10, 800, 600);

    // depending on whether want to draw a legend, need to divide the canvas differently
    if(draw_legend)
    {
	pad1 = new TPad("pad1", "pad1", 0.0, 0.0, 0.8, 1.0, kWhite, 0, 0);
	pad2 = new TPad("pad2", "pad2", 0.80, 0.0, 1.0, 1.0, kWhite, 0, 0);
    }
    else
    {
	pad1 = new TPad("pad1", "pad1", 0.0, 0.0, 0.95, 1.0, kWhite, 0, 0);
	pad2 = new TPad("pad2", "pad2", 0.95, 0.0, 1.0, 1.0, kWhite, 0, 0);
    }

    pad1 -> SetLeftMargin(0.25);
    pad1 -> SetTicks(1, 1);
    pad1 -> SetRightMargin(0.03);
    pad1 -> SetFillColorAlpha(kWhite, 0.0);
    pad2 -> SetLeftMargin(0.0);
    pad2 -> SetFillColorAlpha(kWhite, 0.0);

    leg = new TLegend(0.0, 0.9 - 0.4 / 10 * (hists.size()), 0.5, 0.9);
    for(unsigned int i = 0; i < source_labels.size(); i++)
    {
	leg -> AddEntry(hists[i], source_labels[i], "f");
    } 

    Redraw();
}

void CatPlotter::AddLine(double coord)
{
    line_coords.push_back(coord);
    Redraw();
}

void CatPlotter::DrawLabel(TString label)
{
    draw_label = true;
    this -> label = label;
    Redraw();
}

void CatPlotter::Redraw()
{
    canv -> cd();
    pad1 -> Draw();
    pad2 -> Draw(); 

    pad1 -> cd();
    hs -> Draw("hist hbar");
    gStyle->SetHistTopMargin(0.);
    hs -> GetYaxis() -> SetTitle(title);
    hs -> GetYaxis() -> SetTitleOffset(0.95);
    hs -> GetHistogram() -> GetXaxis() -> SetNdivisions(cat_labels.size(), 0, 0, kFALSE);
    hs -> GetHistogram() -> GetYaxis() -> SetNdivisions(10, 2, 0, kTRUE);
    hs -> GetHistogram() -> GetYaxis() -> SetTickLength(0.01);
    hs -> GetHistogram() -> GetXaxis() -> SetTickLength(0.01);    
    hs -> GetHistogram() -> GetXaxis() -> SetLabelOffset(999);
    hs -> Draw("axis hist hbar");

    for(unsigned int yield = 0; yield < yields.size(); yield++)
    {
	pad1 -> cd();
	TLatex* Tl = new TLatex(0.05, (float)yield, Form("%.2f expected events", yields[yield]));
	Tl -> SetTextSize(0.025);
	Tl -> SetTextColor(kWhite);
	Tl -> SetTextAlign(12);
	Tl -> Draw();
    }

    double x_align = (pad1 -> GetUxmax() - pad1 -> GetUxmin()) * (-0.2) + pad1 -> GetUxmin();

    for(unsigned int ann = 0; ann < cat_labels.size(); ann++)
    {
	pad1 -> cd();
	TLatex* Tl = new TLatex();
	Tl -> SetTextSize(0.025);
	Tl -> SetTextColor(kBlack);
	Tl -> SetTextAlign(22);
	Tl -> DrawLatex(x_align, (float)ann, cat_labels[ann]);
    }

    float upper_bound = (pad1 -> GetUymax() - pad1 -> GetUymin()) * 1.02 + pad1 -> GetUymin();

    // put the luminosity
    pad1 -> cd();
    TLatex* Tl = new TLatex();
    Tl -> SetTextSize(0.025);
    Tl -> SetTextColor(kBlack);
    Tl -> SetTextAlign(31);
    Tl -> DrawLatex(pad1 -> GetUxmax(), upper_bound, Form("%.2f fb^{-1} (13TeV)", lumi));			    
    if(draw_label)
    {
	TLatex* Tl = new TLatex(pad1 -> GetUxmin(), upper_bound, label);
	Tl -> SetTextSize(0.025);
	Tl -> SetTextColor(kBlack);
	Tl -> SetTextAlign(11);
	Tl -> Draw();			    
    }

    if(draw_legend)
    {
	pad2 -> cd();
	leg -> SetTextColor(kBlack);
	leg -> SetTextSize(0.11);
	leg -> SetBorderSize(0);
	leg -> Draw();
    }

    TLine l;
    l.SetLineStyle(3);
    for(auto cur : line_coords)
    {
	l.DrawLine(cur, pad1 -> GetUymin(), cur, pad1 -> GetUymax());
    }

    canv -> cd();
    pad1 -> Draw();
    pad2 -> Draw(); 
    gPad -> RedrawAxis();
    canv -> Update();
}

void CatPlotter::SaveAs(TString file)
{
    canv -> SaveAs(file);
    //delete canv;
}

THStack* CatPlotter::GetStack()
{
    return this -> hs;
}

void CatPlotter::Update()
{
    Redraw();
}
