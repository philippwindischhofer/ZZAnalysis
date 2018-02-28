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
#include <ZZAnalysis/AnalysisStep/test/ConvClassifier/include/Mor17Classifier.h>
#include <ZZAnalysis/AnalysisStep/interface/Category.h>
#include <ZZAnalysis/AnalysisStep/test/Plotter_v2/src/setTDRStyle.cpp>
#include <ZZAnalysis/AnalysisStep/test/ConvClassifier/include/CatPlotter.h>
#include <ZZAnalysis/AnalysisStep/test/ConvClassifier/include/Tree.h>

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

    for(unsigned int hist = 0; hist < hist_names.size(); hist++)
    {
	TH1F* cur_hist = (TH1F*)(infile -> Get(hist_names[hist]));
	hist_vec.push_back(cur_hist);
    }

    return hist_vec;
}

int no_cut(Tree* in)
{
    return kTRUE;
}

int mZZ_cut(Tree* in)
{
    if((in -> ZZMass > 118) && (in -> ZZMass < 130))
	return kTRUE;
    else
	return kFALSE;
}

int main( int argc, char *argv[] )
{
    float lumi = 35.9f;
    
    TString path = "/data_CMS/cms/tsculac/CJLST_NTuples/";
    TString file_name = "/ZZ4lAnalysis.root";
    TString out_folder("../../src/ZZAnalysis/ConvClassifierPlots/");

    std::vector<TString> cat_labels = {"Untagged", "#splitline{VBF-1jet}{ tagged}", "#splitline{VBF-2jet}{ tagged}", "#splitline{VH-leptonic}{    tagged}", "#splitline{VH-hadronic}{     tagged}", "ttH tagged", "#splitline{VH-E_{T}^{miss}}{ tagged}"};
    
    std::vector<TString> data_file_names = {"ggH125", "VBFH125", "WplusH125", "WminusH125", "ZH125", "ttH125", "bbH125", "tqH125"};

    std::vector<TString> data_path;
    for(unsigned int i = 0; i < data_file_names.size(); i++)
    {
	data_path.push_back(path + data_file_names[i] + file_name);
    }

    std::vector<TString> hist_names = {"ggHhist", "VBFhist", "WHhist", "ZHhist", "ttHhist"};
    std::vector<TString> source_labels = {"ggH", "VBF", "WH", "ZH", "ttH"};

    enum hist_index {
	ggHhist = 0,
	VBFhist = 1,
	WHhist = 2,
	ZHhist = 3,
	ttHhist = 4
    };
	
    std::vector<TH1F*> hist_vec(hist_names.size());
	
	for(unsigned int i = 0; i < hist_names.size(); i++)
    {
	hist_vec[i] = new TH1F(hist_names[i], hist_names[i], 7, -0.5, 6.5);
    }

    std::vector<int> cat_colors = {kBlue - 9, kGreen - 6, kRed - 7, kOrange + 6, kCyan - 6};

    for(unsigned int i = 0; i < hist_names.size(); i++)
    {
	hist_vec[i] -> SetFillColor(cat_colors[i]);
	hist_vec[i] -> SetLineColor(cat_colors[i]);
	hist_vec[i] -> SetFillStyle(1001);
    }    

    // No need to fill the histograms every time!
    std::cout << "filling histograms" << std::endl;
    Mor17Classifier* testclass = new Mor17Classifier();
    testclass -> FillHistogram(data_path[0], lumi, hist_vec[ggHhist], no_cut);
    testclass -> FillHistogram(data_path[1], lumi, hist_vec[VBFhist], no_cut);
    testclass -> FillHistogram(data_path[2], lumi, hist_vec[WHhist], no_cut);
    testclass -> FillHistogram(data_path[3], lumi, hist_vec[WHhist], no_cut);
    testclass -> FillHistogram(data_path[4], lumi, hist_vec[ZHhist], no_cut);
    testclass -> FillHistogram(data_path[5], lumi, hist_vec[ttHhist], no_cut);    
    save_histos(out_folder + "histos.root", hist_vec);
    std::cout << "end filling histograms" << std::endl;

    hist_vec = read_histos(out_folder + "histos.root", hist_names);
    
    std::vector<float> sums = renormalize_histograms(hist_vec);

    CatPlotter::Construct(hist_vec, cat_labels, source_labels, sums, out_folder + "categorization.pdf");

    return(0);
}
