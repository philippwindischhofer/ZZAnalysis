#include <ZZAnalysis/AnalysisStep/test/classlib/include/BayesClassifier.h>

BayesClassifier::BayesClassifier(TString calibration_folder, TString config_path, TString engine)
{
    // set the path to where the augmented files are (even though this information is not needed for the classification step)
    Mor18Config conf(calibration_folder + "../augmentation/");

    this -> calibration_folder = calibration_folder;
    this -> config_path = config_path;

    // prepare the discriminant collection without any priors
    coll = MLDiscriminantFactoryFullCategorySetDynamic::GenerateDiscriminantCollection(calibration_folder, config_path, conf);

    if(engine == "robin")
    {
	std::cout << "using round robin" << std::endl;
	comb = new RRMultiClassCombinator();
    }
    else if(engine == "tree")
    {
	std::cout << "using tree (single elimination tournament)" << std::endl;
	comb = new TreeMultiClassCombinator();
    }
    else
    {
	std::cerr << "Error: requested engine '" + engine + "' not implemented!" << std::endl;
	return;
    }
}

BayesClassifier::~BayesClassifier()
{  }

void BayesClassifier::SetEngineParameter(TString parameter_name, float parameter_value)
{
    comb -> SetParameter(parameter_name, parameter_value);
}

void BayesClassifier::SetPriors(float VBF_prior, float ggH_prior, float WHhadr_prior, float ZHhadr_prior, float WHlept_prior, float ZHlept_prior, float ZHMET_prior, float ttHhadr_prior, float ttHlept_prior, float bkg_prior, float qq_prior)
{
    Mor18Config conf;

    // update the discriminant collection with the new prior weights
    coll = MLDiscriminantFactoryFullCategorySetDynamic::GenerateDiscriminantCollection(calibration_folder, config_path, conf, VBF_prior, ggH_prior, WHhadr_prior, ZHhadr_prior, WHlept_prior, ZHlept_prior, ZHMET_prior, ttHhadr_prior, ttHlept_prior, bkg_prior, qq_prior);
}

int BayesClassifier::ClassifyThisEvent(Tree* in)
{
    comb -> Evaluate(in, coll);
    TString winner = comb -> GetWinningCategory();
    float margin = comb -> GetWinningMargin();

    // VBF splitting: put all VBF-classified events with 2 or more jets into the VBF2j category, those with 1 jet into the VBF1j category, the rest into "untagged"
    int VBF_cat = UntaggedMor18;
    if(in -> nCleanedJetsPt30 >= 2)
    {
	VBF_cat = VBF2jTaggedMor18;
    }
    else if(in -> nCleanedJetsPt30 == 1)
    {
	VBF_cat = VBF1jTaggedMor18;
    }

    std::map<TString, int> conversion = {
	{"VBF", VBF_cat},
	{"ZHh", VHHadrTaggedMor18},
	{"WHh", VHHadrTaggedMor18},
	{"ZHl", VHLeptTaggedMor18},
	{"WHl", VHLeptTaggedMor18},
	{"ZHMET", VHMETTaggedMor18},
	{"ttHl", ttHLeptTaggedMor18},
	{"ttHh", ttHHadrTaggedMor18},
	{"ggH", UntaggedMor18},
	{"ZX", UntaggedMor18}, // redirect the background-like events into the untagged category as well
	{"qq", UntaggedMor18}
    };

    return conversion[winner];
}
