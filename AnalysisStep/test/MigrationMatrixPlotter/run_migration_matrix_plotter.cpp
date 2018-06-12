// C++
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

// ROOT
#include "TColor.h"
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
#include "TMath.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/PlottingUtils.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18ConfigReducedCategorySet.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Classifier.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18LIClassifier.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/ConfigFileHandler.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/cuts.h>

double ComputeKFactor(TString input_file_name, Tree* in)
{
    float retval = 1.0;

    if(input_file_name.Contains("ZZTo4l"))
    {
	retval = (in -> KFactor_EW_qqZZ) * (in -> KFactor_QCD_qqZZ_M); // As of Moriond2016
    }
    else if(input_file_name.Contains("ggTo"))
    {
	retval = in -> KFactor_QCD_ggZZ_Nominal; // as of Moriond2016
    }

    return retval;
}

int main(int argc, char *argv[])
{
    // produces the migration matrix for comparing nicely the old and new categorization

    TString package_path = "/data_CMS/cms/wind/180531_optimized_hyperparameters_systematics_packaged/";
    TString engine = "robin";

    TString MCpath = package_path +  "/CJLST_NTuples/";
    TString config_file_path = package_path + "settings.conf";
    TString calibration_dir = package_path + "/calibration/";
    TString priors_file_path = package_path + "/priors_" + engine + "/priors.txt";
    
    Mor18Config* conf = new Mor18Config(MCpath, 41.37, false, 125.0, true);
    std::vector<std::pair<TString, Routing*>> routings = conf -> get_routing();

    int number_categories = conf -> categories().size();

    // prepare the classifiers
    Classifier* bayes_cat = new Mor18LIClassifier(calibration_dir, config_file_path, engine);
    // load priors here and assign them etc.
    ConfigFileHandler* handler = new ConfigFileHandler(priors_file_path, "read");
    float VBF_prior = handler -> GetField("VBF_prior");
    float ggH_prior = handler -> GetField("ggH_prior");
    float WHhadr_prior = handler -> GetField("WHhadr_prior");
    float ZHhadr_prior = handler -> GetField("ZHhadr_prior");
    float WHlept_prior = handler -> GetField("WHlept_prior");
    float ZHlept_prior = handler -> GetField("ZHlept_prior");
    float ZHMET_prior = handler -> GetField("ZHMET_prior");
    float ttHhadr_prior = handler -> GetField("ttHhadr_prior");
    float ttHlept_prior = handler -> GetField("ttHlept_prior");
    float bkg_prior = handler -> GetField("ZX_prior");
    float qq_prior = handler -> GetField("qq_prior");

    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << " using the following priors: " << std::endl;
    std::cout << " VBF_prior = " << VBF_prior << std::endl;
    std::cout << " ggH_prior = " << ggH_prior << std::endl;
    std::cout << " WHhadr_prior = " << WHhadr_prior << std::endl;
    std::cout << " ZHhadr_prior = " << ZHhadr_prior << std::endl;
    std::cout << " WHlept_prior = " << WHlept_prior << std::endl;
    std::cout << " ZHlept_prior = " << ZHlept_prior << std::endl;
    std::cout << " ZHMET_prior = " << ZHMET_prior << std::endl;
    std::cout << " ttHhadr_prior = " << ttHhadr_prior << std::endl;
    std::cout << " ttHlept_prior = " << ttHlept_prior << std::endl;
    std::cout << " ZX_prior = " << bkg_prior << std::endl;
    std::cout << " qq_prior = " << qq_prior << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;

    Mor18LIClassifier* bayes_cat18 = static_cast<Mor18LIClassifier*>(bayes_cat);
    bayes_cat18 -> SetEngineParameter("min_iterations", 25);
    bayes_cat18 -> SetEngineParameter("max_iterations", 125);

    bayes_cat18 -> SetPriors(VBF_prior, ggH_prior, WHhadr_prior, ZHhadr_prior, WHlept_prior, ZHlept_prior, ZHMET_prior, ttHhadr_prior, ttHlept_prior, bkg_prior, qq_prior);

    Classifier* Mor18_cat = new Mor18Classifier();

    std::map<int, int> category_map = {
	{UntaggedMor18, 0},
	{VBF1jTaggedMor18, 1},
	{VBF2jTaggedMor18, 2},
	{VHLeptTaggedMor18, 3},
	{VHHadrTaggedMor18, 4},
	{ttHLeptTaggedMor18, 5},
	{ttHHadrTaggedMor18, 6},
	{VHMETTaggedMor18, 7}
    };

    TH2F* migration_matrix = new TH2F("migration_matrix", "", number_categories, -0.5, number_categories - 0.5, number_categories, -0.5, number_categories - 0.5);

    for(auto& cur: routings)
    {
	TString file_name = cur.first;
	TString file_path = conf -> MC_path() + file_name + conf -> MC_filename();

	TString tree_name = "ClassTree";
    
	std::cout << "reading from " << file_path << std::endl;
    
	// read some auxiliary information
	TFile* input_file = new TFile(file_path);
	TH1F* hCounters = (TH1F*)input_file -> Get(tree_name + "/Counters");
	Long64_t n_gen_events = (Long64_t)hCounters -> GetBinContent(1);
	Long64_t gen_sum_weights = (Long64_t)hCounters -> GetBinContent(40);

	TTree* input_tree = (TTree*)input_file -> Get(tree_name + "/candTree");

	Tree* buffer = new Tree();
	buffer -> Init(input_tree, file_path);

	// total number of entries stored in the entire chain
	Long64_t n_entries = buffer -> fChain -> GetEntriesFast();
	std::cout << "total number of entries = " << n_entries << std::endl;

	Routing* routing = cur.second;
	    
	// tie together the total cut to be used
	auto cut = [&](Tree* in) -> bool {
	    return (mZZ_cut(in) &&
		    (routing -> cut)(in)) ?
	    kTRUE : kFALSE;
	};

	for(Long64_t j_entry = 0; j_entry < n_entries; j_entry++)
	{
	    // get the correct tree in the chain that contains this event
	    Long64_t i_entry = buffer -> LoadTree(j_entry);
	    if(i_entry < 0) break;
	    
	    // now actually read this entry
	    buffer -> fChain -> GetEntry(j_entry);
	    
	    if ( !(buffer -> ZZsel >= 90) ) continue;
	    
	    float event_weight = (conf -> lumi() * buffer -> xsec * 1000. * buffer -> overallEventWeight * ComputeKFactor(file_path, buffer)) / gen_sum_weights;

	    if(file_path.Contains("ggH"))
	    {
		event_weight *= (buffer -> ggH_NNLOPS_weight);
	    }
	    
	    if(cut(buffer))
	    {
		// event accepted for this production mode
		// Note: doing this would not be strictly necessary, as only compute the migration matrix inclusively in the production mode
		int Mor18_result = Mor18_cat -> ClassifyThisEvent(buffer);
		int bayes_result = bayes_cat -> ClassifyThisEvent(buffer);

		migration_matrix -> Fill(category_map[Mor18_result], category_map[bayes_result], event_weight);
	    }
	}
    }

    for(int bayes_cat = 0; bayes_cat < number_categories; bayes_cat++)
    {
	for(int Mor18_cat = 0; Mor18_cat < number_categories; Mor18_cat++)
	{
	    std::cout << migration_matrix -> GetBinContent(Mor18_cat + 1, bayes_cat + 1) << "\t";
	}
	std::cout << std::endl;
    }

    // now normalize it such that each entry for the Bayesian categorization is normalized to unity
    for(int bayes_cat = 0; bayes_cat < number_categories; bayes_cat++)
    {
	float running_sum = 0;
	for(int Mor18_cat = 0; Mor18_cat < number_categories; Mor18_cat++)
	{
	    running_sum += migration_matrix -> GetBinContent(Mor18_cat + 1, bayes_cat + 1);
	}
	for(int Mor18_cat = 0; Mor18_cat < number_categories; Mor18_cat++)
	{
	    float cur_val = migration_matrix -> GetBinContent(Mor18_cat + 1, bayes_cat + 1);
	    float norm_val = cur_val / running_sum > 0.00 ? cur_val / running_sum : 0.0;

	    migration_matrix -> SetBinContent(Mor18_cat + 1, bayes_cat + 1, norm_val);
	}
    }

    TCanvas* canv = new TCanvas("canv", "canv", 10, 10, 800, 600);
    gStyle -> SetOptStat(0);
    canv -> SetTicks(1, 1);

    // put the nice color palette
    int num_colors = 3;
    double stops[] = {0, 0.5, 1.0};
    double reds[] = {0.9686274528503418,0.41708574, 0.031372550874948502};
    double greens[] = {0.9843137264251709, 0.68063055, 0.18823529779911041};
    double blues[] = {1.00, 0.83823146, 0.41960784792900085};
    int num_internal_colors = 150;

    TColor::CreateGradientColorTable(num_colors, stops, reds, greens, blues, num_internal_colors);

    //gStyle -> SetPalette(kOcean);
    //TColor::InvertPalette();

    canv -> SetLeftMargin(0.20);
    canv -> SetRightMargin(0.01);
    canv -> SetTopMargin(0.01);
    canv -> SetBottomMargin(0.22);

    canv -> cd();
    migration_matrix -> Draw("col");

    // put categorial labels
    migration_matrix -> GetXaxis() -> SetLabelOffset(100);
    migration_matrix -> GetYaxis() -> SetLabelOffset(100);

    TLatex t;
    float x;
    float y;

    // x axis first
    t.SetTextAlign(22);
    t.SetTextSize(0.03);
    t.SetTextAngle(-45);
    y = gPad -> GetUymin() - 1.45 * migration_matrix -> GetYaxis() -> GetBinWidth(1);
    for(int i = 0; i < number_categories; i++)
    {
	x = migration_matrix -> GetXaxis() -> GetBinCenter(i + 1);
	t.DrawLatex(x, y, conf -> cat_label(i));
    }

    t.SetTextSize(0.04);
    t.SetTextAngle(0);
    t.DrawLatex(0.5 * (migration_matrix -> GetXaxis() -> GetBinCenter(number_categories)), y - 0.95 * migration_matrix -> GetYaxis() -> GetBinWidth(1), "current categorization");

    // now y axis
    t.SetTextSize(0.03);
    t.SetTextAlign(22);
    x = gPad -> GetUxmin() - 1.25 * migration_matrix -> GetXaxis() -> GetBinWidth(1);
    for(int i = 0; i < number_categories; i++)
    {
	y = migration_matrix -> GetYaxis() -> GetBinCenter(i + 1);
	t.DrawLatex(x, y, conf -> cat_label(i));
    }    

    t.SetTextSize(0.04);
    t.SetTextAngle(90);
    t.DrawLatex(x - 0.95 * migration_matrix -> GetXaxis() -> GetBinWidth(1), 0.5 * (migration_matrix -> GetXaxis() -> GetBinCenter(number_categories)), "Bayesian categorization");    

    canv -> SaveAs(package_path + "/category_migration.pdf");

    // give the values themselves
    t.SetTextAngle(0);
    t.SetTextSize(0.03);
    for(int bayes_cat = 0; bayes_cat < number_categories; bayes_cat++)
    {
	for(int Mor18_cat = 0; Mor18_cat < number_categories; Mor18_cat++)
	{
	    x = migration_matrix -> GetXaxis() -> GetBinCenter(Mor18_cat + 1);
	    y = migration_matrix -> GetYaxis() -> GetBinCenter(bayes_cat + 1);
	    if(migration_matrix -> GetBinContent(Mor18_cat + 1, bayes_cat + 1) > 0.8)
	    {
		t.SetTextColor(kWhite);
	    }
	    else
	    {
		t.SetTextColor(kBlack);
	    }
	    t.DrawLatex(x, y, Form("%.2f", migration_matrix -> GetBinContent(Mor18_cat + 1, bayes_cat + 1)));
	}
	std::cout << std::endl;
    }    

    canv -> SaveAs(package_path + "/category_migration_with_values.pdf");

    for(int bayes_cat = 0; bayes_cat < number_categories; bayes_cat++)
    {
	for(int Mor18_cat = 0; Mor18_cat < number_categories; Mor18_cat++)
	{
	    std::cout << migration_matrix -> GetBinContent(Mor18_cat + 1, bayes_cat + 1) << "\t";
	}
	std::cout << std::endl;
    }

    migration_matrix -> Draw("col same");
    
    std::cout << "done" << std::endl;

    return(0);
}
