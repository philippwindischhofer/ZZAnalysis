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
#include <ZZAnalysis/AnalysisStep/test/classlib/include/MLDiscriminantFactory.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/MLDiscriminantFactoryFullCategorySet.h>

#include <ZZAnalysis/AnalysisStep/interface/Discriminants.h>

void calibrate_discriminant(std::vector<TString> H1_paths, std::vector<std::function<bool(Tree*)>> H1_cuts, std::vector<TString> H0_paths, std::vector<std::function<bool(Tree*)>> H0_cuts, const std::function<bool(Tree*)>& disc_cut, const std::function<float(Tree*)>& disc, TString disc_name, Config& conf, TString out_folder, TString H1_name = "H_{1}", TString H0_name = "H_{0}")
{
    Profiler* prof = new Profiler();
    ProfPlotter* plotter = new ProfPlotter();

    // histograms that are going to hold the distributions of the raw MELA discriminant under the signal- and background hypothesis
    // from those, build the likelihood ratio of the MELA-discriminant
    TH1F* H1_distrib = new TH1F("H1_distrib", "H1_distrib", 50, -0.02, 1.02);
    H1_distrib -> SetLineColor(kBlue - 9);
    H1_distrib -> SetMarkerColor(kBlue - 9);
    H1_distrib -> SetFillColor(kWhite);

    TH1F* H0_distrib = new TH1F("H0_distrib", "H0_distrib", 50, -0.02, 1.02);
    H0_distrib -> SetLineColor(kRed - 7);
    H0_distrib -> SetMarkerColor(kRed - 7);
    H0_distrib -> SetFillColor(kWhite);

    TH1F* H1_distrib_validation = new TH1F("H1_distrib_validation", "H1_distrib_validation", 50, -0.02, 1.02);
    H1_distrib_validation -> SetLineColor(kWhite);
    H1_distrib_validation -> SetFillColor(kWhite);
    H1_distrib_validation -> SetMarkerStyle(21);
    H1_distrib_validation -> SetMarkerColor(kBlue - 9);

    TH1F* H0_distrib_validation = new TH1F("H0_distrib_validation", "H0_distrib_validation", 50, -0.02, 1.02);
    H0_distrib_validation -> SetLineColor(kWhite);
    H0_distrib_validation -> SetFillColor(kWhite);
    H0_distrib_validation -> SetMarkerStyle(21);
    H0_distrib_validation -> SetMarkerColor(kRed - 7);

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
	
	prof -> FillProfile(H1_path, conf.lumi(), H1_distrib, total_cut, disc, false, 0.0, 0.5);
	prof -> FillProfile(H1_path, conf.lumi(), H1_distrib_validation, total_cut, disc, false, 0.5, 1.0);
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
	
	prof -> FillProfile(H0_path, conf.lumi(), H0_distrib, total_cut, disc, false, 0.0, 0.5);
	prof -> FillProfile(H0_path, conf.lumi(), H0_distrib_validation, total_cut, disc, false, 0.5, 1.0);
    }

    // only now do the normalization of both distributions
    double sig_norm = 1.0 / H1_distrib -> Integral("width");
    double bkg_norm = 1.0 / H0_distrib -> Integral("width");
    H1_distrib -> Scale(sig_norm);
    H0_distrib -> Scale(bkg_norm);

    double sig_norm_validation = 1.0 / H1_distrib_validation -> Integral("width");
    double bkg_norm_validation = 1.0 / H0_distrib_validation -> Integral("width");
    H1_distrib_validation -> Scale(sig_norm_validation);
    H0_distrib_validation -> Scale(bkg_norm_validation);

    TH1F* H1_distrib_smooth = (TH1F*)(H1_distrib -> Clone("H1_distrib_smooth"));
    TH1F* H0_distrib_smooth = (TH1F*)(H0_distrib -> Clone("H0_distrib_smooth"));

    // smoothen the histograms
    H1_distrib_smooth -> Smooth(1);
    H0_distrib_smooth -> Smooth(1);

    // now make a fit to the binned, normalized histograms
    TSpline3* H1_spline = new TSpline3(H1_distrib_smooth);
    TSpline3* H0_spline = new TSpline3(H0_distrib_smooth);
    H1_spline -> SetLineColor(kBlue - 9);
    H0_spline -> SetLineColor(kRed - 7);
    H1_spline -> SetLineWidth(3);
    H0_spline -> SetLineWidth(3);

    // Plot the two distributions, for visualization (and cross-checking) purposes
    std::vector<TH1F*> hist_vec = {H1_distrib_validation, H0_distrib_validation, H1_distrib, H0_distrib};
    std::vector<TString> source_labels = {H1_name + " (validation)", H0_name + " (validation)", H1_name + " (training)", H0_name + " (training)"};

    plotter -> Construct(hist_vec, source_labels, disc_name, "normalized to 1", "", "", "P H nostack");
    
    plotter -> GetCanvas() -> cd();
    // H1_spline -> Draw("same");
    // H0_spline -> Draw("same");

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

void calibrate_discriminant(Discriminant* disc, Config& conf, TString out_folder)
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

	calibrate_discriminant(H1_paths, H1_cuts, H0_paths, H0_cuts, cut, disc, name, conf, out_folder);
    }
}

void calibrate_discriminant_collection(DiscriminantCollection* coll, Config& conf, TString out_folder)
{
    std::map<std::pair<TString, TString>, Discriminant*> discs = coll -> GetDiscs();

    for(auto cur: discs)
    {
	calibrate_discriminant(cur.second, conf, out_folder);
    }
}

int main(int argc, char *argv[])
{    
    if(argc != 4)
    {
	std::cerr << "Error: exactly 3 arguments are required" << std::endl;
	return(-1);
    }

    TString MCpath = argv[1];
    TString switchval = argv[2];
    TString out_folder = argv[3];

    Mor18Config conf(MCpath);

    DiscriminantCollection* coll;

    // switches between the trained discriminants (ML) or MELA values only (ME)
    if(switchval == "ME")
    {
	coll = MEDiscriminantFactory::GenerateDiscriminantCollection(out_folder, conf);
    }
    else if(switchval = "ML")
    {
	//coll = MLDiscriminantFactory::GenerateDiscriminantCollection(out_folder, conf);
	coll = MLDiscriminantFactoryFullCategorySet::GenerateDiscriminantCollection(out_folder, conf);
    }
    else
    {
	std::cerr << "Error: did not specify any correct option (ME / ML)" << std::endl;
	return(-1);
    }
	
    calibrate_discriminant_collection(coll, conf, out_folder);

    return(0);
}
