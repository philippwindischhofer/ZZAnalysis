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

#include <boost/range/combine.hpp>

#include <ZZAnalysis/AnalysisStep/interface/Category.h>
#include <ZZAnalysis/AnalysisStep/interface/Discriminants.h>
#include <ZZAnalysis/AnalysisStep/interface/cConstants.h>

// own local files
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Profiler.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/ProfPlotter.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Classifier.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Classifier.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/cuts.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>

TString out_folder = "../../src/ZZAnalysis/ProfilerPlots/";

void make_plots1d(Classifier* testclass, std::function<float(Tree*)> var, int number_bins, float lower, float upper, std::function<bool(Tree*)> ext_cut, int category, TString x_label, TString y_label, TString label_left, bool normalize, TString out_file, Config& conf, float start_fraction = 0.0, float end_fraction = 1.0)
{
    Profiler* prof = new Profiler();
    ProfPlotter* plotter = new ProfPlotter();

    TString label_right = Form("%.2f fb^{-1} (13TeV)", conf.lumi());

    // create the histograms
    std::map<TString, TH1F*> hist_vec;
    std::vector<TString> hist_names = conf.hist_names();

    for(auto& hist_name: hist_names)
    {
	TH1F* new_hist = new TH1F(hist_name, hist_name, number_bins, lower, upper);
	
	if(normalize)
	    new_hist -> SetFillColor(kWhite);
	else
	    new_hist -> SetFillColor(conf.source_color(hist_name));

	new_hist -> SetLineColor(conf.source_color(hist_name));
	new_hist -> SetFillStyle(conf.source_style(hist_name));

	hist_vec.insert(std::make_pair(hist_name, new_hist));
    }

    // now iterate over the routing and fill the histograms
    std::vector<std::pair<TString, Routing*>> routings = conf.get_routing();

    for(auto& cur: routings)
    {
	TString file_name = cur.first;
	TString file_path = conf.MC_path() + file_name + conf.MC_filename();

	Routing* routing = cur.second;

	std::function<bool(Tree*)> cut;

	if(category == -1)
	{
	    // don't fix a certain category, but use just the external cut and the mass cut, as always
	    cut = [&](Tree* in) -> bool {
		return (ext_cut(in) &&
			mZZ_cut(in) &&
			(routing -> cut)(in)) ? 
		kTRUE : kFALSE;};

	}
	else
	{
	    cut = [&](Tree* in) -> bool {
		return (testclass -> ClassifyThisEvent(in) == category &&
			ext_cut(in) &&
			mZZ_cut(in) &&
			(routing -> cut)(in)) ? 
		kTRUE : kFALSE;};
	}
	
	prof -> FillProfile(file_path, conf.lumi(), hist_vec[routing -> hist_name], cut, var, normalize, start_fraction, end_fraction);
    }

    if(normalize)
    	plotter -> Construct(hist_vec, conf, x_label, y_label, label_left, label_right, "nostack");
    else
    	plotter -> Construct(hist_vec, conf, x_label, y_label, label_left, label_right, "");

    plotter -> SaveAs(out_folder + out_file + ".pdf");
}

void make_plot1d(TString file_name, Routing* routing, std::function<float(Tree*)> var, int number_bins, float lower, float upper, bool normalize, TString x_label, TString y_label, TString label_left, TString out_file, Config& conf, float start_fraction = 0.0, float end_fraction = 1.0)
{
    Profiler* prof = new Profiler();
    ProfPlotter* plotter = new ProfPlotter();

    TString label_right = Form("%.2f fb^{-1} (13TeV)", conf.lumi());

    // create the histograms
    std::map<TString, TH1F*> hist_vec;
    TString hist_name = routing -> hist_name;

    TH1F* new_hist = new TH1F(hist_name, hist_name, number_bins, lower, upper);
    
    if(normalize)
	new_hist -> SetFillColor(kWhite);
    else
	new_hist -> SetFillColor(kBlue - 9);
    
    new_hist -> SetLineColor(kBlue - 9);    
    hist_vec.insert(std::make_pair(hist_name, new_hist));

    TString file_path = conf.MC_path() + file_name + conf.MC_filename();
    std::function<bool(Tree*)> cut;
    
    prof -> FillProfile(file_path, conf.lumi(), hist_vec[routing -> hist_name], routing -> cut, var, normalize, start_fraction, end_fraction);
    plotter -> Construct(hist_vec, conf, x_label, y_label, label_left, label_right, "");
    plotter -> SaveAs(out_folder + out_file + ".pdf");
}

// skeleton code for doing 2d histograms also. to be completed
void make_plots2d(std::function<float(Tree*)> var_x, std::function<float(Tree*)> var_y, Config& conf, float start_fraction, float end_fraction)
{
   TCanvas* canv = new TCanvas("canv", "canv", 800, 800);
   TH2F* hist2d = new TH2F("hist2d", "hist2d", 100, -1.1, -0.9, 100, -1.1, -0.9);

   Profiler* prof = new Profiler();

   TString input_file = conf.MC_path() + "ggH125" + conf.MC_filename();

   prof -> FillProfile(input_file, conf.lumi(), hist2d, no_cut, var_x, var_y, false, start_fraction, end_fraction);

   canv -> cd();
   hist2d -> Draw();

   canv -> SaveAs(out_folder + "2dhist.pdf");
}

int main(int argc, char *argv[])
{
    Mor18Classifier* testclass = new Mor18Classifier();
    Mor18Config conf;

    // auto j1cut = [&](Tree* in) -> bool {
    // 	return (((in -> nCleanedJetsPt30 == 1) ? kTRUE : kFALSE)) ? 
    // 	kTRUE : kFALSE;};

    auto j2cut = [&](Tree* in) -> bool {
    	return (((in -> nCleanedJetsPt30 >= 2) && mZZ_cut(in) ? kTRUE : kFALSE)) ? 
    	kTRUE : kFALSE;};

    auto var = [&](Tree* in) -> float{return in -> D_WHh_ggH_ML;};

    TString plot_name = "VBF_ggH";
    TString quantity = "VBF_ggH";
    TString y_label = "events";
    TString category_name = "";
    int number_bins = 100;
    float axis_lower = 0.0;
    float axis_upper = 1.0;
    int category = -1;
    bool normalize = false;

    make_plot1d("WplusH125", new Routing(j2cut, "VBFhist"), var, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    //make_plots1d(testclass, var, number_bins, axis_lower, axis_upper, j2cut, category, quantity, y_label, category_name, normalize, plot_name, conf);

    // ------------------------------------------------------------------

//     // do a sample plot
//     auto var2 = [&](Tree* in) -> float{return in -> ZZMass;};
//     plot_name = "mZZ";
//     quantity = "m_{4l}";
//     y_label = "normalized to 1";
//     category_name = "";
//     number_bins = 100;
//     axis_lower = 118.0;
//     axis_upper = 130.0;
//     category = -1;
//     normalize = true;

//     make_plots1d(testclass, var2, number_bins, axis_lower, axis_upper, no_cut, category, quantity, y_label, category_name, normalize, plot_name, conf);

//     // make_plots1d(testclass, var2, number_bins, axis_lower, axis_upper, final_state_4mu_cut, category, quantity, y_label, category_name + ", ZZ #rightarrow 4#mu", normalize, plot_name + "_4mu");
//     // make_plots1d(testclass, var2, number_bins, axis_lower, axis_upper, final_state_4e_cut, category, quantity, y_label, category_name + ", ZZ #rightarrow 4e", normalize, plot_name + "_4e");
//     // make_plots1d(testclass, var2, number_bins, axis_lower, axis_upper, final_state_2e2mu_cut, category, quantity, y_label, category_name + ", ZZ #rightarrow 2e2#mu", normalize, plot_name + "_2e2mu");

//     // ------------------------------------------------------------------


//     // do a sample plot
//     auto var3 = [&](Tree* in) -> float{return in -> PFMET;};
//     plot_name = "PFMET";
//     quantity = "PFMET";
//     y_label = "normalized to 1";
//     category_name = "";
//     number_bins = 100;
//     axis_lower = 0.0;
//     axis_upper = 200.0;
//     category = -1;
//     normalize = true;

//     make_plots1d(testclass, var3, number_bins, axis_lower, axis_upper, no_cut, category, quantity, y_label, category_name, normalize, plot_name, conf);

//     // do a sample plot
//     auto var4 = [&](Tree* in) -> float{return in -> nExtraLep;};
//     plot_name = "nExtraLep";
//     quantity = "nExtraLep";
//     y_label = "normalized to 1";
//     category_name = "";
//     number_bins = 4;
//     axis_lower = 0.0;
//     axis_upper = 4.0;
//     category = -1;
//     normalize = true;

//     make_plots1d(testclass, var4, number_bins, axis_lower, axis_upper, no_cut, category, quantity, y_label, category_name, normalize, plot_name, conf);


//     // do a sample plot
//     auto var44 = [&](Tree* in) -> float{return in -> nExtraZ;};
//     plot_name = "nExtraZ";
//     quantity = "nExtraZ";
//     y_label = "normalized to 1";
//     category_name = "";
//     number_bins = 4;
//     axis_lower = 0.0;
//     axis_upper = 4.0;
//     category = -1;
//     normalize = true;

//     make_plots1d(testclass, var44, number_bins, axis_lower, axis_upper, no_cut, category, quantity, y_label, category_name, normalize, plot_name, conf);


//     // do a sample plot
//     auto var5 = [&](Tree* in) -> float{return in -> nCleanedJetsPt30;};
//     plot_name = "nCleanedJetsPt30";
//     quantity = "nCleanedJetsPt30";
//     y_label = "normalized to 1";
//     category_name = "";
//     number_bins = 8;
//     axis_lower = 0.0;
//     axis_upper = 8.0;
//     category = -1;
//     normalize = true;

//     make_plots1d(testclass, var5, number_bins, axis_lower, axis_upper, no_cut, category, quantity, y_label, category_name, normalize, plot_name, conf);

//     // do a sample plot
//     auto var6 = [&](Tree* in) -> float{return in -> nCleanedJetsPt30BTagged_bTagSF;};
//     plot_name = "nCleanedJetsPt30BTagged_bTagSF";
//     quantity = "nCleanedJetsPt30BTagged_bTagSF";
//     y_label = "normalized to 1";
//     category_name = "";
//     number_bins = 8;
//     axis_lower = 0.0;
//     axis_upper = 8.0;
//     category = -1;
//     normalize = true;

//     make_plots1d(testclass, var6, number_bins, axis_lower, axis_upper, no_cut, category, quantity, y_label, category_name, normalize, plot_name, conf);


//     // // ------------------------------------------------------------------    

//     // plot the kinematic discriminant for VBF-2j (as an example for a normalized distribution plot)
// //    auto var3 = [&](Tree* in) -> float{return DVBF2j_ME(in -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal, in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, in -> ZZMass);};
//     // auto var3 = [&](Tree* in) -> float{return in -> ZZMass;};
    
//     // plot_name = "DVBF2j_ME_python";
//     // quantity = "DVBF2j_ME";
//     // y_label = "normalized to 1";
//     // category_name = "";
//     // number_bins = 50;
//     // axis_lower = 118.0;
//     // axis_upper = 130.0;
//     // category = -1; // no category
//     // normalize = false;

//     // auto local_cut = no_cut;//[&](Tree* in) -> bool {return (in -> nCleanedJetsPt30 >= 2) ? kTRUE : kFALSE;};

//     // make_plots1d(testclass, var3, number_bins, axis_lower, axis_upper, local_cut, category, quantity, y_label, category_name, normalize, plot_name);
//     // make_plots1d(testclass, var3, number_bins, axis_lower, axis_upper, 
//     // 		 [&](Tree* in) -> bool {return final_state_4mu_cut(in) && local_cut(in);}, category, quantity, y_label, 
//     // 		 category_name + "ZZ #rightarrow 4#mu", normalize, plot_name + "_4mu");
//     // make_plots1d(testclass, var3, number_bins, axis_lower, axis_upper, 
//     // 		 [&](Tree* in) -> bool {return final_state_4e_cut(in) && local_cut(in);}, category, quantity, y_label, 
//     // 		 category_name + "ZZ #rightarrow 4e", normalize, plot_name + "_4e");
//     // make_plots1d(testclass, var3, number_bins, axis_lower, axis_upper, 
//     // 		 [&](Tree* in) -> bool {return final_state_2e2mu_cut(in) && local_cut(in);}, category, quantity, y_label, 
//     // 		 category_name + "ZZ #rightarrow 2e2#mu", normalize, plot_name + "_2e2mu");

//     // ------------------------------------------------------------------    

//     //LikClassifier* lik = new LikClassifier();
//     //auto var4 = [&](Tree* in) -> float{return lik -> GetDiscriminant(in);};

//     // compare it to a plot of the likelihood-ratio improved VBF-discriminant
//     plot_name = "DVBF2j_ME_lik";
//     quantity = "DVBF2j_ME_lik";
//     y_label = "normalized to 1";
//     category_name = "";
//     number_bins = 50;
//     axis_lower = 0.0;
//     axis_upper = 1.0;
//     category = -1; // no category
//     normalize = true;

//     //make_plots1d(testclass, var4, number_bins, axis_lower, axis_upper, local_cut, category, quantity, y_label, category_name, normalize, plot_name);
//     // make_plots1d(testclass, var3, number_bins, axis_lower, axis_upper, 
//     // 		 [&](Tree* in) -> bool {return final_state_4mu_cut(in) && local_cut(in);}, category, quantity, y_label, 
//     // 		 category_name + "ZZ #rightarrow 4#mu", normalize, plot_name + "_4mu");
//     // make_plots1d(testclass, var3, number_bins, axis_lower, axis_upper, 
//     // 		 [&](Tree* in) -> bool {return final_state_4e_cut(in) && local_cut(in);}, category, quantity, y_label, 
//     // 		 category_name + "ZZ #rightarrow 4e", normalize, plot_name + "_4e");
//     // make_plots1d(testclass, var3, number_bins, axis_lower, axis_upper, 
//     // 		 [&](Tree* in) -> bool {return final_state_2e2mu_cut(in) && local_cut(in);}, category, quantity, y_label, 
//     // 		 category_name + "ZZ #rightarrow 2e2#mu", normalize, plot_name + "_2e2mu");
    

    return(0);
}
