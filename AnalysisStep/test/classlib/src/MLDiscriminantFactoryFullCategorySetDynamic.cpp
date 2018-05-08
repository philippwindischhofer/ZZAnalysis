#include <ZZAnalysis/AnalysisStep/test/classlib/include/MLDiscriminantFactoryFullCategorySetDynamic.h>

DiscriminantCollection* MLDiscriminantFactoryFullCategorySetDynamic::GenerateDiscriminantCollection(TString calibration_dir, TString config_path, Config& conf)
{
    // priors for the categories (can be arbitrarily scaled, only the ratio is going to be needed)
    float VBF_prior = 1.0;
    float ggH_prior = 1.0;
    float WHhadr_prior = 1.0;
    float ZHhadr_prior = 1.0;
    float WHlept_prior = 1.0;
    float ZHlept_prior = 1.0;
    float ZHMET_prior = 1.0;
    float ttHhadr_prior = 1.0;
    float ttHlept_prior = 1.0;
    float bkg_prior = 1.0;

    return GenerateDiscriminantCollection(calibration_dir, config_path, conf, VBF_prior, ggH_prior, WHhadr_prior, ZHhadr_prior, WHlept_prior, ZHlept_prior, ZHMET_prior, ttHhadr_prior, ttHlept_prior, bkg_prior);

}

DiscriminantCollection* MLDiscriminantFactoryFullCategorySetDynamic::GenerateDiscriminantCollection(TString calibration_dir, TString config_path, Config& conf, float VBF_prior, float ggH_prior, float WHhadr_prior, float ZHhadr_prior, float WHlept_prior, float ZHlept_prior, float ZHMET_prior, float ttHhadr_prior, float ttHlept_prior, float bkg_prior)
{
    std::map<TString, float> priors;

    priors["VBF"] = VBF_prior;
    priors["ggH"] = ggH_prior;
    priors["WHh"] = WHhadr_prior;
    priors["ZHh"] = ZHhadr_prior;
    priors["WHl"] = WHlept_prior;
    priors["ZHl"] = ZHlept_prior;
    priors["ZHMET"] = ZHMET_prior;
    priors["ttHh"] = ttHhadr_prior;
    priors["ttHl"] = ttHlept_prior;
    priors["bkg"] = bkg_prior;

    return GenerateDiscriminantCollection(calibration_dir, config_path, conf, priors);
}

// changes the path to MC files, keeps the file / directory structure the same, but changes the overall location
TString MLDiscriminantFactoryFullCategorySetDynamic::ChangePath(TString original, TString new_head)
{
    std::vector<std::string> components = ConfigFileUtils::Split(original.Data(), "/");
    return new_head + components.end()[-2] + "/" + components.end()[-1];
}

DiscriminantCollection* MLDiscriminantFactoryFullCategorySetDynamic::GenerateDiscriminantCollection(TString calibration_dir, TString config_path, Config& conf, std::map<TString, float> priors)
{
    DiscriminantCollection* coll = new DiscriminantCollection();
    char model_prefix = '[';
    char preprocessor_prefix = '<';

    // open the configuration file that was passed
    ConfigFileHandler* confhandler = new ConfigFileHandler(config_path, "read");

    std::vector<TString> sections = confhandler -> GetSections();

    std::cout << "starting to parse configuration file: " << config_path << std::endl;

    for(auto& section: sections)
    {
	// in the first step, go only for model collections
	if(!section.BeginsWith(model_prefix) && !section.BeginsWith(preprocessor_prefix))
	{
	    Discriminant* disc = new Discriminant(calibration_dir);
	    
	    TString H1_name = confhandler -> GetField(section, "H1_name");
	    TString H0_name = confhandler -> GetField(section, "H0_name");
	    
	    std::cout << "--------------------------------------------------" << std::endl;
	    std::cout << "model collection = " << section << std::endl;
	    std::cout << "H1_name = " << H1_name << std::endl;
	    std::cout << "H0_name = " << H0_name << std::endl;

	    std::map<TString, TString> H1_stream_def = ConfigFileUtils::ParsePythonDict(confhandler -> GetField(section, "H1_stream_unmixed").Data());
	    std::map<TString, TString> H0_stream_def = ConfigFileUtils::ParsePythonDict(confhandler -> GetField(section, "H0_stream_unmixed").Data());

	    EventStream* H1Stream = new EventStream();
	    for(auto& entry: H1_stream_def)
	    {
		auto cut = ConfigFileUtils::ParsePythonCut(entry.second.Data());

		TString path = ChangePath(entry.first, conf.MC_path());
		std::cout << path << " <-> " << entry.second << std::endl;
		
		H1Stream -> AddEventSource(path, cut);
	    }

	    EventStream* H0Stream = new EventStream();
	    for(auto& entry: H0_stream_def)
	    {
		auto cut = ConfigFileUtils::ParsePythonCut(entry.second.Data());

		TString path = ChangePath(entry.first, conf.MC_path());
		std::cout << path << " <-> " << entry.second << std::endl;

		H0Stream -> AddEventSource(path, cut);
	    }

	    float H1_prior = priors[H1_name];
	    float H0_prior = priors[H0_name];

	    disc -> SetH1Source(H1Stream);
	    disc -> SetH0Source(H0Stream);
	    disc -> SetH1Weight(H1_prior);
	    disc -> SetH0Weight(H0_prior);

	    std::cout << "H1_prior = " << H1_prior << std::endl;
	    std::cout << "H0_prior = " << H0_prior << std::endl;

	    std::vector<TString> models = ConfigFileUtils::ParsePythonList(confhandler -> GetField(section, "models").Data());

	    // iterate over the individual models
	    for(auto& model: models)
	    {
		std::cout << "model = " << model << std::endl;
		TString preprocessor = confhandler -> GetField(model_prefix + model, "preprocessor");
		std::cout << "preprocessor = " << preprocessor << std::endl;
		std::cout << "preprocessor_cuts = " << confhandler -> GetField(preprocessor_prefix + preprocessor, "preprocessor_cuts") << std::endl;

		auto preprocessor_cut = ConfigFileUtils::ParsePythonJetCut((confhandler -> GetField(preprocessor_prefix + preprocessor, "preprocessor_cuts").Data()));

		auto get_disc = [=](Tree* in) -> float {
		    return *(in -> ML_discriminants[section]);
		};

		disc -> AddComponent(model, preprocessor_cut, get_disc);
	    }

	    coll -> AddDiscriminant(std::make_pair(H1_name, H0_name), disc);
	}
    }

    delete(confhandler);
    return coll;
}

