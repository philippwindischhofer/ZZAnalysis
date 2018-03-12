#include <ZZAnalysis/AnalysisStep/test/Optimizer/include/opt_utils.h>

// generates a vector of histograms corresponding to the signals
std::vector<TH1F*> generate_signal_histvec(int fill_histos, Classifier* classifier, const std::function<bool(Tree*)>& cut, TString data_id, TString in_path, TString out_path)
{
    TString hist_storage(data_id + "_signal.root");
    std::vector<TString> signal_path;

    for(unsigned int i = 0; i < signal_file_names.size(); i++)
    {
	signal_path.push_back(in_path + signal_file_names[i] + file_name);
    }
	
    std::vector<TH1F*> hist_vec(signal_hist_names.size());

    for(unsigned int i = 0; i < signal_hist_names.size(); i++)
    {
	hist_vec[i] = new TH1F(signal_hist_names[i], signal_hist_names[i], cat_labels.size(), -0.5, cat_labels.size() - 0.5);
	hist_vec[i] -> SetFillColor(signal_source_colors[i]);
	hist_vec[i] -> SetLineColor(signal_source_colors[i]);
	hist_vec[i] -> SetFillStyle(1001);
    }    

    if(fill_histos)
    {	
	std::cout << "filling histograms" << std::endl;

	// for the signal files, can be very specific and select precisely the correct differential channel
	// ideally, create one separate histogram (by using generator-level cuts) for each signal category. Then, can compute the efficiency for this signal to end up in the matching signal category
	classifier -> FillHistogram(signal_path[0], lumi, hist_vec[ggHhist], [&](Tree* in) -> bool{return(mZZ_cut(in) && cut(in));});
	classifier -> FillHistogram(signal_path[1], lumi, hist_vec[VBFhist], [&](Tree* in) -> bool{return(mZZ_cut(in) && cut(in));});
	classifier -> FillHistogram(signal_path[2], lumi, hist_vec[WHXhist], [&](Tree* in) -> bool{return(extraLeptons_0_cut(in) && cut(in));});
	classifier -> FillHistogram(signal_path[2], lumi, hist_vec[WHlnuhist], [&](Tree* in) -> bool{return(extraLeptons_1_cut(in) && cut(in));}); // W decays leptonically
	classifier -> FillHistogram(signal_path[3], lumi, hist_vec[WHXhist], [&](Tree* in) -> bool{return(extraLeptons_0_cut(in) && cut(in));});
	classifier -> FillHistogram(signal_path[3], lumi, hist_vec[WHlnuhist], [&](Tree* in) -> bool{return(extraLeptons_1_cut(in) && cut(in));}); // W decays leptonically
	classifier -> FillHistogram(signal_path[4], lumi, hist_vec[ZHXhist], [&](Tree* in) -> bool{return(extraNeutrinos_0_Leptons_0_cut(in) && cut(in));}); // no extra leptons (electrons, muons) and neutrinos for the hadronic decay of the Z
	classifier -> FillHistogram(signal_path[4], lumi, hist_vec[ZHnunuhist], [&](Tree* in) -> bool{return(extraNeutrinos_2_cut(in) && cut(in));});
	classifier -> FillHistogram(signal_path[4], lumi, hist_vec[ZH2lhist], [&](Tree* in) -> bool{return(extraLeptons_2_cut(in) && cut(in));}); // need exactly two extra leptons to have the Z decaying leptonically
	classifier -> FillHistogram(signal_path[5], lumi, hist_vec[ttH0lhist], [&](Tree* in) -> bool{return(extraLeptons_0_cut(in) && cut(in));});    
	classifier -> FillHistogram(signal_path[5], lumi, hist_vec[ttH1lhist], [&](Tree* in) -> bool{return(extraLeptons_1_cut(in) && cut(in));});
	classifier -> FillHistogram(signal_path[5], lumi, hist_vec[ttH2lhist], [&](Tree* in) -> bool{return(extraLeptons_2_cut(in) && cut(in));});    
	std::cout << "end filling histograms" << std::endl;

	save_histos(out_path + STORAGE_PREFIX + hist_storage, hist_vec);
    }    

    // read the histograms back from the root file
    hist_vec = read_histos(out_path + STORAGE_PREFIX + hist_storage, signal_hist_names);

    return hist_vec;
}

std::vector<TH1F*> generate_background_histvec(int fill_histos, Classifier* classifier, const std::function<bool(Tree*)>& cut, TString data_id, TString in_path, TString out_path)
{
    TString hist_storage(data_id + "_background.root");
    std::vector<TString> background_path;

    for(unsigned int i = 0; i < background_file_names.size(); i++)
    {
	background_path.push_back(in_path + background_file_names[i] + file_name);
    }

    std::vector<TH1F*> hist_vec(background_hist_names.size());

    for(unsigned int i = 0; i < background_hist_names.size(); i++)
    {
	hist_vec[i] = new TH1F(background_hist_names[i], background_hist_names[i], cat_labels.size(), -0.5, cat_labels.size() - 0.5);
	hist_vec[i] -> SetFillStyle(3244);
	hist_vec[i] -> SetLineColor(background_source_colors[i]);
	hist_vec[i] -> SetFillColor(background_source_colors[i]);
    }

    if(fill_histos)
    {	
	// No need to fill the histograms every time!
	std::cout << "filling background histograms" << std::endl;
	
	// for the background files, don't have any requirements for many categories nor for any additional cuts, they just get summed up anyways later (keep the file-induced categorization anyways)

	classifier -> FillHistogram(background_path[0], lumi, hist_vec[ZZ4lhist], [&](Tree* in) -> int{return(mZZ_cut(in) && cut(in));});
	//classifier -> FillHistogram(background_path[1], lumi, hist_vec[DYhist], [&](Tree* in) -> int{return(mZZ_cut(in) && cut(in));});
	//classifier -> FillHistogram(background_path[2], lumi, hist_vec[TThist], [&](Tree* in) -> int{return(mZZ_cut(in) && cut(in));});

	// aggregate all the gg -> 4l channels together
	classifier -> FillHistogram(background_path[3], lumi, hist_vec[gg4lhist], [&](Tree* in) -> int{return(mZZ_cut(in) && cut(in));});
	classifier -> FillHistogram(background_path[4], lumi, hist_vec[gg4lhist], [&](Tree* in) -> int{return(mZZ_cut(in) && cut(in));});
	classifier -> FillHistogram(background_path[5], lumi, hist_vec[gg4lhist], [&](Tree* in) -> int{return(mZZ_cut(in) && cut(in));});
	classifier -> FillHistogram(background_path[6], lumi, hist_vec[gg4lhist], [&](Tree* in) -> int{return(mZZ_cut(in) && cut(in));});
	classifier -> FillHistogram(background_path[7], lumi, hist_vec[gg4lhist], [&](Tree* in) -> int{return(mZZ_cut(in) && cut(in));});
	classifier -> FillHistogram(background_path[8], lumi, hist_vec[gg4lhist], [&](Tree* in) -> int{return(mZZ_cut(in) && cut(in));});

	std::cout << "end filling background histograms" << std::endl;

	save_histos(out_path + STORAGE_PREFIX + hist_storage, hist_vec);	
    }    

    // read the histograms back from the root file
    hist_vec = read_histos(out_path + STORAGE_PREFIX + hist_storage, background_hist_names);

    return hist_vec;
}

void make_punzi(int fill_histos, Classifier* classifier, TString out_file, TString data_id, const std::function<bool(Tree*)>& cut, TString in_path, TString out_path)
{
    std::vector<TH1F*> signal_hist_vec = generate_signal_histvec(fill_histos, classifier, cut, data_id, in_path, out_path);
    std::vector<TH1F*> background_hist_vec = generate_background_histvec(fill_histos, classifier, cut, data_id, in_path, out_path);
   
    // the total number of events from each category
    float VBF_events = get_total_events(signal_hist_vec[VBFhist]); // this is the sum of all VBF events (i.e. both VBF 1-jet and VBF 2-jet events)

    float VHhadr_events = get_total_events(signal_hist_vec[WHXhist]);
    VHhadr_events += get_total_events(signal_hist_vec[ZHXhist]);

    float VHlept_events = get_total_events(signal_hist_vec[WHlnuhist]);
    VHlept_events += get_total_events(signal_hist_vec[ZH2lhist]);

#ifdef Mor17
    float ttH_events = get_total_events(signal_hist_vec[ttH0lhist]);
    ttH_events += get_total_events(signal_hist_vec[ttH1lhist]);
    ttH_events += get_total_events(signal_hist_vec[ttH2lhist]);
#endif

#ifdef Mor18
    float ttH_hadronic_events = get_total_events(signal_hist_vec[ttH0lhist]);
    float ttH_leptonic_events = get_total_events(signal_hist_vec[ttH1lhist]);
    ttH_leptonic_events += get_total_events(signal_hist_vec[ttH2lhist]);
#endif

    float VHMET_events = get_total_events(signal_hist_vec[ZHnunuhist]);

    float untagged_events = get_total_events(signal_hist_vec[ggHhist]);

    // the correctly classified events:
    // note that this routes 
    //      WH, W -> X    and ZH, Z -> X   into VHhadr
    //      WH, W -> l nu and ZH, Z -> 2 l into VHlept
    //      ggH                            into untagged
    //      VBF                            into VBF1jet and VBF2jet
    //      ttH + 0,1,2 l                  into ttH
    float VBF1j_correct_events = signal_hist_vec[VBFhist] -> GetBinContent(VBF1jet_bin);
    float VBF2j_correct_events = signal_hist_vec[VBFhist] -> GetBinContent(VBF2jet_bin);

    float VHlept_correct_events = signal_hist_vec[WHlnuhist] -> GetBinContent(VHleptonic_bin);
    VHlept_correct_events += signal_hist_vec[ZH2lhist] -> GetBinContent(VHleptonic_bin);

    float VHhadr_correct_events = signal_hist_vec[WHXhist] -> GetBinContent(VHhadronic_bin);
    VHhadr_correct_events += signal_hist_vec[ZHXhist] -> GetBinContent(VHhadronic_bin);

#ifdef Mor17
    float ttH_correct_events = signal_hist_vec[ttH0lhist] -> GetBinContent(ttH_bin);
    ttH_correct_events += signal_hist_vec[ttH1lhist] -> GetBinContent(ttH_bin);
    ttH_correct_events += signal_hist_vec[ttH2lhist] -> GetBinContent(ttH_bin);
#endif

#ifdef Mor18
    float ttH_hadronic_correct_events = signal_hist_vec[ttH0lhist] -> GetBinContent(ttH_hadronic_bin);
    float ttH_leptonic_correct_events = signal_hist_vec[ttH1lhist] -> GetBinContent(ttH_leptonic_bin);
    ttH_leptonic_correct_events += signal_hist_vec[ttH2lhist] -> GetBinContent(ttH_leptonic_bin);
#endif

    float VHMET_correct_events = signal_hist_vec[ZHnunuhist] -> GetBinContent(VHMET_bin);

    float untagged_correct_events = signal_hist_vec[ggHhist] -> GetBinContent(untagged_bin);

    // generate the vector holding both signal- and background histograms: this makes sure to get the *total* background in each category: the unwanted signal contributions + the actual background
    std::vector<TH1F*> hist_vec(signal_hist_vec);
    hist_vec.insert(hist_vec.end(), background_hist_vec.begin(), background_hist_vec.end());
    std::vector<float> sums = get_category_sums(hist_vec);

    float untagged_punzi = get_punzi_purity(
	sums[untagged_bin - 1] - untagged_correct_events, // background in this category
	untagged_correct_events, // signal in this category
	untagged_events, // total signal (important for computing the efficiency)
	5, 2);

    float VBF1j_punzi = get_punzi_purity(
	sums[VBF1jet_bin - 1] - VBF1j_correct_events,
	VBF1j_correct_events,
	VBF_events,
	5, 2);

    float VBF2j_punzi = get_punzi_purity(
	sums[VBF2jet_bin - 1] - VBF2j_correct_events,
	VBF2j_correct_events,
	VBF_events,
	5, 2);

#ifdef Mor17
    float ttH_punzi = get_punzi_purity(
	sums[ttH_bin - 1] - ttH_correct_events,
	ttH_correct_events,
	ttH_events,
	5, 2);
#endif

#ifdef Mor18
    float ttH_leptonic_punzi = get_punzi_purity(
	sums[ttH_leptonic_bin - 1] - ttH_leptonic_correct_events,
	ttH_leptonic_correct_events,
	ttH_leptonic_events,
	5, 2);

    float ttH_hadronic_punzi = get_punzi_purity(
	sums[ttH_hadronic_bin - 1] - ttH_hadronic_correct_events,
	ttH_hadronic_correct_events,
	ttH_hadronic_events,
	5, 2);
#endif

    float VHhadr_punzi = get_punzi_purity(
	sums[VHhadronic_bin - 1] - VHhadr_correct_events,
	VHhadr_correct_events,
	VHhadr_events,
	5, 2);

    float VHlept_punzi = get_punzi_purity(
	sums[VHleptonic_bin - 1] - VHlept_correct_events,
	VHlept_correct_events,
	VHlept_events,
	5, 2);

    float VHMET_punzi = get_punzi_purity(
	sums[VHMET_bin - 1] - VHMET_correct_events,
	VHMET_correct_events,
	VHMET_events,
	5, 2);
    
    // make the plot of the punzi purity
    std::vector<TH1F*> punzi_hist_vec = {new TH1F("punzi_purity", "punzi_purity", cat_labels.size(), -0.5, cat_labels.size() - 0.5)};

    punzi_hist_vec[0] -> Fill(untagged_bin - 1, untagged_punzi);
    punzi_hist_vec[0] -> Fill(VBF1jet_bin - 1, VBF1j_punzi);
    punzi_hist_vec[0] -> Fill(VBF2jet_bin - 1, VBF2j_punzi);
    punzi_hist_vec[0] -> Fill(VHhadronic_bin - 1, VHhadr_punzi);
    punzi_hist_vec[0] -> Fill(VHleptonic_bin - 1, VHlept_punzi);
    punzi_hist_vec[0] -> Fill(VHMET_bin - 1, VHMET_punzi);
    
#ifdef Mor17
    punzi_hist_vec[0] -> Fill(ttH_bin - 1, ttH_punzi);
#endif

#ifdef Mor18
    punzi_hist_vec[0] -> Fill(ttH_leptonic_bin - 1, ttH_leptonic_punzi);
    punzi_hist_vec[0] -> Fill(ttH_hadronic_bin - 1, ttH_hadronic_punzi);
#endif

    save_histos(out_path + STORAGE_PREFIX + "punzi_plot_hist" + ".root", punzi_hist_vec);

    CatPlotter plotter;
    plotter.Construct(punzi_hist_vec, cat_labels, std::vector<TString>(), std::vector<float>(), "Punzi purity", lumi);   
    plotter.SaveAs(out_folder + STORAGE_PREFIX + out_file + ".pdf");
}
