#include <ZZAnalysis/AnalysisStep/test/Profiler/include/ProfPlotter.h>

ProfPlotter::ProfPlotter()
{  }

ProfPlotter::~ProfPlotter()
{  }

void ProfPlotter::Construct(std::vector<TH1F*> hist_vec, std::vector<TString> source_labels, TString label_x, TString label_y, TString label_l, TString label_r)
{
    this -> label_l = label_l;
    this -> label_r = label_r;
    this -> label_x = label_x;
    this -> label_y = label_y;

    // Do the plotting
    canv = new TCanvas("canv", "canv", 10, 10, 800, 600);
    hs = new THStack();

    gStyle -> SetHistTopMargin(0.);

    pad1 = new TPad("pad1", "pad1", 0.0, 0.0, 0.8, 1.0, kWhite, 0, 0);
    pad2 = new TPad("pad2", "pad2", 0.80, 0.0, 1.0, 1.0, kWhite, 0, 0);

    pad1 -> SetLeftMargin(0.13);
    pad1 -> SetTicks(1, 1);
    pad1 -> SetRightMargin(0.03);
    pad1 -> SetFillColorAlpha(kWhite, 0.0);
    pad2 -> SetLeftMargin(0.0);
    pad2 -> SetFillColorAlpha(kWhite, 0.0);

    leg = new TLegend(0.0, 0.9 - 0.4 / 10 * (hist_vec.size()), 0.5, 0.9);
    leg -> SetTextColor(kBlack);
    leg -> SetTextSize(0.11);
    leg -> SetBorderSize(0);

    if(source_labels.size() > 0)
	draw_legend = true;

    for(unsigned int i = 0; i < source_labels.size(); i++)
    {
	leg -> AddEntry(hist_vec[i], source_labels[i], "f");
    }
    
    for(auto& hist : hist_vec)
    {
	hs -> Add(hist);
    }

    std::cout << "redrawing" << std::endl;

    Redraw();
}

void ProfPlotter::Redraw()
{
    canv -> cd();
    pad1 -> Draw();
    pad2 -> Draw();

    if(draw_legend)
    {
	pad2 -> cd();
	leg -> Draw();
    }

    pad1 -> cd();
    hs -> Draw("hist");
    hs -> GetXaxis() -> SetTitle(label_x);
    hs -> GetYaxis() -> SetTitle(label_y);
    hs -> Draw("hist");

    // draw left & right labels
    TLatex* Tl = new TLatex();
    Tl -> SetTextSize(0.025);
    Tl -> SetTextColor(kBlack);
    Tl -> SetTextAlign(31);
    
    float upper_bound = (pad1 -> GetUymax() - pad1 -> GetUymin()) * 1.02 + pad1 -> GetUymin();

    Tl -> DrawLatex(pad1 -> GetUxmax(), upper_bound, label_r);

    Tl -> SetTextAlign(11);
    Tl -> DrawLatex(pad1 -> GetUxmin(), upper_bound, label_l);
}

void ProfPlotter::SaveAs(TString file)
{
    canv -> SaveAs(file);
    delete canv;
}
