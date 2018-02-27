// C++
#include <iostream>
#include <fstream>
#include <string>

// ROOT
#include "TApplication.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TStyle.h"
#include "THStack.h"
#include "TLatex.h"
#include "TLegend.h"

// My own files
#include <ZZAnalysis/AnalysisStep/test/ConvClassifier/include/ConvClassifier.h>
#include <ZZAnalysis/AnalysisStep/interface/Category.h>
#include <ZZAnalysis/AnalysisStep/test/Plotter_v2/src/setTDRStyle.cpp>

enum SignalCategory {
    ggH = 0,
    VBF = 1
};

std::vector<float> renormalize_histograms(std::vector<TH1F*> hist_vec)
{
    std::vector<float> sumvec(7);

    for(unsigned int bin = 0; bin < sumvec.size(); bin++)
    {
	sumvec[bin] = 0;

	for(unsigned int hist = 0; hist < hist_vec.size(); hist++)
	{
	    sumvec[bin] += (hist_vec[hist] -> GetBinContent(bin + 1));
	}
    }

    for(unsigned int bin = 0; bin < sumvec.size(); bin++)
    {
	for(unsigned int hist = 0; hist < hist_vec.size(); hist++)
	{
	    float cur_bin = hist_vec[hist] -> GetBinContent(bin + 1);
	    hist_vec[hist] -> SetBinContent(bin + 1, cur_bin / sumvec[bin]);
	}
    }

    return sumvec;
}

void save_histos(TString file, std::vector<TH1F*>& hist_vec)
{
    TFile* outfile = new TFile(file, "recreate");
    outfile -> cd();

    for(unsigned int hist = 0; hist < hist_vec.size(); hist++)
    {
	hist_vec[hist] -> Write();
    }

    outfile -> Close();
    delete outfile;
}

std::vector<TH1F*> read_histos(TString file, std::vector<TString> hist_names)
{
    std::vector<TH1F*> hist_vec;
    TFile* infile = TFile::Open(file);

    // problem is somewhere here!
    for(unsigned int hist = 0; hist < hist_vec.size(); hist++)
    {
	hist_vec.push_back((TH1F*)(infile -> Get(hist_names[hist])));
    }

    return hist_vec;
}

int main( int argc, char *argv[] )
{
    //setTDRStyle();

    TString path = "/data_CMS/cms/tsculac/CJLST_NTuples/";
    TString file_name = "/ZZ4lAnalysis.root";

    std::vector<TString> annotations = {"Untagged", "VBF-1jet tagged", "VBF-2jet tagged", "VH-leptonic tagged", "VH-hadronic tagged", "ttH tagged", "VH-E_{T}^{miss} tagged"};
    
    TString ggH125      = path + "ggH125"     + file_name;
    TString VBFH125     = path + "VBFH125"    + file_name;
    TString WpH125      = path + "WplusH125"  + file_name;
    TString WmH125      = path + "WminusH125" + file_name;
    TString ZH125       = path + "ZH125"      + file_name;
    TString ttH125      = path + "ttH125"     + file_name;
    TString bbH125      = path + "bbH125"     + file_name;
    TString tqH125      = path + "tqH125"     + file_name;

    float lumi = 35.9f;

    float xsec_ggH = 48.58 * 0.0002745f;
    float xsec_VBF = 3.782 * 0.0002745f;
    float xsec_WpH = 0.839913391993366 * 0.0002745f;
    float xsec_WmH = 0.532731376975169 * 0.0002745f;
    float xsec_ZH = 0.883888217433748 * 0.0007494205421886052f;
    float xsec_ttH = 0.5071 * 0.0007694542606325344f;

    std::vector<TString> hist_names = {"ggHhist", "VBFhist", "WHhist", "ZHhist", "ttHhist"};
    std::vector<TString> channel_names = {"ggH", "VBF", "WH", "ZH", "ttH"};

    TH1F* ggHhist = new TH1F(hist_names[0], hist_names[0], 7, -0.5, 6.5);
    TH1F* VBFhist = new TH1F(hist_names[1], hist_names[1], 7, -0.5, 6.5);
    TH1F* WHhist = new TH1F(hist_names[2], hist_names[2], 7, -0.5, 6.5);
    TH1F* ZHhist = new TH1F(hist_names[3], hist_names[3], 7, -0.5, 6.5);
    TH1F* ttHhist = new TH1F(hist_names[4], hist_names[4], 7, -0.5, 6.5);

    std::vector<TH1F*> hist_vec = {ggHhist, VBFhist, WHhist, ZHhist, ttHhist};

    ggHhist -> SetFillColor(kBlue);
    VBFhist -> SetFillColor(kGreen);
    WHhist -> SetFillColor(kRed);
    ZHhist -> SetFillColor(kOrange);
    ttHhist -> SetFillColor(kPink);

    ggHhist -> SetLineColor(kBlue);
    VBFhist -> SetLineColor(kGreen);
    WHhist -> SetLineColor(kRed);
    ZHhist -> SetLineColor(kOrange);
    ttHhist -> SetLineColor(kPink);

    ggHhist -> SetFillStyle(1001);
    VBFhist -> SetFillStyle(1001);
    WHhist -> SetFillStyle(1001);
    ZHhist -> SetFillStyle(1001);
    ttHhist -> SetFillStyle(1001);

    std::cout << "filling histograms" << std::endl;
    ConvClassifier* testclass = new ConvClassifier();
    testclass -> FillHistogram(ggH125, lumi, xsec_ggH, ggHhist);
    testclass -> FillHistogram(VBFH125, lumi, xsec_VBF, VBFhist);
    testclass -> FillHistogram(WpH125, lumi, xsec_WpH, WHhist);
    testclass -> FillHistogram(WmH125, lumi, xsec_WmH, WHhist);
    testclass -> FillHistogram(ZH125, lumi, xsec_ZH, ZHhist);
    testclass -> FillHistogram(ttH125, lumi, xsec_ttH, ttHhist);    
    //save_histos("histos.root", hist_vec);
    std::cout << "end filling histograms" << std::endl;

    hist_vec = read_histos("histos.root", hist_names);
    std::cout << "read" << std::endl;

    std::vector<float> sums = renormalize_histograms(hist_vec);
    std::cout << "renormalized" << std::endl;
    
    THStack* hs = new THStack("hs","");

    for(unsigned int i = 0; i < hist_vec.size(); i++)
    {
	hs -> Add(hist_vec[i]);
    }
    std::cout << "added to stack" << std::endl;
    // hs -> Add(ggHhist);
    // hs -> Add(VBFhist);
    // hs -> Add(WHhist);
    // hs -> Add(ZHhist);
    // hs -> Add(ttHhist);

    TCanvas* canv = new TCanvas("canv", "canv", 10, 10, 600, 600);
    std::cout << "adding to legend" << std::endl;
    canv -> cd();
    std::cout << "adding to legend" << std::endl;
    hs -> Draw("hist");
    //hs -> GetYaxis() -> SetTitle("signal fraction");
    std::cout << "adding to legend" << std::endl;

    //hs -> GetHistogram() -> GetXaxis() -> SetTickLength(0);
    //hs -> GetHistogram() -> GetXaxis() -> SetLabelOffset(999);

    TLegend* leg = new TLegend(1.0,0.72,1.2,0.92);

    std::cout << "adding to legend" << std::endl;

    for(unsigned int i = 0; i < hist_vec.size(); i++)
    {
	leg -> AddEntry(hist_vec[i], channel_names[i], "f");
    }

    std::cout << "added to legend" << std::endl;

    // leg -> AddEntry(ggHhist, "ggH", "f");
    // leg -> AddEntry(VBFhist, "VBF", "f");
    // leg -> AddEntry(WHhist, "WH", "f");
    // leg -> AddEntry(ZHhist, "ZH", "f");
    // leg -> AddEntry(ttHhist, "ttH", "f");
   
    for(unsigned int ann = 0; ann < annotations.size(); ann++)
    {
	TLatex* Tl = new TLatex((float)ann, 0.05, annotations[ann] + " (" + Form("%.3f", sums[ann]) + " expected events)");
	Tl -> SetTextAngle(90);
	Tl -> SetTextSize(0.02);
	Tl -> SetTextColor(kWhite);
	Tl -> SetTextAlign(12);
	Tl -> Draw();
    }
    
    leg -> Draw();
    canv -> Update();

    TString folder("../../src/ZZAnalysis/ConvClassifierPlots/");
    system("mkdir -p " + folder);
    canv -> SaveAs(folder + "ggh_hist.pdf");
    delete canv;

    return(0);
}
