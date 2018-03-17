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
#include "TKDE.h"
#include "TSpline.h"
#include "TF1.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/cuts.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/utils.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Profiler.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/ProfPlotter.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Discriminant.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/DiscriminantCollection.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/EventStream.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/MEDiscriminantFactory.h>

#include <ZZAnalysis/AnalysisStep/interface/Discriminants.h>

TString out_folder = "../../src/ZZAnalysis/CalibratorPlots/";

void calibrate_discriminant(std::vector<TString> H1_paths, std::vector<std::function<bool(Tree*)>> H1_cuts, std::vector<TString> H0_paths, std::vector<std::function<bool(Tree*)>> H0_cuts, const std::function<bool(Tree*)>& disc_cut, const std::function<float(Tree*)>& disc, TString disc_name, Config& conf)
{
    Profiler* prof = new Profiler();
    ProfPlotter* plotter = new ProfPlotter();

    // histograms that are going to hold the distributions of the raw MELA discriminant under the signal- and background hypothesis
    // from those, build the likelihood ratio of the MELA-discriminant
    TH1F* H1_distrib = new TH1F("H1_distrib", "H1_distrib", 50, -0.02, 1.02);
    H1_distrib -> SetLineColor(kBlue - 9);
    H1_distrib -> SetFillColor(kWhite);

    TH1F* H0_distrib = new TH1F("H0_distrib", "H0_distrib", 50, -0.02, 1.02);
    H0_distrib -> SetLineColor(kRed - 7);
    H0_distrib -> SetFillColor(kWhite);

    // make the signal distribution
    for(auto tup: boost::combine(H1_paths, H1_cuts))
    {
	std::function<bool(Tree*)> H1_cut;
	TString H1_path;
	boost::tie(H1_path, H1_cut) = tup;

	auto total_cut = [&](Tree* in) -> bool {
	    return (H1_cut(in) &&
		    disc_cut(in)) ? 
	    kTRUE : kFALSE;};
	
	prof -> FillProfile(H1_path, conf.lumi(), H1_distrib, total_cut, disc, false);
    }

    for(auto tup: boost::combine(H0_paths, H0_cuts))
    {
	std::function<bool(Tree*)> H0_cut;
	TString H0_path;
	boost::tie(H0_path, H0_cut) = tup;

	auto total_cut = [&](Tree* in) -> bool {
	    return (H0_cut(in) &&
		    disc_cut(in)) ? 
	    kTRUE : kFALSE;};
	
	prof -> FillProfile(H0_path, conf.lumi(), H0_distrib, total_cut, disc, false);
    }

    // only now do the normalization of both distributions
    double sig_norm = 1.0 / H1_distrib -> Integral("width");
    double bkg_norm = 1.0 / H0_distrib -> Integral("width");
    H1_distrib -> Scale(sig_norm);
    H0_distrib -> Scale(bkg_norm);

    TH1F* H1_distrib_smooth = (TH1F*)(H1_distrib -> Clone("H1_distrib_smooth"));
    TH1F* H0_distrib_smooth = (TH1F*)(H0_distrib -> Clone("H0_distrib_smooth"));

    // smoothen the histograms
    H1_distrib_smooth -> Smooth(0);
    H0_distrib_smooth -> Smooth(0);

    // now make a fit to the binned, normalized histograms
    TSpline3* H1_spline = new TSpline3(H1_distrib_smooth);
    TSpline3* H0_spline = new TSpline3(H0_distrib_smooth);
    H1_spline -> SetLineColor(kBlue - 9);
    H0_spline -> SetLineColor(kRed - 7);
    H1_spline -> SetLineWidth(3);
    H0_spline -> SetLineWidth(3);

    // Plot the two distributions, for visualization (and cross-checking) purposes
    std::vector<TH1F*> hist_vec = {H1_distrib, H0_distrib};
    std::vector<TString> source_labels = {"H_{1}", "H_{0}"};

    plotter -> Construct(hist_vec, source_labels, disc_name, "normalized to 1", "", "", "nostack");
    
    plotter -> GetCanvas() -> cd();
    H1_spline -> Draw("same");
    H0_spline -> Draw("same");

    // Save the calibration splines to a file
    TFile* outfile = new TFile(out_folder + disc_name + "_calines.root", "recreate");
    outfile -> cd();
    H1_spline -> Write();
    H0_spline -> Write();
    H1_distrib -> Write();
    H0_distrib -> Write();
    outfile -> Close();
    delete outfile;

    plotter -> SaveAs(out_folder + disc_name + ".pdf");
}


void calibrate_discriminant(Discriminant* disc, Config& conf)
{
    std::vector<TString> H1_paths = disc -> GetH1Source() -> GetPaths();
    std::vector<std::function<bool(Tree*)>> H1_cuts = disc -> GetH1Source() -> GetCuts();
    std::vector<TString> H0_paths = disc -> GetH0Source() -> GetPaths();
    std::vector<std::function<bool(Tree*)>> H0_cuts = disc -> GetH0Source() -> GetCuts();

    // now need to iterate over the components defined in the Discriminant and calibrate each one
    std::vector<TString> names = disc -> GetNames();
    std::vector<std::function<bool(Tree*)>> cuts = disc -> GetCuts();
    std::vector<std::function<float(Tree*)>> discs = disc -> GetDiscs();
    
    for(auto tup: boost::combine(names, cuts, discs))
    {
	std::function<bool(Tree*)> cut;
	std::function<float(Tree*)> disc;
	TString name;
	boost::tie(name, cut, disc) = tup;

	calibrate_discriminant(H1_paths, H1_cuts, H0_paths, H0_cuts, cut, disc, name, conf);
    }
}

void calibrate_discriminant_collection(DiscriminantCollection* coll, Config& conf)
{
    std::map<std::pair<TString, TString>, Discriminant*> discs = coll -> GetDiscs();

    for(auto cur: discs)
    {
	calibrate_discriminant(cur.second, conf);
    }
}

int main(int argc, char *argv[])
{    
    Mor18Config conf;

    DiscriminantCollection* coll = MEDiscriminantFactory::GenerateDiscriminantCollection(out_folder, conf);

    calibrate_discriminant_collection(coll, conf);

    // TString H1_file = "VBFH125";
    // TString H0_file = "ggH125";
    // TString disc_name = "VBF2j-ggH";

    // calibrate_discriminant(H1_file, H0_file, conf, 2jcut, DVBF2j_ME, disc_name);   

    // // try to read them back and evaluate them
    // TFile* infile = new TFile(out_folder + disc_name + "_calines.root");
    // TSpline3* H1_spline = (TSpline3*)(infile -> Get("H1_distrib_smooth") -> Clone());
    // TSpline3* H0_spline = (TSpline3*)(infile -> Get("H0_distrib_smooth") -> Clone());
    // infile -> Close();
    
    // std::cout << H1_spline -> Eval(0.0) << std::endl;
    // std::cout << H0_spline -> Eval(0.0) << std::endl;

    return(0);
}
