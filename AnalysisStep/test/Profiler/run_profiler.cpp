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
    Mor18Config conf("/data_CMS/cms/wind/CJLST_NTuples_prepared/");

    TString quantity;
    TString plot_name;

    TString y_label = "events";
    TString category_name = "";
    int number_bins;
    float axis_lower;
    float axis_upper;
    int category = -1;
    bool normalize = false;

    auto var1 = [&](Tree* in) -> float{return in -> ZZMass;};
    axis_lower = 100.0;
    axis_upper = 500.0;
    number_bins = 100;
    quantity = "ZZMass";
    plot_name = "ZZMass_ZHl";
    make_plot1d("ZH125", new Routing(extraLeptons_2_cut, "ZHlhist"), var1, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "ZZMass_WminusHl";
    make_plot1d("WminusH125", new Routing(extraLeptons_1_cut, "WminusHlhist"), var1, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "ZZMass_WplusHl";
    make_plot1d("WplusH125", new Routing(extraLeptons_1_cut, "WplusHlhist"), var1, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "ZZMass_AllData";
    make_plot1d("AllData", new Routing(no_cut, "datahist"), var1, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    // -------------------------------------------------

    auto var2 = [&](Tree* in) -> float{return in -> nExtraLep;};
    axis_lower = 0;
    axis_upper = 5;
    number_bins = 6;
    quantity = "nExtraLep";
    plot_name = "nExtraLep_ZHl";
    make_plot1d("ZH125", new Routing(extraLeptons_2_cut, "ZHlhist"), var2, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "nExtraLep_WminusHl";
    make_plot1d("WminusH125", new Routing(extraLeptons_1_cut, "WminusHlhist"), var2, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "nExtraLep_WplusHl";
    make_plot1d("WplusH125", new Routing(extraLeptons_1_cut, "WplusHlhist"), var2, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "nExtraLep_AllData";
    make_plot1d("AllData", new Routing(no_cut, "datahist"), var2, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    // -------------------------------------------------

    auto var3 = [&](Tree* in) -> float{return in -> PFMET;};
    axis_lower = 0;
    axis_upper = 500;
    number_bins = 100;
    quantity = "PFMET";
    plot_name = "PFMET_ZHl";
    make_plot1d("ZH125", new Routing(extraLeptons_2_cut, "ZHlhist"), var3, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "PFMET_WminusHl";
    make_plot1d("WminusH125", new Routing(extraLeptons_1_cut, "WminusHlhist"), var3, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "PFMET_WplusHl";
    make_plot1d("WplusH125", new Routing(extraLeptons_1_cut, "WplusHlhist"), var3, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "PFMET_AllData";
    make_plot1d("AllData", new Routing(no_cut, "datahist"), var3, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    // -------------------------------------------------

    auto var4 = [&](Tree* in) -> float{return in -> ZZEta;};
    axis_lower = -3;
    axis_upper = 3;
    number_bins = 100;
    quantity = "ZZEta";
    plot_name = "ZZEta_ZHl";
    make_plot1d("ZH125", new Routing(extraLeptons_2_cut, "ZHlhist"), var4, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "ZZEta_WminusHl";
    make_plot1d("WminusH125", new Routing(extraLeptons_1_cut, "WminusHlhist"), var4, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "ZZEta_WplusHl";
    make_plot1d("WplusH125", new Routing(extraLeptons_1_cut, "WplusHlhist"), var4, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "ZZEta_AllData";
    make_plot1d("AllData", new Routing(no_cut, "datahist"), var4, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    // -------------------------------------------------

    auto var5 = [&](Tree* in) -> float{return in -> Z1Mass;};
    axis_lower = 40;
    axis_upper = 120;
    number_bins = 100;
    quantity = "Z1Mass";
    plot_name = "Z1Mass_ZHl";
    make_plot1d("ZH125", new Routing(extraLeptons_2_cut, "ZHlhist"), var5, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "Z1Mass_WminusHl";
    make_plot1d("WminusH125", new Routing(extraLeptons_1_cut, "WminusHlhist"), var5, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "Z1Mass_WplusHl";
    make_plot1d("WplusH125", new Routing(extraLeptons_1_cut, "WplusHlhist"), var5, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "Z1Mass_AllData";
    make_plot1d("AllData", new Routing(no_cut, "datahist"), var5, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "Z1Mass_ggH";
    make_plot1d("ggH125", new Routing(no_cut, "datahist"), var5, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);


    // -------------------------------------------------

    auto var6 = [&](Tree* in) -> float{return in -> Z2Mass;};
    axis_lower = 15;
    axis_upper = 120;
    number_bins = 100;
    quantity = "Z2Mass";
    plot_name = "Z2Mass_ZHl";
    make_plot1d("ZH125", new Routing(extraLeptons_2_cut, "ZHlhist"), var6, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "Z2Mass_WminusHl";
    make_plot1d("WminusH125", new Routing(extraLeptons_1_cut, "WminusHlhist"), var6, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "Z2Mass_WplusHl";
    make_plot1d("WplusH125", new Routing(extraLeptons_1_cut, "WplusHlhist"), var6, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "Z2Mass_AllData";
    make_plot1d("AllData", new Routing(no_cut, "datahist"), var6, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "Z2Mass_ggH";
    make_plot1d("ggH125", new Routing(no_cut, "datahist"), var6, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    return(0);
}
