#include <ZZAnalysis/AnalysisStep/test/ConvClassifier/include/CatPlotter.h>

void CatPlotter::Construct(std::vector<TH1F*> hists, std::vector<TString> cat_labels, std::vector<TString> source_labels, std::vector<float> yields, float lumi, TString file)
{
    THStack* hs = new THStack("hs","");

    for(unsigned int i = 0; i < hists.size(); i++)
    {
	hs -> Add(hists[i]);
    }

    TCanvas* canv = new TCanvas("canv", "canv", 10, 10, 800, 600);
    TPad* pad1 = new TPad("pad1", "pad1", 0.0, 0.0, 0.80, 1.0, kWhite, 0, 0);
    TPad* pad2 = new TPad("pad2", "pad2", 0.80, 0.0, 1.0, 1.0, kWhite, 0, 0);
    pad1 -> SetLeftMargin(0.3);
    pad1 -> SetTicks(1, 1);
    pad1 -> SetRightMargin(0.03);
    pad2 -> SetLeftMargin(0.0);
    pad1 -> Draw();
    pad2 -> Draw(); 

    pad1 -> cd();
    hs -> Draw("hist hbar");
    gStyle->SetHistTopMargin(0.);

    for(unsigned int i = 0; i < hists.size(); i++)
    {
	hists[i] -> SetBarOffset(0.15);
	hists[i] -> SetBarWidth(0.7);
    }

    hs -> GetYaxis() -> SetTitle("signal fraction");
    hs -> GetYaxis() -> SetTitleOffset(0.95);
    hs -> GetHistogram() -> GetXaxis() -> SetNdivisions(7, 0, 0, kFALSE);
    hs -> GetHistogram() -> GetYaxis() -> SetNdivisions(10, 2, 0, kFALSE);
    hs -> GetHistogram() -> GetYaxis() -> SetTickLength(0.02);
    hs -> GetHistogram() -> GetXaxis() -> SetLabelOffset(999);
    hs -> Draw("hist hbar");

    TLegend* leg = new TLegend(0.0, 0.5, 1.0, 0.9);

    for(unsigned int i = 0; i < hists.size(); i++)
    {
	leg -> AddEntry(hists[i], source_labels[i], "f");
    }
   
    for(unsigned int ann = 0; ann < cat_labels.size(); ann++)
    {
	pad1 -> cd();
	TLatex* Tl = new TLatex(0.05, (float)ann, Form("%.3f expected events", yields[ann]));
	Tl -> SetTextSize(0.025);
	Tl -> SetTextColor(kWhite);
	Tl -> SetTextAlign(12);
	Tl -> Draw();

	pad1 -> cd();
	Tl = new TLatex(-0.2, (float)ann, cat_labels[ann]);
	Tl -> SetTextSize(0.025);
	Tl -> SetTextColor(kBlack);
	Tl -> SetTextAlign(22);
	Tl -> Draw();
    }

    // put the luminosity
    pad1 -> cd();
    TLatex* Tl = new TLatex(1.0, cat_labels.size() - 0.4, Form("%.2f fb^{-1} (13TeV)", lumi));
    Tl -> SetTextSize(0.025);
    Tl -> SetTextColor(kBlack);
    Tl -> SetTextAlign(31);
    Tl -> Draw();			    

    pad2 -> cd();
    leg -> SetTextColor(kBlack);
    leg -> SetTextSize(0.11);
    leg -> SetBorderSize(0);
    leg -> Draw();

    canv -> cd();
    pad1 -> Draw();
    pad2 -> Draw(); 
    gPad -> RedrawAxis();
    canv -> Update();

    canv -> SaveAs(file);
    delete canv;
}
