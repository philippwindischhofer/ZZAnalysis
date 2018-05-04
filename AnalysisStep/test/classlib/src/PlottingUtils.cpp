#include <ZZAnalysis/AnalysisStep/test/classlib/include/PlottingUtils.h>

std::map<TString, TH1F*> PlottingUtils::generate_classifier_histmap(int fill_histos, Classifier* classifier, Config* conf, const std::function<bool(Tree*)>& ext_cut, TString data_id, TString out_path, float start_fraction, float end_fraction, bool fast_reweighting)
{
    TString hist_storage = data_id + "_signal.root";
    
    Profiler* prof = new Profiler();

    int n_cat = conf -> number_categories();
    std::map<TString, TH1F*> histmap = conf -> generate_empty_histmap(n_cat, -0.5, n_cat - 0.5);

    if(fill_histos)
    {
	std::cout << "filling histograms" << std::endl;

	// iterate over the routing specified in the configuration
	std::vector<std::pair<TString, Routing*>> routings = conf -> get_routing();

	for(auto& cur: routings)
	{
	    TString file_name = cur.first;
	    TString file_path = conf -> MC_path() + file_name + conf -> MC_filename();

	    Routing* routing = cur.second;
	    
	    // tie together the total cut to be used
	    auto cut = [&](Tree* in) -> bool {
		return (ext_cut(in) && 
			//mZZ_cut(in) &&
			(routing -> cut)(in)) ?
		kTRUE : kFALSE;
	    };

	    auto var = [&](Tree* in) -> float {
		return classifier -> ClassifyThisEvent(in);
	    };
	    
	    TH1F* fill_counts = prof -> FillProfile(file_path, conf -> lumi(), histmap[routing -> hist_name], cut, var, false, start_fraction, end_fraction, fast_reweighting);

	    // give some statistics output
	    std::cout << "-----------------------------------------" << std::endl;
	    std::cout << "statistics output for: " << routing -> hist_name << std::endl;

	    for(auto& assignment: conf -> bin_assignment())
	    {
		std::cout << "bin : " << assignment.first << " (" << assignment.second << "): " << fill_counts -> GetBinContent(assignment.second + 1) << " fills, " << histmap[routing -> hist_name] -> GetBinContent(assignment.second + 1) << " events" << std::endl;
	    }

	    std::cout << "-----------------------------------------" << std::endl;
	}

	std::cout << "end filling histograms" << std::endl;

	save_histmap(out_path + conf -> storage_prefix() + hist_storage, histmap);
    }

    histmap = read_histmap(out_path + conf -> storage_prefix() + hist_storage, conf -> hist_names());

    return histmap;
}

std::vector<TH1F*> PlottingUtils::generate_classifier_histvec(int fill_histos, Classifier* classifier, Config* conf, const std::function<bool(Tree*)>& ext_cut, TString data_id, TString out_path, bool fast_reweighting)
{
    std::map<TString, TH1F*> histmap = generate_classifier_histmap(fill_histos, classifier, conf, ext_cut, data_id, out_path, fast_reweighting);

    std::vector<TH1F*> histvec;

    for(auto& cur: histmap)
    {
	histvec.push_back(cur.second);
    }

    return histvec;
}

void PlottingUtils::make_barchart(std::map<TString, TH1F*> histmap, TString out_folder, TString out_file, TString label, Config* conf)
{
    CatPlotter plotter;

    plotter.Construct(histmap, conf, std::vector<float>(), "exp. events");
    plotter.DrawLabel(label);
    //plotter.SaveAs(out_folder + conf -> storage_prefix() + out_file + "_absolute.pdf");
    plotter.SaveFiles(out_folder + conf -> storage_prefix() + out_file + "_absolute");

    std::vector<float> sums_SB = unitize_histmap(histmap);    
    
    plotter.Construct(histmap, conf, sums_SB, "signal fraction");        
    plotter.DrawLabel(label);
    //plotter.SaveAs(out_folder + conf -> storage_prefix() + out_file + ".pdf");
    plotter.SaveFiles(out_folder + conf -> storage_prefix() + out_file);
}

void PlottingUtils::make_S_barchart(int fill_histos, Classifier* classifier, TString out_folder, TString out_file, TString data_id, TString label, const std::function<bool(Tree*)>& cut, Config* conf, float start_fraction, float end_fraction, bool fast_reweighting)
{
    // get first the total histmap (holding signal and background)
    std::map<TString, TH1F*> histmap = generate_classifier_histmap(fill_histos, classifier, conf, cut, data_id, out_folder, start_fraction, end_fraction, fast_reweighting);
    
    // but then, only forward the part that actually corresponds to the signal
    std::map<TString, TH1F*> sig_histmap;
    std::vector<TString> backgrounds = conf -> signal_hist_names();

    for(auto& cur: histmap)
    {
	if(std::find(backgrounds.begin(), backgrounds.end(), cur.first) != backgrounds.end())
	{
	    std::cout << "getting signal only" << cur.first << std::endl;
	    sig_histmap.insert(cur);
	}
    }

    make_barchart(sig_histmap, out_folder, out_file, label, conf);
}

void PlottingUtils::make_SB_barchart(int fill_histos, Classifier* classifier, TString out_folder, TString out_file, TString data_id, TString label, const std::function<bool(Tree*)>& cut, Config* conf, float start_fraction, float end_fraction, bool fast_reweighting)
{
    std::map<TString, TH1F*> histmap = generate_classifier_histmap(fill_histos, classifier, conf, cut, data_id, out_folder, start_fraction, end_fraction, fast_reweighting);

    make_barchart(histmap, out_folder, out_file, label, conf);
}

std::map<TString, float> PlottingUtils::make_correct_events(int fill_histos, Classifier* classifier, TString out_folder, TString out_file, TString data_id, const std::function<bool(Tree*)>& cut, Config* conf, float start_fraction, float end_fraction, bool fast_reweighting)
{
    std::map<TString, TH1F*> histmap = generate_classifier_histmap(fill_histos, classifier, conf, cut, data_id, out_folder, start_fraction, end_fraction, fast_reweighting);
    std::vector<SignalAssignment*> assignments = conf -> signal_assignment();
    
    // compute the number of correctly classified events, for each (abstract) category
    std::map<TString, float> correct_events;

    for(auto& assignment: assignments)
    {
	std::cout << "abs_cat = " << assignment -> category << ", hist = " << assignment -> histogram << ", bin number = " << conf -> bin_number(assignment -> bin) << std::endl;
	correct_events[assignment -> category] += (histmap[assignment -> histogram] -> GetBinContent(
						       conf -> bin_number(assignment -> bin)));
    }

    return correct_events;
}

std::map<TString, float> PlottingUtils::make_total_events(int fill_histos, Classifier* classifier, TString out_folder, TString out_file, TString data_id, const std::function<bool(Tree*)>& cut, Config* conf, float start_fraction, float end_fraction, bool fast_reweighting)
{
    std::map<TString, TH1F*> histmap = generate_classifier_histmap(fill_histos, classifier, conf, cut, data_id, out_folder, start_fraction, end_fraction, fast_reweighting);
    
    // compute the total number of events in each (abstract) category [that need not necessarily coincide with the categorization output of the classifier!]
    std::map<TString, float> total_events;
    std::map<int, float> bin_sums = get_bin_sums(histmap, conf);
    std::vector<std::pair<TString, int>> bin_assignments = conf -> bin_assignment();
    
    for(auto& assignment: bin_assignments)
    {
	std::cout << "bin assignment: " << assignment.first << " / " << assignment.second << " / " << bin_sums[assignment.second] << " events " << std::endl;
	total_events[assignment.first] += bin_sums[assignment.second];
    }

    return total_events;
}

std::map<TString, float> PlottingUtils::make_desired_events(int fill_histos, Classifier* classifier, TString out_folder, TString out_file, TString data_id, const std::function<bool(Tree*)>& cut, Config* conf, float start_fraction, float end_fraction, bool fast_reweighting)
{
    std::map<TString, TH1F*> histmap = generate_classifier_histmap(fill_histos, classifier, conf, cut, data_id, out_folder, start_fraction, end_fraction, fast_reweighting);
    
    // compute the total number of events that, in the ideal case, should have ended up in each abstract category
    std::map<TString, float> desired_events;
    std::vector<std::pair<TString, TString>> histo_assignments = conf -> histogram_assignment();
    
    for(auto& assignment: histo_assignments)
    {
	std::cout << "histo assignment: " << assignment.first << " / " << assignment.second << " / " << get_total_events(histmap[assignment.second]) << " events " << std::endl;
	desired_events[assignment.first] += get_total_events(histmap[assignment.second]);
    }

    return desired_events;
}

// is more refined and really takes ONLY those events as signals, whose production mode also agrees with the category
void PlottingUtils::make_SBfine_ratio(int fill_histos, Classifier* classifier, TString out_folder, TString out_file, TString data_id, const std::function<bool(Tree*)>& cut, Config* conf, float start_fraction, float end_fraction, bool fast_reweighting)
{
    auto correct_events = make_correct_events(fill_histos, classifier, out_folder, out_file, data_id, cut, conf, fast_reweighting);
    auto total_events = make_total_events(kFALSE, classifier, out_folder, out_file, data_id, cut, conf, fast_reweighting);
    
    // now compute S / (S + B) for each abstract category
    std::vector<std::pair<TString, float>> SB;

    for(auto& abs_cat: conf -> abstract_categories())
    {
	std::cout << abs_cat << ": " << correct_events[abs_cat] / total_events[abs_cat] << std::endl;
	SB.push_back(std::make_pair(abs_cat, correct_events[abs_cat] / total_events[abs_cat]));
    }

    // sort it before filling the histogram
    std::sort(SB.begin(), SB.end(), 
	      [&](std::pair<TString, float>&a, std::pair<TString, float>&b)
	      {return conf -> abs_cat_number(a.first) < conf -> abs_cat_number(b.first);});

    // build the histogram
    std::vector<TString> cat_labels;
    int n_abstract_categories = conf -> number_abstract_categories();
    std::vector<TH1F*> SB_hist_vec = {new TH1F("SB_fine", "SB_fine", n_abstract_categories, -0.5, n_abstract_categories - 0.5)};

    int abstract_bin = 0;
    for(auto& SB_val: SB)
    {
	std::cout << SB_val.second << std::endl;
	SB_hist_vec[0] -> Fill(abstract_bin, SB_val.second);
	cat_labels.push_back(conf -> abs_cat_label(SB_val.first));
	abstract_bin++;
    }

    CatPlotter plotter;

    plotter.Construct(SB_hist_vec, cat_labels, std::vector<TString>(), std::vector<float>(), "S / (S + B)", conf -> lumi());       
    plotter.GetStack() -> SetMaximum(1.0);
    plotter.Update();
    plotter.SaveAs(out_folder + conf -> storage_prefix() + out_file + ".pdf");
}

void PlottingUtils::make_punzi(int fill_histos, Classifier* classifier, TString out_folder, TString out_file, TString data_id, const std::function<bool(Tree*)>& cut, Config* conf, float start_fraction, float end_fraction, bool fast_reweighting)
{
    auto correct_events = make_correct_events(fill_histos, classifier, out_folder, out_file, data_id, cut, conf, start_fraction, end_fraction, fast_reweighting);
    auto total_events = make_total_events(kFALSE, classifier, out_folder, out_file, data_id, cut, conf, start_fraction, end_fraction, fast_reweighting);
    auto desired_events = make_desired_events(kFALSE, classifier, out_folder, out_file, data_id, cut, conf, start_fraction, end_fraction, fast_reweighting);

    // now compute punzi for each abstract category
    std::vector<std::pair<TString, float>> punzi;
    
    for(auto& abs_cat: conf -> abstract_categories())
    {
	float total = total_events[abs_cat];
	float correct = correct_events[abs_cat];
	float desired = desired_events[abs_cat];

	punzi.push_back(std::make_pair(abs_cat, 
				       get_punzi_purity(total - correct,
							correct,
							desired,
							5, 2)));

    }

    // sort them before filling the histogram
    std::sort(punzi.begin(), punzi.end(), 
	      [&](std::pair<TString, float>&a, std::pair<TString, float>&b)
	      {return conf -> abs_cat_number(a.first) < conf -> abs_cat_number(b.first);});

    // build the histogram
    std::vector<TString> cat_labels;
    int n_abstract_categories = conf -> number_abstract_categories();
    std::vector<TH1F*> punzi_hist_vec = {new TH1F("punzi_purity", "punzi_purity", n_abstract_categories, -0.5, n_abstract_categories - 0.5)};

    int abstract_bin = 0;
    for(auto& punzi_val: punzi)
    {
	std::cout << punzi_val.second << std::endl;
	punzi_hist_vec[0] -> Fill(abstract_bin, punzi_val.second);
	cat_labels.push_back(conf -> abs_cat_label(punzi_val.first));
	abstract_bin++;
    }

    save_histos(out_folder + conf -> storage_prefix() + out_file + "_plot_hist" + ".root", punzi_hist_vec);

    CatPlotter plotter;

    plotter.Construct(punzi_hist_vec, cat_labels, std::vector<TString>(), std::vector<float>(), "Punzi purity", conf -> lumi());       
    plotter.Update();
    //plotter.SaveAs(out_folder + conf -> storage_prefix() + out_file + ".pdf");    
    plotter.SaveFiles(out_folder + conf -> storage_prefix() + out_file);    
}
