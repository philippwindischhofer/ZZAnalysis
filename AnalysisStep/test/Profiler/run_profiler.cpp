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
    Mor18Config conf("/data_CMS/cms/wind/CJLST_NTuples_ZZMask_all/");

    auto var = [&](Tree* in) -> float{return in -> ZZMass;};

    TString quantity = "ZZMass";
    TString y_label = "events";
    TString category_name = "";
    int number_bins = 100;
    float axis_lower = 0.0;
    float axis_upper = 300.0;
    int category = -1;
    bool normalize = false;

    TString plot_name = "ZZMass_ZHl";
    make_plot1d("ZH125", new Routing(extraLeptons_2_cut, "ZHlhist"), var, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    plot_name = "AllData";
    make_plot1d("AllData", new Routing(no_cut, "VBFhist"), var, number_bins, axis_lower, axis_upper, normalize, quantity, y_label, category_name, plot_name, conf);

    return(0);
}
