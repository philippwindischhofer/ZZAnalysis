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
#include <ZZAnalysis/AnalysisStep/test/classlib/include/MLDiscriminantFactoryFullCategorySetDynamic.h>

#include <ZZAnalysis/AnalysisStep/interface/Discriminants.h>

float number_bins_freedman_diaconis(TH1F* hist)
{
    float number_entries = hist -> GetEntries();

    // now need to compute the inter-quartil-range (IQR):
    double quantile_values[2];
    double quantiles[2] = {0.25, 0.75}; // need the first and the third quartile

    hist -> GetQuantiles(2, quantile_values, quantiles);

    float IQR = quantile_values[1] - quantile_values[0];

    // the optimum bin width according to Freedman and Diaconis
    float bin_width = 2.0 * IQR * TMath::Power(number_entries, -1.0 / 3.0);

    float number_bins = 1.0 / bin_width;

    std::cout << "number_entries = " << number_entries << std::endl;
    std::cout << "1st quartile = " << quantile_values[0] << std::endl;
    std::cout << "3rd quartile = " << quantile_values[1] << std::endl;
    std::cout << "IQR = " << IQR << std::endl;
    std::cout << "number of bins = " << number_bins << std::endl;

    return number_bins;
}

int closest_integer_divisor(int candidate_divisor, int dividend)
{
    if(candidate_divisor == 0)
	return 0;

    int upper_candidate = candidate_divisor;
    int lower_candidate = candidate_divisor;

    while((dividend%upper_candidate) != 0 && upper_candidate < dividend)
	upper_candidate++;

    while((dividend%lower_candidate) != 0 && lower_candidate > 0)
	lower_candidate--;

    if((upper_candidate - candidate_divisor) < (candidate_divisor - lower_candidate))
	return upper_candidate;
    else
	return lower_candidate;
}

void calibrate_discriminant(std::vector<TString> H1_paths, std::vector<std::function<bool(Tree*)>> H1_cuts, std::vector<TString> H0_paths, std::vector<std::function<bool(Tree*)>> H0_cuts, const std::function<bool(Tree*)>& disc_cut, const std::function<float(Tree*)>& disc, TString disc_name, Config& conf, TString out_folder, float start_fraction = 0.0, float end_fraction = 0.5, TString H1_name = "H_{1}", TString H0_name = "H_{0}")
{
    Profiler* prof = new Profiler();
    ProfPlotter* plotter = new ProfPlotter();

    // histograms that are going to hold the distributions of the raw MELA discriminant under the signal- and background hypothesis
    // from those, build the likelihood ratio of the MELA-discriminant

    // 55440 is a highly composite number!
    // First, bin all the data with an absurdely high number of bins to determine the optimal number of bins, then re-bin them with the correct settings
    int original_number_bins = 55440;
    TH1F* H1_distrib = new TH1F("H1_distrib", "H1_distrib", original_number_bins, -0.02, 1.02);
    H1_distrib -> SetLineColor(kBlue - 9);
    H1_distrib -> SetMarkerColor(kBlue - 9);
    H1_distrib -> SetFillColor(kWhite);

    TH1F* H0_distrib = new TH1F("H0_distrib", "H0_distrib", original_number_bins, -0.02, 1.02);
    H0_distrib -> SetLineColor(kRed - 7);
    H0_distrib -> SetMarkerColor(kRed - 7);
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

	prof -> FillProfile(H1_path, conf.lumi(), H1_distrib, total_cut, disc, false, start_fraction, end_fraction);
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
	
	prof -> FillProfile(H0_path, conf.lumi(), H0_distrib, total_cut, disc, false, start_fraction, end_fraction);
    }

    // now rebin each histogram separately to have a well-behaved density-estimator at the end
    int number_bins_H0 = round(number_bins_freedman_diaconis(H0_distrib));
    int number_bins_H1 = round(number_bins_freedman_diaconis(H1_distrib));

    std::cout << "rebinning with: number_bins_H0 = " << number_bins_H0 << std::endl;
    std::cout << "rebinning with: number_bins_H1 = " << number_bins_H1 << std::endl;

    H1_distrib = new TH1F("H1_distrib", "H1_distrib", number_bins_H1, -0.02, 1.02);
    H1_distrib -> SetLineColor(kBlue - 9);
    H1_distrib -> SetMarkerColor(kBlue - 9);
    H1_distrib -> SetFillColor(kWhite);

    H0_distrib = new TH1F("H0_distrib", "H0_distrib", number_bins_H0, -0.02, 1.02);
    H0_distrib -> SetLineColor(kRed - 7);
    H0_distrib -> SetMarkerColor(kRed - 7);
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
	
	prof -> FillProfile(H1_path, conf.lumi(), H1_distrib, total_cut, disc, false, start_fraction, end_fraction);
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
	
	prof -> FillProfile(H0_path, conf.lumi(), H0_distrib, total_cut, disc, false, start_fraction, end_fraction);
    }

    // only now do the normalization of both distributions
    double sig_norm = 1.0 / H1_distrib -> Integral("width");
    double bkg_norm = 1.0 / H0_distrib -> Integral("width");
    H1_distrib -> Scale(sig_norm);
    H0_distrib -> Scale(bkg_norm);

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
    std::vector<TH1F*> hist_vec = {H1_distrib, H0_distrib};
    // std::vector<TString> source_labels = {H1_name + " (validation: 0.5 - 0.75)", 
    // 					  H0_name + " (validation: 0.5 - 0.75)", 
    // 					  H1_name + Form(" (%.2f - %.2f)", start_fraction, end_fraction),
    // 					  H0_name + Form(" (%.2f - %.2f)", start_fraction, end_fraction)};

    std::vector<TString> source_labels = {H1_name,
					  H0_name};

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

void calibrate_discriminant(Discriminant* disc, Config& conf, TString out_folder, float start_fraction, float end_fraction)
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

	calibrate_discriminant(H1_paths, H1_cuts, H0_paths, H0_cuts, cut, disc, name, conf, out_folder, start_fraction, end_fraction);
    }
}

void calibrate_discriminant_collection(DiscriminantCollection* coll, Config& conf, TString out_folder, float start_fraction, float end_fraction)
{
    std::map<std::pair<TString, TString>, Discriminant*> discs = coll -> GetDiscs();

    for(auto cur: discs)
    {
	calibrate_discriminant(cur.second, conf, out_folder, start_fraction, end_fraction);
    }
}

int main(int argc, char *argv[])
{    
    if(argc != 6)
    {
	std::cerr << "Error: exactly 5 arguments are required" << std::endl;
	return(-1);
    }

    TString MCpath = argv[1];
    TString switchval = argv[2];
    TString out_folder = argv[3];

    float start_fraction = std::atof(argv[4]);
    float end_fraction = std::atof(argv[5]);

    Mor18Config conf(MCpath);

    DiscriminantCollection* coll;

    std::cout << "start_fraction = " << start_fraction << std::endl;
    std::cout << "end_fraction = " << end_fraction << std::endl;

    // switches between the trained discriminants (ML) or MELA values only (ME)
    if(switchval == "ME")
    {
	coll = MEDiscriminantFactory::GenerateDiscriminantCollection(out_folder, conf);
    }
    else if(switchval = "ML")
    {
	//coll = MLDiscriminantFactory::GenerateDiscriminantCollection(out_folder, conf);
	TString config_path = out_folder + "../settings.conf";
	coll = MLDiscriminantFactoryFullCategorySetDynamic::GenerateDiscriminantCollection(out_folder, config_path, conf);
    }
    else
    {
	std::cerr << "Error: did not specify any correct option (ME / ML)" << std::endl;
	return(-1);
    }
	
    calibrate_discriminant_collection(coll, conf, out_folder, start_fraction, end_fraction);

    return(0);
}
