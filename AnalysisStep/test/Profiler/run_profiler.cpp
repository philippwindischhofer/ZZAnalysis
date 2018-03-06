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

// own local files
#include <ZZAnalysis/AnalysisStep/test/Profiler/include/Profiler.h>
#include <ZZAnalysis/AnalysisStep/test/Profiler/include/ProfPlotter.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/Classifier.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/Mor17Classifier.h>
#include <ZZAnalysis/AnalysisStep/interface/Category.h>

// global file locations
float lumi = 35.9f;

TString path = "/data_CMS/cms/tsculac/CJLST_NTuples/";
TString file_name = "/ZZ4lAnalysis.root";
TString out_folder = "../../src/ZZAnalysis/ProfilerPlots/";

std::vector<TString> file_names = {
    "ggH125",
    "VBFH125",
    "WplusH125",
    "WminusH125",
    "ZH125",
    "ttH125",
    "ZZTo4l", 
    "ggTo2e2mu_Contin_MCFM701",
    "ggTo2e2tau_Contin_MCFM701",
    "ggTo2mu2tau_Contin_MCFM701",
    "ggTo4e_Contin_MCFM701",
    "ggTo4mu_Contin_MCFM701",
    "ggTo4tau_Contin_MCFM701"
};

std::vector<TString> hist_names = {
    "ggHhist",
    "VBFhist",
    "WHhist",
    "ZHhist",
    "ttHhist",
    "qqTo4lhist",
    "ggTo4lhist"
};

std::vector<TString> source_labels = {
    "ggH",
    "VBF",
    "WH",
    "ZH",
    "ttH",
    "q#bar{q} #rightarrow ZZ #rightarrow 4l",
    "gg #rightarrow ZZ #rightarrow 4l"
};

std::vector<int> source_styles = {
    1001,
    1001,
    1001,
    1001,
    1001,
    3244,
    3244
};

enum hist_index {
    ggHhist = 0,
    VBFhist = 1,
    WHhist = 2,
    ZHhist = 3,
    ttHhist = 4,
    qqhist = 5,
    gghist = 6
};

std::vector<int> source_colors = {
    kBlue - 9,
    kGreen - 6,
    kRed - 7,
    kYellow - 7,
    kCyan - 6,
    kBlue - 9,
    kGreen - 6
};

bool no_cut(Tree* in)
{
    return kTRUE;
}

bool mZZ_cut(Tree* in)
{
    if((in -> ZZMass > 118.) && (in -> ZZMass < 130.))
	return kTRUE;
    else
	return kFALSE;
}

bool final_state_4e_cut(Tree* in)
{
    if(((in -> Z1Flav) == -121) && ((in -> Z2Flav) == -121))
	return kTRUE;
    else
	return kFALSE;
}

bool final_state_2e2mu_cut(Tree* in)
{
    if( (((in -> Z1Flav) == -121) && ((in -> Z2Flav) == -169)) ||
	(((in -> Z1Flav) == -169) && ((in -> Z2Flav) == -121)) )
	return kTRUE;
    else
	return kFALSE;
}

bool final_state_4mu_cut(Tree* in)
{
    if(((in -> Z1Flav) == -169) && ((in -> Z2Flav) == -169))
	return kTRUE;
    else
	return kFALSE;
}

void make_plots(Classifier* testclass, float lumi, std::function<float(Tree*)> var, std::function<bool(Tree*)> ext_cut, int category, TString x_label, TString label_left, TString out_file)
{
    Profiler* prof = new Profiler();
    ProfPlotter* plotter = new ProfPlotter();

    TString label_right = Form("%.2f fb^{-1} (13TeV)", lumi);

    std::vector<TH1F*> hist_vec(hist_names.size());
    std::vector<TString> path_vec(file_names.size());

    for(unsigned int i = 0; i < hist_names.size(); i++)
    {
	hist_vec[i] = new TH1F(file_names[i], file_names[i], 100, 0, 100);
	hist_vec[i] -> SetFillColor(source_colors[i]);
	hist_vec[i] -> SetLineColor(source_colors[i]);
	hist_vec[i] -> SetFillStyle(source_styles[i]);
    }
    
    for(unsigned int i = 0; i < file_names.size(); i++)
    {
	path_vec[i] = path + file_names[i] + file_name;
    }

    auto cut = [&](Tree* in) -> bool {
	return (testclass -> ClassifyThisEvent(in) == category &&
		ext_cut(in) &&
		mZZ_cut(in)) ? 
	kTRUE : kFALSE;};

    prof -> FillProfile(path_vec[0], lumi, hist_vec[ggHhist], cut, var);
    prof -> FillProfile(path_vec[1], lumi, hist_vec[VBFhist], cut, var);
    prof -> FillProfile(path_vec[2], lumi, hist_vec[WHhist], cut, var);
    prof -> FillProfile(path_vec[3], lumi, hist_vec[WHhist], cut, var);
    prof -> FillProfile(path_vec[4], lumi, hist_vec[ZHhist], cut, var);
    prof -> FillProfile(path_vec[5], lumi, hist_vec[ttHhist], cut, var);
    prof -> FillProfile(path_vec[6], lumi, hist_vec[qqhist], cut, var);
    prof -> FillProfile(path_vec[7], lumi, hist_vec[gghist], cut, var);
    prof -> FillProfile(path_vec[8], lumi, hist_vec[gghist], cut, var);
    prof -> FillProfile(path_vec[9], lumi, hist_vec[gghist], cut, var);
    prof -> FillProfile(path_vec[10], lumi, hist_vec[gghist], cut, var);
    prof -> FillProfile(path_vec[11], lumi, hist_vec[gghist], cut, var);
    prof -> FillProfile(path_vec[12], lumi, hist_vec[gghist], cut, var);

    plotter -> Construct(hist_vec, source_labels, x_label, "Events", label_left, label_right);
    plotter -> SaveAs(out_folder + out_file + ".pdf");
}


int main(int argc, char *argv[])
{
    Mor17Classifier* testclass = new Mor17Classifier();
    auto testvar = [&](Tree* in) -> float{return in -> Z1Mass;};

    make_plots(testclass, lumi, testvar, final_state_4mu_cut, ttHTaggedMor17, "m(Z1)", "ZZ #rightarrow 4#mu", "testplot");

    return(0);
}
