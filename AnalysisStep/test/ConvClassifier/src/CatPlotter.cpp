#include <ZZAnalysis/AnalysisStep/test/ConvClassifier/include/CatPlotter.h>

CatPlotter::CatPlotter()
{  }

CatPlotter::~CatPlotter()
{  }

void CatPlotter::Construct(std::vector<TH1F*> hists, std::vector<TString> cat_labels, std::vector<TString> source_labels, std::vector<float> yields, TString title, float lumi)
{
    this -> cat_labels = cat_labels;
    this -> source_labels = source_labels;
    this -> yields = yields;
    this -> lumi = lumi;
    this -> title = title;

    this -> hs = new THStack("hs","");

    for(unsigned int i = 0; i < hists.size(); i++)
    {
	hists[i] -> SetBarOffset(0.15);
	hists[i] -> SetBarWidth(0.7);
    }

    for(unsigned int i = 0; i < hists.size(); i++)
    {
	hs -> Add(hists[i]);
    }

    canv = new TCanvas("canv", "canv", 10, 10, 800, 600);
    pad1 = new TPad("pad1", "pad1", 0.0, 0.0, 0.80, 1.0, kWhite, 0, 0);
    pad2 = new TPad("pad2", "pad2", 0.80, 0.0, 1.0, 1.0, kWhite, 0, 0);
    pad1 -> SetLeftMargin(0.3);
    pad1 -> SetTicks(1, 1);
    pad1 -> SetRightMargin(0.03);
    pad1 -> SetFillColorAlpha(kWhite, 0.0);
    pad2 -> SetLeftMargin(0.0);
    pad2 -> SetFillColorAlpha(kWhite, 0.0);

    draw_legend = (source_labels.size() > 0) ? true : false;

    leg = new TLegend(0.0, 0.9 - 0.4 / 9 * (hists.size()), 1.0, 0.9);
    for(unsigned int i = 0; i < source_labels.size(); i++)
    {
	leg -> AddEntry(hists[i], source_labels[i], "f");
    } 

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
    hs -> GetHistogram() -> GetXaxis() -> SetNdivisions(7, 0, 0, kFALSE);
    hs -> GetHistogram() -> GetYaxis() -> SetNdivisions(10, 2, 0, kTRUE);
    hs -> GetHistogram() -> GetYaxis() -> SetTickLength(0.02);
    hs -> GetHistogram() -> GetXaxis() -> SetLabelOffset(999);
    hs -> Draw("hist hbar");

    for(unsigned int yield = 0; yield < yields.size(); yield++)
    {
	pad1 -> cd();
	TLatex* Tl = new TLatex(0.05, (float)yield, Form("%.3f expected events", yields[yield]));
	Tl -> SetTextSize(0.025);
	Tl -> SetTextColor(kWhite);
	Tl -> SetTextAlign(12);
	Tl -> Draw();
    }

    for(unsigned int ann = 0; ann < cat_labels.size(); ann++)
    {
	pad1 -> cd();
	TLatex* Tl = new TLatex((-0.2) * hs -> GetMaximum(), (float)ann, cat_labels[ann]);
	Tl -> SetTextSize(0.025);
	Tl -> SetTextColor(kBlack);
	Tl -> SetTextAlign(22);
	Tl -> Draw();
    }

    // put the luminosity
    pad1 -> cd();
    TLatex* Tl = new TLatex(hs -> GetMaximum(), cat_labels.size() - 0.4, Form("%.2f fb^{-1} (13TeV)", lumi));
    Tl -> SetTextSize(0.025);
    Tl -> SetTextColor(kBlack);
    Tl -> SetTextAlign(31);
    Tl -> Draw();			    

    if(draw_legend)
    {
	pad2 -> cd();
	leg -> SetTextColor(kBlack);
	leg -> SetTextSize(0.11);
	leg -> SetBorderSize(0);
	leg -> Draw();
    }

    canv -> cd();
    pad1 -> Draw();
    pad2 -> Draw(); 
    gPad -> RedrawAxis();
    canv -> Update();
}

void CatPlotter::SaveAs(TString file)
{
    this -> canv -> SaveAs(file);
    delete this -> canv;
}

THStack* CatPlotter::GetStack()
{
    return this -> hs;
}

void CatPlotter::Update()
{
    Redraw();
}
