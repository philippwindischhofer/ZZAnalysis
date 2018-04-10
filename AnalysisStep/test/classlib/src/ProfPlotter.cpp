#include <ZZAnalysis/AnalysisStep/test/classlib/include/ProfPlotter.h>

ProfPlotter::ProfPlotter()
{  }

ProfPlotter::~ProfPlotter()
{  }

void ProfPlotter::Construct(std::map<TString, TH1F*> hist_vec, Config& conf, TString label_x, TString label_y, TString label_l, TString label_r, TString args)
{
    std::vector<TH1F*> hist_only_vec;
    std::vector<TString> source_labels;
    
    for(auto& hist: hist_vec)
    {	
	TString hist_name = hist.first;
	
	hist_only_vec.push_back(hist.second);
	source_labels.push_back(conf.source_label(hist_name));
    }

    Construct(hist_only_vec, source_labels, label_x, label_y, label_l, label_r, args);
}

void ProfPlotter::Construct(std::vector<TH1F*> hist_vec, std::vector<TString> source_labels, TString label_x, TString label_y, TString label_l, TString label_r, TString args)
{
    this -> label_l = label_l;
    this -> label_r = label_r;
    this -> label_x = label_x;
    this -> label_y = label_y;

    // Do the plotting
    canv = new TCanvas("canv", "canv", 10, 10, 800, 600);
    hs = new THStack();

    //gStyle -> SetHistTopMargin(0.);

    pad1 = new TPad("pad1", "pad1", 0.0, 0.0, 0.8, 1.0, kWhite, 0, 0);
    pad2 = new TPad("pad2", "pad2", 0.80, 0.0, 1.0, 1.0, kWhite, 0, 0);

    pad1 -> SetLeftMargin(0.15);
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
	if(source_labels[i] != NULL)
	{
	    if(hist_vec[i] -> GetMarkerStyle() > 8)
		leg -> AddEntry(hist_vec[i], source_labels[i], "p");
	    else
		leg -> AddEntry(hist_vec[i], source_labels[i], "f");
	}
    }
    
    for(auto& hist : hist_vec)
    {
	hs -> Add(hist);
    }

    std::cout << "redrawing" << std::endl;

    Redraw(args);
}

TPad* ProfPlotter::GetCanvas()
{
    return pad1;
}

void ProfPlotter::Redraw(TString args)
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
    hs -> Draw("hist " + args);
    hs -> GetXaxis() -> SetTitle(label_x);
    hs -> GetYaxis() -> SetTitle(label_y);
    hs -> Draw("hist " + args);

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
