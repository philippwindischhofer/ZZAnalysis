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
#include "TF1.h"

#include <ZZAnalysis/AnalysisStep/test/KDE/include/histo_settings.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/cuts.h>
#include <ZZAnalysis/AnalysisStep/test/Benchmarker/include/utils.h>
#include <ZZAnalysis/AnalysisStep/test/Profiler/include/Profiler.h>
#include <ZZAnalysis/AnalysisStep/test/Profiler/include/ProfPlotter.h>

#include <ZZAnalysis/AnalysisStep/interface/Discriminants.h>

float lumi = 35.9f;

TString path = "/data_CMS/cms/tsculac/CJLST_NTuples/";
TString file_name = "/ZZ4lAnalysis.root";
TString out_folder = "../../src/ZZAnalysis/KDEPlots/";

void build_KDE()
{
    Profiler* prof = new Profiler();
    ProfPlotter* plotter = new ProfPlotter();

    // histograms that are going to hold the distributions of the raw MELA discriminant under the signal- and background hypothesis
    // from those, build the likelihood ratio of the MELA-discriminant
    TH1F* sig_distrib = new TH1F("sig_distrib", "sig_distrib", 50, 0.0, 1.0);
    sig_distrib -> SetLineColor(kBlue - 9);
    sig_distrib -> SetFillColor(kWhite);

    TH1F* bkg_distrib = new TH1F("bkg_distrib", "bkg_distrib", 50, 0.0, 1.0);
    bkg_distrib -> SetLineColor(kRed - 7);
    bkg_distrib -> SetFillColor(kWhite);

    // build the list of complete file paths for where to look for the signal and background MC data: the first one in the list is for the signal, all the others are for the "backgrounds", i.e. all the other higgs production modes and the "actual" background. NOTE: put all the "backgrounds" into the same histogram, since their distributions are very, very siilar. Don't expect to gain / lose anything much anyways
    std::vector<TString> path_vec(file_names.size());
    for(unsigned int i = 0; i < file_names.size(); i++)
    {
	path_vec[i] = path + file_names[i] + file_name;
    }

    // don't fix a certain category, but use just the external cut and the mass cut, as always

    auto local_cut = [&](Tree* in) -> bool {return (in -> nCleanedJetsPt30 >= 2) ? kTRUE : kFALSE;};
    auto cut = [&](Tree* in) -> bool {
    	return (local_cut(in) &&
    		mZZ_cut(in)) ? 
    	kTRUE : kFALSE;};

    // need the raw MELA kinematic discriminant
    auto var = [&](Tree* in) -> float{return DVBF2j_ME(
	    in -> p_JJVBF_SIG_ghv1_1_JHUGen_JECNominal, 
	    in -> p_JJQCD_SIG_ghg2_1_JHUGen_JECNominal, 
	    in -> ZZMass);
    };

    // make the signal distribution
    prof -> FillProfile(path_vec[0], lumi, sig_distrib, cut, var, false);

    // make the background distribution with all the other files
    for(unsigned int i = 1; i < file_names.size(); i++)
    {
    	prof -> FillProfile(path_vec[i], lumi, bkg_distrib, cut, var, false);
    }

    // for the KDE, in principle also need the *full* dataset at once
    std::vector<double> data_vec_sig;
    std::vector<double> weight_vec_sig;

    std::vector<double> data_vec_bkg;
    std::vector<double> weight_vec_bkg;

    prof -> FillDataWeights(path_vec[0], lumi, cut, var, &data_vec_sig, &weight_vec_sig);

    for(unsigned int i = 1; i < file_names.size(); i++)
    {
	prof -> FillDataWeights(path_vec[i], lumi, cut, var, &data_vec_bkg, &weight_vec_bkg);
    }

    // only now do the normalization of both distributions
    double sig_norm = 1.0 / sig_distrib -> Integral("width");
    double bkg_norm = 1.0 / bkg_distrib -> Integral("width");
    sig_distrib -> Scale(sig_norm);
    bkg_distrib -> Scale(bkg_norm);

    // plot the two distributions, for visualization (and cross-checking) purposes
    std::vector<TH1F*> hist_vec = {sig_distrib, bkg_distrib};
    std::vector<TString> source_labels = {"VBF", "background"};

    plotter -> Construct(hist_vec, source_labels, "DVBF2j_ME", "normalized to 1", "", "", "nostack");
    plotter -> SaveAs(out_folder + "VBF2JME" + ".pdf");

    // now do the kernel density estimation
    TKDE* sig_kde = new TKDE(data_vec_sig.size(), data_vec_sig.data(), weight_vec_sig.data(), 0.0, 1.0, "", 1.0);
    sig_kde -> SetMirror(TKDE::kMirrorBoth);
    sig_kde -> SetKernelType(TKDE::kGaussian);

    TKDE* bkg_kde = new TKDE(data_vec_bkg.size(), data_vec_bkg.data(), weight_vec_bkg.data(), 0.0, 1.0, "", 1.0);
    bkg_kde -> SetMirror(TKDE::kMirrorBoth);
    bkg_kde -> SetKernelType(TKDE::kGaussian);

    std::cout << "built KDEs" << std::endl;
    std::cout << "data_vec_bkg.size() = " << data_vec_bkg.size() << std::endl;
    std::cout << "data_vec_sig.size() = " << data_vec_sig.size() << std::endl;

    TF1* func_sig = sig_kde -> GetFunction(100, 0.0, 1.0);
    TF1* func_bkg = bkg_kde -> GetFunction(100, 0.0, 1.0);

    TCanvas* canv = new TCanvas("canv", "canv", 800, 600);
    canv -> cd();

    TH1F* sig_kde_hist = new TH1F("sig_kde_hist", "sig_kde_hist", 1000, 0.0, 1.0);
    TH1F* bkg_kde_hist = new TH1F("bkg_kde_hist", "bkg_kde_hist", 1000, 0.0, 1.0);
    sig_kde_hist -> Add(func_sig);
    bkg_kde_hist -> Add(func_bkg);

    TH1F* lik_hist = new TH1F("lik_hist", "lik_hist", 1000, 0.0, 1.0);
    gStyle -> SetOptStat(0);

    lik_hist -> Divide(sig_kde_hist, bkg_kde_hist);
    lik_hist -> Scale(1.0 / lik_hist -> GetMaximum());
    
    // save the likelihood-ratio diagram
    std::vector<TH1F*> out_vec = {lik_hist};
    save_histos(out_folder + "lik_hist" + ".root", out_vec);

    lik_hist -> Draw("hist p");
    canv -> SaveAs(out_folder + "KDE" + ".pdf");
}

int main(int argc, char *argv[])
{    
    build_KDE();   

    return(0);
}
