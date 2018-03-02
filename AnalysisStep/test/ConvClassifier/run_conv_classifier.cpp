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
#include <ZZAnalysis/AnalysisStep/test/ConvClassifier/include/utils.h>
#include <ZZAnalysis/AnalysisStep/test/ConvClassifier/include/Tree.h>

// global definitions
float lumi = 35.9f;

TString path = "/data_CMS/cms/tsculac/CJLST_NTuples/";
TString file_name = "/ZZ4lAnalysis.root";
TString out_folder("../../src/ZZAnalysis/ConvClassifierPlots/");

std::vector<TString> cat_labels = {"Untagged", "#splitline{VBF-1jet}{ tagged}", "#splitline{VBF-2jet}{ tagged}", "#splitline{VH-leptonic}{    tagged}", "#splitline{VH-hadronic}{     tagged}", "ttH tagged", "#splitline{VH-E_{T}^{miss}}{ tagged}"};
    
std::vector<TString> signal_file_names = {"ggH125", "VBFH125", "WplusH125", "WminusH125", "ZH125", "ttH125", "bbH125", "tqH125"};

std::vector<TString> signal_source_labels = {
    "ggH", 
    "VBF", 
    "WH, W #rightarrow X",
    "WH, W #rightarrow l#nu", 
    "ZH, Z #rightarrow X",
    "ZH, Z #rightarrow 2l",
    "t#bar{t}H, t#bar{t} #rightarrow 0l + X",
    "t#bar{t}H, t#bar{t} #rightarrow 1l + X",
    "t#bar{t}H, t#bar{t} #rightarrow 2l + X"};

// names of the signal histograms
std::vector<TString> signal_hist_names = {
    "ggHhist", 
    "VBFhist",
    "WHXhist", 
    "WHlnuhist", 
    "ZHXhist", 
    "ZH2lhist", 
    "ttH0lhist", 
    "ttH1lhist", 
    "ttH2lhist"};

enum hist_index {
    ggHhist = 0,
    VBFhist = 1,
    WHXhist = 2,
    WHlnuhist = 3,
    ZHXhist = 4,
    ZH2lhist = 5,
    ttH0lhist = 6,
    ttH1lhist = 7,
    ttH2lhist = 8
};

// bin 0 is actually the underflow bin -> relevant content starts at bin 1 only
enum bin_index {
    untagged_bin = 1,
    VBF1jet_bin = 2,
    VBF2jet_bin = 3,
    VHleptonic_bin = 4,
    VHhadronic_bin = 5,
    ttH_bin = 6,
    VHMET_bin = 7
};

std::vector<int> signal_source_colors = {
    kBlue - 9, 
    kGreen - 6, 
    kRed - 7, 
    kRed - 6, 
    kOrange + 6, 
    kOrange + 7, 
    kCyan - 6,
    kCyan - 2,
    kCyan + 3
};

std::vector<int> background_source_colors = {
    kBlue - 9,
    kGreen - 6,
    kRed - 9,
    kRed - 6,
    kRed - 2,
    kRed + 3,
    kCyan - 6,
    kCyan - 2,
    kCyan + 3    
};

std::vector<TString> background_file_names = {
    "ZZTo4l", 
    "DYJetsToLL_M50",
    "TTTo2L2Nu",
    "ggTo2e2mu_Contin_MCFM701",
    "ggTo2e2tau_Contin_MCFM701",
    "ggTo2mu2tau_Contin_MCFM701",
    "ggTo4e_Contin_MCFM701",
    "ggTo4mu_Contin_MCFM701",
    "ggTo4tau_Contin_MCFM701"
};

std::vector<TString> background_hist_names = {
    "ZZTo4l", 
    "DYToLL",
    "TTTo2L2Nu",
    "ggTo2e2mu",
    "ggTo2e2tau",
    "ggTo2mu2tau",
    "ggTo4e",
    "ggTo4mu",
    "ggTo4tau"
};

std::vector<TString> background_source_labels = {
    "ZZ #rightarrow 4l",
    "DY",
    "tt #rightarrow 2l2#nu",
    "gg #rightarrow 2e2#mu",
    "gg #rightarrow 2e2#tau",
    "gg #rightarrow 2#mu2#tau",
    "gg #rightarrow 4e",
    "gg #rightarrow 4#mu",
    "gg #rightarrow 4#tau"
};

// cuts that are used to separate the individual signal contributions
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

int extraLeptons_0_cut(Tree* in)
{
    if(mZZ_cut(in) && (in -> GenAssocLep1Id == 0) && (in -> GenAssocLep2Id == 0))
	return kTRUE;
    else
	return kFALSE;
}

int extraLeptons_1_cut(Tree* in)
{
    if(mZZ_cut(in) && (in -> GenAssocLep1Id != 0) && (in -> GenAssocLep2Id == 0))
	return kTRUE;
    else
	return kFALSE;
}

int extraLeptons_2_cut(Tree* in)
{
    if(mZZ_cut(in) && (in -> GenAssocLep1Id != 0) && (in -> GenAssocLep2Id != 0))
	return kTRUE;
    else
	return kFALSE;
}

// generates a vector of histograms corresponding to the signals
std::vector<TH1F*> generate_signal_histvec(int fill_histos)
{
    TString hist_storage("histograms_signal.root");
    std::vector<TString> signal_path;

    for(unsigned int i = 0; i < signal_file_names.size(); i++)
    {
	signal_path.push_back(path + signal_file_names[i] + file_name);
    }
	
    std::vector<TH1F*> hist_vec(signal_hist_names.size());

    for(unsigned int i = 0; i < signal_hist_names.size(); i++)
    {
	hist_vec[i] = new TH1F(signal_hist_names[i], signal_hist_names[i], 7, -0.5, 6.5);
	hist_vec[i] -> SetFillColor(signal_source_colors[i]);
	hist_vec[i] -> SetLineColor(signal_source_colors[i]);
	hist_vec[i] -> SetFillStyle(1001);
    }    

    if(fill_histos)
    {	
	std::cout << "filling histograms" << std::endl;
	Mor17Classifier* testclass = new Mor17Classifier();
	// for the signal files, can be very specific and select precisely the correct differential channel
	// ideally, create one separate histogram (by using generator-level cuts) for each signal category. Then, can compute the efficiency for this signal to end up in the matching signal category
	testclass -> FillHistogram(signal_path[0], lumi, hist_vec[ggHhist], mZZ_cut);
	testclass -> FillHistogram(signal_path[1], lumi, hist_vec[VBFhist], mZZ_cut);
	testclass -> FillHistogram(signal_path[2], lumi, hist_vec[WHXhist], extraLeptons_0_cut);
	testclass -> FillHistogram(signal_path[2], lumi, hist_vec[WHlnuhist], extraLeptons_1_cut); // W decays leptonically
	testclass -> FillHistogram(signal_path[3], lumi, hist_vec[WHXhist], extraLeptons_0_cut);
	testclass -> FillHistogram(signal_path[3], lumi, hist_vec[WHlnuhist], extraLeptons_1_cut); // W decays leptonically
	testclass -> FillHistogram(signal_path[4], lumi, hist_vec[ZHXhist], extraLeptons_0_cut); // no extra leptons (electrons, muons) for the non-leptonic decay of the Z
	testclass -> FillHistogram(signal_path[4], lumi, hist_vec[ZH2lhist], extraLeptons_2_cut); // need exactly two extra leptons to have the Z decaying leptonically
	testclass -> FillHistogram(signal_path[5], lumi, hist_vec[ttH0lhist], extraLeptons_0_cut);    
	testclass -> FillHistogram(signal_path[5], lumi, hist_vec[ttH1lhist], extraLeptons_1_cut);
	testclass -> FillHistogram(signal_path[5], lumi, hist_vec[ttH2lhist], extraLeptons_2_cut);    
	std::cout << "end filling histograms" << std::endl;

	save_histos(out_folder + hist_storage, hist_vec);
    }    

    // read the histograms back from the root file
    hist_vec = read_histos(out_folder + hist_storage, signal_hist_names);

    return hist_vec;
}

std::vector<TH1F*> generate_background_histvec(int fill_histos)
{
    TString hist_storage("histograms_background.root");
    std::vector<TString> background_path;

    for(unsigned int i = 0; i < background_file_names.size(); i++)
    {
	background_path.push_back(path + background_file_names[i] + file_name);
    }

    std::vector<TH1F*> hist_vec(background_path.size());

    for(unsigned int i = 0; i < background_hist_names.size(); i++)
    {
	hist_vec[i] = new TH1F(background_hist_names[i], background_hist_names[i], 7, -0.5, 6.5);
	hist_vec[i] -> SetFillStyle(3244);
	hist_vec[i] -> SetLineColor(background_source_colors[i]);
	hist_vec[i] -> SetFillColor(background_source_colors[i]);
    }

    if(fill_histos)
    {	
	// No need to fill the histograms every time!
	std::cout << "filling background histograms" << std::endl;
	Mor17Classifier* testclass = new Mor17Classifier();
	
	// for the background files, don't have any requirements for many categories nor for any additional cuts, they just get summed up anyways later (keep the file-induced categorization anyways)
	for(unsigned int i = 0; i < background_path.size(); i++)
	{
	    std::cout << "reading from " << background_path[i] << std::endl;
	    testclass -> FillHistogram(background_path[i], lumi, hist_vec[i], mZZ_cut);
	}

	std::cout << "end filling background histograms" << std::endl;

	save_histos(out_folder + hist_storage, hist_vec);	
    }    

    // read the histograms back from the root file
    hist_vec = read_histos(out_folder + hist_storage, background_hist_names);

    return hist_vec;
}

void make_S_purity(int fill_histos)
{
    std::vector<TH1F*> hist_vec;

    hist_vec = generate_signal_histvec(fill_histos);

    CatPlotter plotter;
    
    // make the global purity plots
    // plot the total, absolute numbers of expected events in each category
    plotter.Construct(hist_vec, cat_labels, signal_source_labels, std::vector<float>(), "exp. events", lumi);
    plotter.SaveAs(out_folder + "categorization_S_absolute.pdf");

    // plot the renormalized version
    std::vector<float> sums_signal_only = renormalize_histograms(hist_vec);
    plotter.Construct(hist_vec, cat_labels, signal_source_labels, sums_signal_only, "signal fraction", lumi);        
    plotter.SaveAs(out_folder + "categorization_S.pdf");
}

void make_SB_purity(int fill_histos)
{
    std::vector<TH1F*> signal_hist_vec = generate_signal_histvec(fill_histos);
    std::vector<TH1F*> background_hist_vec = generate_background_histvec(fill_histos);
    
    std::vector<TH1F*> hist_vec(signal_hist_vec);
    hist_vec.insert(hist_vec.end(), background_hist_vec.begin(), background_hist_vec.end());

    std::vector<TString> source_labels(signal_source_labels);
    source_labels.insert(source_labels.end(), background_source_labels.begin(), background_source_labels.end());

    CatPlotter plotter;

    plotter.Construct(hist_vec, cat_labels, source_labels, std::vector<float>(), "exp. events", lumi);
    plotter.SaveAs(out_folder + "categorization_SB_absolute.pdf");
    
    std::vector<float> sums_SB = renormalize_histograms(hist_vec);
    plotter.Construct(hist_vec, cat_labels, source_labels, sums_SB, "signal fraction", lumi);        
    plotter.SaveAs(out_folder + "categorization_SB.pdf");
}

// this treats ALL higgs events as signals, even if their production mode does not agree with the category they have been placed into
void make_SB_ratio(int fill_histos)
{
    std::vector<TH1F*> signal_hist_vec = generate_signal_histvec(fill_histos);
    std::vector<TH1F*> background_hist_vec = generate_background_histvec(fill_histos);

    // generate the vector holding both signal- and background histograms: this makes sure to get the *total* background in each category: the unwanted signal contributions + the actual background
    std::vector<float> S_sums = get_category_sums(signal_hist_vec);

    std::vector<TH1F*> hist_vec(signal_hist_vec);
    hist_vec.insert(hist_vec.end(), background_hist_vec.begin(), background_hist_vec.end());
    std::vector<float> SB_sums = get_category_sums(hist_vec);

    // make the plot of the punzi purity
    std::vector<TH1F*> SB_hist_vec = {new TH1F("SB", "SB", 7, -0.5, 6.5)};

    for(int i = 0; i < SB_hist_vec[0] -> GetSize() - 2; i++)
    {
	SB_hist_vec[0] -> Fill(i, S_sums[i] / SB_sums[i]);
    }

    CatPlotter plotter;

    plotter.Construct(SB_hist_vec, cat_labels, std::vector<TString>(), std::vector<float>(), "S / (S + B)", lumi);       
    plotter.GetStack() -> SetMaximum(1.0);
    plotter.Update();
    plotter.SaveAs(out_folder + "SB.pdf");
}

// is more refined and really takes ONLY those events as signals, whose production mode also agrees with the category
void make_SBfine_ratio(int fill_histos)
{
    std::vector<TH1F*> signal_hist_vec = generate_signal_histvec(fill_histos);
    std::vector<TH1F*> background_hist_vec = generate_background_histvec(fill_histos);
   
    // the correctly classified events
    // --------------------------------------------
    // needs to be revised!!
    float VBF1j_correct_events = signal_hist_vec[VBFhist] -> GetBinContent(VBF1jet_bin);
    float VBF2j_correct_events = signal_hist_vec[VBFhist] -> GetBinContent(VBF2jet_bin);

    float VHlept_correct_events = signal_hist_vec[WHlnuhist] -> GetBinContent(VHleptonic_bin);
    VHlept_correct_events += signal_hist_vec[ZH2lhist] -> GetBinContent(VHleptonic_bin);

    float VHhadr_correct_events = signal_hist_vec[WHXhist] -> GetBinContent(VHhadronic_bin);
    VHhadr_correct_events += signal_hist_vec[ZHXhist] -> GetBinContent(VHhadronic_bin);

    float VHMET_correct_events = signal_hist_vec[WHlnuhist] -> GetBinContent(VHMET_bin);
    VHMET_correct_events += signal_hist_vec[ZHXhist] -> GetBinContent(VHMET_bin);

    float ttH_correct_events = signal_hist_vec[ttH0lhist] -> GetBinContent(ttH_bin);
    ttH_correct_events += signal_hist_vec[ttH1lhist] -> GetBinContent(ttH_bin);
    ttH_correct_events += signal_hist_vec[ttH2lhist] -> GetBinContent(ttH_bin);

    float untagged_correct_events = signal_hist_vec[ggHhist] -> GetBinContent(untagged_bin);
    // -------------------------------------------

    // generate the vector holding both signal- and background histograms: this makes sure to get the *total* background in each category: the unwanted signal contributions + the actual background
    std::vector<TH1F*> hist_vec(signal_hist_vec);
    hist_vec.insert(hist_vec.end(), background_hist_vec.begin(), background_hist_vec.end());
    std::vector<float> sums = get_category_sums(hist_vec); sums.insert(sums.begin(), 0);     // add a new element in front to make the remaining ones effectively 1-indexed
    
    std::vector<TH1F*> SB_hist_vec = {new TH1F("SB_fine", "SB_fine", 7, -0.5, 6.5)};

    SB_hist_vec[0] -> Fill(untagged_bin - 1, untagged_correct_events / sums[untagged_bin]);
    SB_hist_vec[0] -> Fill(VBF1jet_bin - 1, VBF1j_correct_events / sums[VBF1jet_bin]);
    SB_hist_vec[0] -> Fill(VBF2jet_bin - 1, VBF2j_correct_events / sums[VBF2jet_bin]);
    SB_hist_vec[0] -> Fill(ttH_bin - 1, ttH_correct_events / sums[ttH_bin]);
    SB_hist_vec[0] -> Fill(VHhadronic_bin - 1, VHhadr_correct_events / sums[VHhadronic_bin]);
    SB_hist_vec[0] -> Fill(VHleptonic_bin - 1, VHlept_correct_events / sums[VHleptonic_bin]);
    SB_hist_vec[0] -> Fill(VHMET_bin - 1, VHMET_correct_events / sums[VHMET_bin]);

    CatPlotter plotter;

    plotter.Construct(SB_hist_vec, cat_labels, std::vector<TString>(), std::vector<float>(), "S_{fine} / (S_{fine} + B)", lumi);       
    plotter.GetStack() -> SetMaximum(1.0);
    plotter.Update();
    plotter.SaveAs(out_folder + "SB_fine.pdf");
}

void make_punzi(int fill_histos)
{
    std::vector<TH1F*> signal_hist_vec = generate_signal_histvec(fill_histos);
    std::vector<TH1F*> background_hist_vec = generate_background_histvec(fill_histos);
   
    // make the Punzi-purity plot
    // for each signal category, need the number of signal events, the total number of signal events, and the number of background events in this category
    float VBF_events = get_total_events(signal_hist_vec[VBFhist]); // this is the sum of all VBF events (i.e. both VBF 1-jet and VBF 2-jet events)

    float VHhadr_events = get_total_events(signal_hist_vec[WHXhist]);
    VHhadr_events += get_total_events(signal_hist_vec[ZHXhist]);

    float VHlept_events = get_total_events(signal_hist_vec[WHlnuhist]);
    VHlept_events += get_total_events(signal_hist_vec[ZH2lhist]);

    float ttH_events = get_total_events(signal_hist_vec[ttH0lhist]);
    ttH_events += get_total_events(signal_hist_vec[ttH1lhist]);
    ttH_events += get_total_events(signal_hist_vec[ttH2lhist]);

    // untagged are those that are neither VBF, VH or ttH, i.e. only ggH remains
    float untagged_events = get_total_events(signal_hist_vec[ggHhist]);

    // -----------------------------------------
    // needs to be revised!!
    // the correctly classified events
    float VBF1j_correct_events = signal_hist_vec[VBFhist] -> GetBinContent(VBF1jet_bin);
    float VBF2j_correct_events = signal_hist_vec[VBFhist] -> GetBinContent(VBF2jet_bin);

    float VHlept_correct_events = signal_hist_vec[WHlnuhist] -> GetBinContent(VHleptonic_bin);
    VHlept_correct_events += signal_hist_vec[ZH2lhist] -> GetBinContent(VHleptonic_bin);

    float VHhadr_correct_events = signal_hist_vec[WHXhist] -> GetBinContent(VHhadronic_bin);
    VHhadr_correct_events += signal_hist_vec[ZHXhist] -> GetBinContent(VHhadronic_bin);

    float VHMET_correct_events = signal_hist_vec[WHlnuhist] -> GetBinContent(VHMET_bin);
    VHMET_correct_events += signal_hist_vec[ZHXhist] -> GetBinContent(VHMET_bin);

    float ttH_correct_events = signal_hist_vec[ttH0lhist] -> GetBinContent(ttH_bin);
    ttH_correct_events += signal_hist_vec[ttH1lhist] -> GetBinContent(ttH_bin);
    ttH_correct_events += signal_hist_vec[ttH2lhist] -> GetBinContent(ttH_bin);

    float untagged_correct_events = signal_hist_vec[ggHhist] -> GetBinContent(untagged_bin);
    // ----------------------------------

    // generate the vector holding both signal- and background histograms: this makes sure to get the *total* background in each category: the unwanted signal contributions + the actual background
    std::vector<TH1F*> hist_vec(signal_hist_vec);
    hist_vec.insert(hist_vec.end(), background_hist_vec.begin(), background_hist_vec.end());
    std::vector<float> sums = get_category_sums(hist_vec); sums.insert(sums.begin(), 0);     // add a new element in front to make the remaining ones effectively 1-indexed

    float untagged_punzi = get_punzi_purity(
	sums[untagged_bin] - untagged_correct_events, // background in this category
	untagged_correct_events, // signal in this category
	untagged_events, // total signal (important for computing the efficiency)
	5, 2);

    float VBF1j_punzi = get_punzi_purity(
	sums[VBF1jet_bin] - VBF1j_correct_events,
	VBF1j_correct_events,
	VBF_events,
	5, 2);

    float VBF2j_punzi = get_punzi_purity(
	sums[VBF2jet_bin] - VBF2j_correct_events,
	VBF2j_correct_events,
	VBF_events,
	5, 2);

    float ttH_punzi = get_punzi_purity(
	sums[ttH_bin] - ttH_correct_events,
	ttH_correct_events,
	ttH_events,
	5, 2);

    float VHhadr_punzi = get_punzi_purity(
	sums[VHhadronic_bin] - VHhadr_correct_events,
	VHhadr_correct_events,
	VHhadr_events,
	5, 2);

    float VHlept_punzi = get_punzi_purity(
	sums[VHleptonic_bin] - VHlept_correct_events,
	VHlept_correct_events,
	VHlept_events,
	5, 2);
    
    // make the plot of the punzi purity
    std::vector<TH1F*> punzi_hist_vec = {new TH1F("punzi_purity", "punzi_purity", 7, -0.5, 6.5)};

    punzi_hist_vec[0] -> Fill(untagged_bin - 1, untagged_punzi);
    punzi_hist_vec[0] -> Fill(VBF1jet_bin - 1, VBF1j_punzi);
    punzi_hist_vec[0] -> Fill(VBF2jet_bin - 1, VBF2j_punzi);
    punzi_hist_vec[0] -> Fill(ttH_bin - 1, ttH_punzi);
    punzi_hist_vec[0] -> Fill(VHhadronic_bin - 1, VHhadr_punzi);
    punzi_hist_vec[0] -> Fill(VHleptonic_bin - 1, VHlept_punzi);
    
    CatPlotter plotter;
    plotter.Construct(punzi_hist_vec, cat_labels, std::vector<TString>(), std::vector<float>(), "Punzi purity", lumi);   
    plotter.SaveAs(out_folder + "punzi.pdf");
}

int main( int argc, char *argv[] )
{
    make_SB_purity(kTRUE);
    make_S_purity(kFALSE);
    make_punzi(kFALSE);
    make_SB_ratio(kFALSE);
    make_SBfine_ratio(kFALSE);

    return(0);
}
