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
#include <ZZAnalysis/AnalysisStep/test/Profiler/include/histo_settings.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/Classifier.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/Mor17Classifier.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/Mor18Classifier.h>
#include <ZZAnalysis/AnalysisStep/interface/Category.h>
#include <ZZAnalysis/AnalysisStep/interface/Discriminants.h>
#include <ZZAnalysis/AnalysisStep/interface/cConstants.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/cuts.h>

// global file locations
float lumi = 35.9f;

TString path = "/data_CMS/cms/tsculac/CJLST_NTuples/";
TString file_name = "/ZZ4lAnalysis.root";
TString out_folder = "../../src/ZZAnalysis/ProfilerPlots/";

void make_plots1d(Classifier* testclass, float lumi, std::function<float(Tree*)> var, int number_bins, float lower, float upper, std::function<bool(Tree*)> ext_cut, int category, TString x_label, TString y_label, TString label_left, bool normalize, TString out_file)
{
    Profiler* prof = new Profiler();
    ProfPlotter* plotter = new ProfPlotter();

    TString label_right = Form("%.2f fb^{-1} (13TeV)", lumi);

    std::vector<TH1F*> hist_vec(hist_names.size());
    std::vector<TString> path_vec(file_names.size());

    for(unsigned int i = 0; i < hist_names.size(); i++)
    {
	hist_vec[i] = new TH1F(file_names[i], file_names[i], number_bins, lower, upper);
	if(normalize)
	    hist_vec[i] -> SetFillColor(kWhite);
	else
	    hist_vec[i] -> SetFillColor(source_colors[i]);

	hist_vec[i] -> SetLineColor(source_colors[i]);
	hist_vec[i] -> SetFillStyle(source_styles[i]);
    }
    
    for(unsigned int i = 0; i < file_names.size(); i++)
    {
	path_vec[i] = path + file_names[i] + file_name;
    }

    std::function<bool(Tree*)> cut;

    if(category == -1)
    {
	// don't fix a certain category, but use just the external cut and the mass cut, as always
	cut = [&](Tree* in) -> bool {
	    return (ext_cut(in) &&
		    mZZ_cut(in)) ? 
	    kTRUE : kFALSE;};

    }
    else
    {
	cut = [&](Tree* in) -> bool {
	    return (testclass -> ClassifyThisEvent(in) == category &&
		    ext_cut(in) &&
		    mZZ_cut(in)) ? 
	    kTRUE : kFALSE;};
    }

    prof -> FillProfile(path_vec[0], lumi, hist_vec[ggHhist], cut, var, normalize);
    prof -> FillProfile(path_vec[1], lumi, hist_vec[VBFhist], cut, var, normalize);
    prof -> FillProfile(path_vec[2], lumi, hist_vec[WHhist], cut, var, normalize);
    prof -> FillProfile(path_vec[3], lumi, hist_vec[WHhist], cut, var, normalize);
    prof -> FillProfile(path_vec[4], lumi, hist_vec[ZHhist], cut, var, normalize);
    prof -> FillProfile(path_vec[5], lumi, hist_vec[ttHhist], cut, var, normalize);
    prof -> FillProfile(path_vec[6], lumi, hist_vec[qqhist], cut, var, normalize);
    prof -> FillProfile(path_vec[7], lumi, hist_vec[gghist], cut, var, normalize);
    prof -> FillProfile(path_vec[8], lumi, hist_vec[gghist], cut, var, normalize);
    prof -> FillProfile(path_vec[9], lumi, hist_vec[gghist], cut, var, normalize);
    prof -> FillProfile(path_vec[10], lumi, hist_vec[gghist], cut, var, normalize);
    prof -> FillProfile(path_vec[11], lumi, hist_vec[gghist], cut, var, normalize);
    prof -> FillProfile(path_vec[12], lumi, hist_vec[gghist], cut, var, normalize);

    if(normalize)
	plotter -> Construct(hist_vec, source_labels, x_label, y_label, label_left, label_right, "nostack");
    else
	plotter -> Construct(hist_vec, source_labels, x_label, y_label, label_left, label_right, "");
    plotter -> SaveAs(out_folder + out_file + ".pdf");
}

void make_plots2d(std::function<float(Tree*)> var_x, std::function<float(Tree*)> var_y)
{
   TCanvas* canv = new TCanvas("canv", "canv", 800, 800);
   TH2F* hist2d = new TH2F("hist2d", "hist2d", 100, -1.1, -0.9, 100, -1.1, -0.9);

   Profiler* prof = new Profiler();

   TString input_file = path + "ggH125" + file_name;

   prof -> FillProfile(input_file, lumi, hist2d, no_cut, var_x, var_y, false);

   canv -> cd();
   hist2d -> Draw();

   canv -> SaveAs(out_folder + "2dhist.pdf");
}

int main(int argc, char *argv[])
{
    Mor17Classifier* testclass = new Mor17Classifier();

    // auto var_x = [&](Tree* in) -> float {return in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal;};
    // auto var_y = [&](Tree* in) -> float {return in -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal;};

    // make_plots2d(var_x, var_y);

/*
    // do a sample plot: look only at the ones that have -1
    auto var = [&](Tree* in) -> float{return in -> ZZMass;};
    TString plot_name = "mZZ";
    TString quantity = "m_{4l}";
    TString y_label = "Events";
    TString category_name = "";
    int number_bins = 100;
    float axis_lower = 118.0;
    float axis_upper = 130.0;
    int category = -1;
    bool normalize = false;

    auto local_cut = [&](Tree* in) -> bool {return (in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal == -1.0 && in -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal) ? kTRUE : kFALSE;};

    make_plots1d(testclass, lumi, var, number_bins, axis_lower, axis_upper, local_cut, category, quantity, y_label, category_name, normalize, plot_name);
*/
/*
    // do a sample plot
    auto var = [&](Tree* in) -> float{return in -> ZZMass;};
    TString plot_name = "mZZ";
    TString quantity = "m_{4l}";
    TString y_label = "Events";
    TString category_name = "t#bar{t}H tagged";
    int number_bins = 100;
    float axis_lower = 118.0;
    float axis_upper = 130.0;
    int category = ttHTaggedMor17;
    bool normalize = false;

    make_plots1d(testclass, lumi, var, number_bins, axis_lower, axis_upper, no_cut, category, quantity, y_label, category_name, normalize, plot_name);

    make_plots1d(testclass, lumi, var, number_bins, axis_lower, axis_upper, final_state_4mu_cut, category, quantity, y_label, category_name + ", ZZ #rightarrow 4#mu", normalize, plot_name + "_4mu");
    make_plots1d(testclass, lumi, var, number_bins, axis_lower, axis_upper, final_state_4e_cut, category, quantity, y_label, category_name + ", ZZ #rightarrow 4e", normalize, plot_name + "_4e");
    make_plots1d(testclass, lumi, var, number_bins, axis_lower, axis_upper, final_state_2e2mu_cut, category, quantity, y_label, category_name + ", ZZ #rightarrow 2e2#mu", normalize, plot_name + "_2e2mu");
*/

    // plot the kinematic discriminant for VBF-2j
    auto var = [&](Tree* in) -> float{return DVBF2j_ME(in -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal, in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, in -> ZZMass);};
    
    // auto var = [&](Tree* in) -> float{return DVBF1j_ME(
    // 	    in -> p_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
    // 	    in -> pAux_JVBF_SIG_ghv1_1_JHUGen_JECNominal,
    // 	    in -> p_JQCD_SIG_ghg2_1_JHUGen_JECNominal,
    // 	    in -> ZZMass
    // 	    );};
    
    TString plot_name = "DVBF2j_ME";
    TString quantity = "DVBF2j_ME";
    TString y_label = "normalized to 1";
    TString category_name = "";
    int number_bins = 50;
    float axis_lower = 0.44;
    float axis_upper = 0.48;
    int category = -1; // no category
    bool normalize = true;

    make_plots1d(testclass, lumi, var, number_bins, axis_lower, axis_upper, no_cut, category, quantity, y_label, category_name, normalize, plot_name);
    // make_plots1d(testclass, lumi, var, number_bins, axis_lower, axis_upper, final_state_4mu_cut, category, quantity, y_label, category_name + "ZZ #rightarrow 4#mu", normalize, plot_name + "_4mu");
    // make_plots1d(testclass, lumi, var, number_bins, axis_lower, axis_upper, final_state_4e_cut, category, quantity, y_label, category_name + "ZZ #rightarrow 4e", normalize, plot_name + "_4e");
    // make_plots1d(testclass, lumi, var, number_bins, axis_lower, axis_upper, final_state_2e2mu_cut, category, quantity, y_label, category_name + "ZZ #rightarrow 2e2#mu", normalize, plot_name + "_2e2mu");


    return(0);
}
