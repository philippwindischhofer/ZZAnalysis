#include <ZZAnalysis/AnalysisStep/test/classlib/include/opt_utils.h>

// generates a vector of histograms corresponding to the signals
std::vector<TH1F*> generate_signal_histvec(int fill_histos, Classifier* classifier, const std::function<bool(Tree*)>& cut, TString data_id, TString out_path, Config& conf)
{
    TString hist_storage(data_id + "_signal.root");
    std::vector<TString> signal_path;

    for(unsigned int i = 0; i < conf.signal_file_names().size(); i++)
    {
	signal_path.push_back(conf.MC_path() + conf.signal_file_names()[i] + conf.MC_filename());
    }
	
    std::vector<TH1F*> hist_vec(conf.signal_hist_names().size());

    for(unsigned int i = 0; i < conf.signal_hist_names().size(); i++)
    {
	hist_vec[i] = new TH1F(conf.signal_hist_names()[i], conf.signal_hist_names()[i], conf.cat_labels().size(), -0.5, conf.cat_labels().size() - 0.5);
	hist_vec[i] -> SetFillColor(conf.signal_source_colors()[i]);
	hist_vec[i] -> SetLineColor(conf.signal_source_colors()[i]);
	hist_vec[i] -> SetFillStyle(1001);
    }    

    if(fill_histos)
    {	
	std::cout << "filling histograms" << std::endl;

	// for the signal files, can be very specific and select precisely the correct differential channel
	// ideally, create one separate histogram (by using generator-level cuts) for each signal category. Then, can compute the efficiency for this signal to end up in the matching signal category
	classifier -> FillHistogram(signal_path[0], conf.lumi(), hist_vec[conf.hist_index("ggHhist")], [&](Tree* in) -> bool{return(mZZ_cut(in) && cut(in));});
	classifier -> FillHistogram(signal_path[1], conf.lumi(), hist_vec[conf.hist_index("VBFhist")], [&](Tree* in) -> bool{return(mZZ_cut(in) && cut(in));});
	classifier -> FillHistogram(signal_path[2], conf.lumi(), hist_vec[conf.hist_index("WHXhist")], [&](Tree* in) -> bool{return(extraLeptons_0_cut(in) && cut(in));});
	classifier -> FillHistogram(signal_path[2], conf.lumi(), hist_vec[conf.hist_index("WHlnuhist")], [&](Tree* in) -> bool{return(extraLeptons_1_cut(in) && cut(in));}); // W decays leptonically
	classifier -> FillHistogram(signal_path[3], conf.lumi(), hist_vec[conf.hist_index("WHXhist")], [&](Tree* in) -> bool{return(extraLeptons_0_cut(in) && cut(in));});
	classifier -> FillHistogram(signal_path[3], conf.lumi(), hist_vec[conf.hist_index("WHlnuhist")], [&](Tree* in) -> bool{return(extraLeptons_1_cut(in) && cut(in));}); // W decays leptonically
	classifier -> FillHistogram(signal_path[4], conf.lumi(), hist_vec[conf.hist_index("ZHXhist")], [&](Tree* in) -> bool{return(extraNeutrinos_0_Leptons_0_cut(in) && cut(in));}); // no extra leptons (electrons, muons) and neutrinos for the hadronic decay of the Z
	classifier -> FillHistogram(signal_path[4], conf.lumi(), hist_vec[conf.hist_index("ZHnunuhist")], [&](Tree* in) -> bool{return(extraNeutrinos_2_cut(in) && cut(in));});
	classifier -> FillHistogram(signal_path[4], conf.lumi(), hist_vec[conf.hist_index("ZH2lhist")], [&](Tree* in) -> bool{return(extraLeptons_2_cut(in) && cut(in));}); // need exactly two extra leptons to have the Z decaying leptonically
	classifier -> FillHistogram(signal_path[5], conf.lumi(), hist_vec[conf.hist_index("ttH0lhist")], [&](Tree* in) -> bool{return(extraLeptons_0_cut(in) && cut(in));});    
	classifier -> FillHistogram(signal_path[5], conf.lumi(), hist_vec[conf.hist_index("ttH1lhist")], [&](Tree* in) -> bool{return(extraLeptons_1_cut(in) && cut(in));});
	classifier -> FillHistogram(signal_path[5], conf.lumi(), hist_vec[conf.hist_index("ttH2lhist")], [&](Tree* in) -> bool{return(extraLeptons_2_cut(in) && cut(in));});    
	std::cout << "end filling histograms" << std::endl;

	save_histos(out_path + conf.storage_prefix() + hist_storage, hist_vec);
    }    

    // read the histograms back from the root file
    hist_vec = read_histos(out_path + conf.storage_prefix() + hist_storage, conf.signal_hist_names());

    std::cout << "end preparing signal" << std::endl;

    return hist_vec;
}

std::vector<TH1F*> generate_background_histvec(int fill_histos, Classifier* classifier, const std::function<bool(Tree*)>& cut, TString data_id, TString out_path, Config& conf)
{
    TString hist_storage(data_id + "_background.root");
    std::vector<TString> background_path;

    for(unsigned int i = 0; i < conf.background_file_names().size(); i++)
    {
	background_path.push_back(conf.MC_path() + conf.background_file_names()[i] + conf.MC_filename());
    }

    std::vector<TH1F*> hist_vec(conf.background_hist_names().size());

    for(unsigned int i = 0; i < conf.background_hist_names().size(); i++)
    {
	hist_vec[i] = new TH1F(conf.background_hist_names()[i], conf.background_hist_names()[i], conf.cat_labels().size(), -0.5, conf.cat_labels().size() - 0.5);
	hist_vec[i] -> SetFillStyle(3244);
	hist_vec[i] -> SetLineColor(conf.background_source_colors()[i]);
	hist_vec[i] -> SetFillColor(conf.background_source_colors()[i]);
    }

    if(fill_histos)
    {	
	// No need to fill the histograms every time!
	std::cout << "filling background histograms" << std::endl;
	
	// for the background files, don't have any requirements for many categories nor for any additional cuts, they just get summed up anyways later (keep the file-induced categorization anyways)

	classifier -> FillHistogram(background_path[0], conf.lumi(), hist_vec[conf.hist_index("ZZ4lhist")], [&](Tree* in) -> int{return(mZZ_cut(in) && cut(in));});

	// aggregate all the gg -> 4l channels together
	classifier -> FillHistogram(background_path[1], conf.lumi(), hist_vec[conf.hist_index("gg4lhist")], [&](Tree* in) -> int{return(mZZ_cut(in) && cut(in));});
	classifier -> FillHistogram(background_path[2], conf.lumi(), hist_vec[conf.hist_index("gg4lhist")], [&](Tree* in) -> int{return(mZZ_cut(in) && cut(in));});
	classifier -> FillHistogram(background_path[3], conf.lumi(), hist_vec[conf.hist_index("gg4lhist")], [&](Tree* in) -> int{return(mZZ_cut(in) && cut(in));});
	classifier -> FillHistogram(background_path[4], conf.lumi(), hist_vec[conf.hist_index("gg4lhist")], [&](Tree* in) -> int{return(mZZ_cut(in) && cut(in));});
	classifier -> FillHistogram(background_path[5], conf.lumi(), hist_vec[conf.hist_index("gg4lhist")], [&](Tree* in) -> int{return(mZZ_cut(in) && cut(in));});
	classifier -> FillHistogram(background_path[6], conf.lumi(), hist_vec[conf.hist_index("gg4lhist")], [&](Tree* in) -> int{return(mZZ_cut(in) && cut(in));});

	std::cout << "end filling background histograms" << std::endl;

	save_histos(out_path + conf.storage_prefix() + hist_storage, hist_vec);	
    }    

    // read the histograms back from the root file
    hist_vec = read_histos(out_path + conf.storage_prefix() + hist_storage, conf.background_hist_names());

    std::cout << "end preparing background" << std::endl;

    return hist_vec;
}

void make_punzi(int fill_histos, Classifier* classifier, TString out_file, TString data_id, const std::function<bool(Tree*)>& cut, TString out_path, Config& conf)
{
    std::vector<TH1F*> signal_hist_vec = generate_signal_histvec(fill_histos, classifier, cut, data_id, out_path, conf);
    std::vector<TH1F*> background_hist_vec = generate_background_histvec(fill_histos, classifier, cut, data_id, out_path, conf);

    std::cout << "got all histograms" << std::endl;
   
    // the total number of events from each category
    float VBF_events = get_total_events(signal_hist_vec[conf.hist_index("VBFhist")]); // this is the sum of all VBF events (i.e. both VBF 1-jet and VBF 2-jet events)

    float VHhadr_events = get_total_events(signal_hist_vec[conf.hist_index("WHXhist")]);
    VHhadr_events += get_total_events(signal_hist_vec[conf.hist_index("ZHXhist")]);

    float VHlept_events = get_total_events(signal_hist_vec[conf.hist_index("WHlnuhist")]);
    VHlept_events += get_total_events(signal_hist_vec[conf.hist_index("ZH2lhist")]);

    float VHMET_events = get_total_events(signal_hist_vec[conf.hist_index("ZHnunuhist")]);

    float untagged_events = get_total_events(signal_hist_vec[conf.hist_index("ggHhist")]);

    std::cout << "got all total event counts" << std::endl;

    // the correctly classified events:
    // note that this routes 
    //      WH, W -> X    and ZH, Z -> X   into VHhadr
    //      WH, W -> l nu and ZH, Z -> 2 l into VHlept
    //      ggH                            into untagged
    //      VBF                            into VBF1jet and VBF2jet
    //      ttH + 0,1,2 l                  into ttH
    float VBF1j_correct_events = signal_hist_vec[conf.hist_index("VBFhist")] -> GetBinContent(conf.bin_index("VBF1jet_bin"));
    float VBF2j_correct_events = signal_hist_vec[conf.hist_index("VBFhist")] -> GetBinContent(conf.bin_index("VBF2jet_bin"));

    float VHlept_correct_events = signal_hist_vec[conf.hist_index("WHlnuhist")] -> GetBinContent(conf.bin_index("VHleptonic_bin"));
    VHlept_correct_events += signal_hist_vec[conf.hist_index("ZH2lhist")] -> GetBinContent(conf.bin_index("VHleptonic_bin"));

    float VHhadr_correct_events = signal_hist_vec[conf.hist_index("WHXhist")] -> GetBinContent(conf.bin_index("VHhadronic_bin"));
    VHhadr_correct_events += signal_hist_vec[conf.hist_index("ZHXhist")] -> GetBinContent(conf.bin_index("VHhadronic_bin"));

    float VHMET_correct_events = signal_hist_vec[conf.hist_index("ZHnunuhist")] -> GetBinContent(conf.bin_index("VHMET_bin"));

    float untagged_correct_events = signal_hist_vec[conf.hist_index("ggHhist")] -> GetBinContent(conf.bin_index("untagged_bin"));

    // generate the vector holding both signal- and background histograms: this makes sure to get the *total* background in each category: the unwanted signal contributions + the actual background
    std::vector<TH1F*> hist_vec(signal_hist_vec);
    hist_vec.insert(hist_vec.end(), background_hist_vec.begin(), background_hist_vec.end());
    std::vector<float> sums = get_category_sums(hist_vec);

    float untagged_punzi = get_punzi_purity(
	sums[conf.bin_index("untagged_bin") - 1] - untagged_correct_events, // background in this category
	untagged_correct_events, // signal in this category
	untagged_events, // total signal (important for computing the efficiency)
	5, 2);

    float VBF1j_punzi = get_punzi_purity(
	sums[conf.bin_index("VBF1jet_bin") - 1] - VBF1j_correct_events,
	VBF1j_correct_events,
	VBF_events,
	5, 2);

    float VBF2j_punzi = get_punzi_purity(
	sums[conf.bin_index("VBF2jet_bin") - 1] - VBF2j_correct_events,
	VBF2j_correct_events,
	VBF_events,
	5, 2);

    float VHhadr_punzi = get_punzi_purity(
	sums[conf.bin_index("VHhadronic_bin") - 1] - VHhadr_correct_events,
	VHhadr_correct_events,
	VHhadr_events,
	5, 2);

    float VHlept_punzi = get_punzi_purity(
	sums[conf.bin_index("VHleptonic_bin") - 1] - VHlept_correct_events,
	VHlept_correct_events,
	VHlept_events,
	5, 2);

    float VHMET_punzi = get_punzi_purity(
	sums[conf.bin_index("VHMET_bin") - 1] - VHMET_correct_events,
	VHMET_correct_events,
	VHMET_events,
	5, 2);
    
    // make the plot of the punzi purity
    std::vector<TH1F*> punzi_hist_vec = {new TH1F("punzi_purity", "punzi_purity", conf.cat_labels().size(), -0.5, conf.cat_labels().size() - 0.5)};

    punzi_hist_vec[0] -> Fill(conf.bin_index("untagged_bin") - 1, untagged_punzi);
    punzi_hist_vec[0] -> Fill(conf.bin_index("VBF1jet_bin") - 1, VBF1j_punzi);
    punzi_hist_vec[0] -> Fill(conf.bin_index("VBF2jet_bin") - 1, VBF2j_punzi);
    punzi_hist_vec[0] -> Fill(conf.bin_index("VHhadronic_bin") - 1, VHhadr_punzi);
    punzi_hist_vec[0] -> Fill(conf.bin_index("VHleptonic_bin") - 1, VHlept_punzi);
    punzi_hist_vec[0] -> Fill(conf.bin_index("VHMET_bin") - 1, VHMET_punzi);

    // now handle the few differences between 2017 and 2018
    if(conf.id() == "Mor17")
    {
	float ttH_events = get_total_events(signal_hist_vec[conf.hist_index("ttH0lhist")]);
	ttH_events += get_total_events(signal_hist_vec[conf.hist_index("ttH1lhist")]);
	ttH_events += get_total_events(signal_hist_vec[conf.hist_index("ttH2lhist")]);

	float ttH_correct_events = signal_hist_vec[conf.hist_index("ttH0lhist")] -> GetBinContent(conf.bin_index("ttH_bin"));
	ttH_correct_events += signal_hist_vec[conf.hist_index("ttH1lhist")] -> GetBinContent(conf.bin_index("ttH_bin"));
	ttH_correct_events += signal_hist_vec[conf.hist_index("ttH2lhist")] -> GetBinContent(conf.bin_index("ttH_bin"));

	float ttH_punzi = get_punzi_purity(
	    sums[conf.bin_index("ttH_bin") - 1] - ttH_correct_events,
	    ttH_correct_events,
	    ttH_events,
	    5, 2);

	punzi_hist_vec[0] -> Fill(conf.bin_index("ttH_bin") - 1, ttH_punzi);
    }
    else if(conf.id() == "Mor18")
    {
	float ttH_hadronic_events = get_total_events(signal_hist_vec[conf.hist_index("ttH0lhist")]);
	float ttH_leptonic_events = get_total_events(signal_hist_vec[conf.hist_index("ttH1lhist")]);
	ttH_leptonic_events += get_total_events(signal_hist_vec[conf.hist_index("ttH2lhist")]);

	float ttH_hadronic_correct_events = signal_hist_vec[conf.hist_index("ttH0lhist")] -> GetBinContent(conf.bin_index("ttH_hadronic_bin"));
	float ttH_leptonic_correct_events = signal_hist_vec[conf.hist_index("ttH1lhist")] -> GetBinContent(conf.bin_index("ttH_leptonic_bin"));
	ttH_leptonic_correct_events += signal_hist_vec[conf.hist_index("ttH2lhist")] -> GetBinContent(conf.bin_index("ttH_leptonic_bin"));

	float ttH_leptonic_punzi = get_punzi_purity(
	    sums[conf.bin_index("ttH_leptonic_bin") - 1] - ttH_leptonic_correct_events,
	    ttH_leptonic_correct_events,
	    ttH_leptonic_events,
	    5, 2);

	float ttH_hadronic_punzi = get_punzi_purity(
	    sums[conf.bin_index("ttH_hadronic_bin") - 1] - ttH_hadronic_correct_events,
	    ttH_hadronic_correct_events,
	    ttH_hadronic_events,
	    5, 2);

	punzi_hist_vec[0] -> Fill(conf.bin_index("ttH_leptonic_bin") - 1, ttH_leptonic_punzi);
	punzi_hist_vec[0] -> Fill(conf.bin_index("ttH_hadronic_bin") - 1, ttH_hadronic_punzi);
    }

    save_histos(out_path + conf.storage_prefix() + "punzi_plot_hist" + ".root", punzi_hist_vec);

    CatPlotter plotter;
    plotter.Construct(punzi_hist_vec, conf.cat_labels(), std::vector<TString>(), std::vector<float>(), "Punzi purity", conf.lumi());   
    plotter.SaveAs(out_path + conf.storage_prefix() + out_file + ".pdf");
}
