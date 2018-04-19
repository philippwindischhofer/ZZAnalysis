#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18LIClassifier.h>

Mor18LIClassifier::Mor18LIClassifier(TString out_folder, TString engine)
{
    //manual_WPs = false;

    Mor18Config conf;

    calibration_folder = out_folder;

    // prepare the discriminant collection without any priors
    coll = MLDiscriminantFactoryFullCategorySet::GenerateDiscriminantCollection(out_folder, conf);

    if(engine == "voting")
    {
	std::cout << "using simple voting" << std::endl;
	comb = new VotingMultiClassCombinator();
    }
    else if(engine == "rand")
    {
	std::cout << "using RAND without KL corrections" << std::endl;
	comb = new RANDKLMultiClassCombinator(false);
    }
    else if(engine == "rand_KL")
    {
	std::cout << "using RAND with KL corrections" << std::endl;
	comb = new RANDKLMultiClassCombinator(true);
	static_cast<RANDKLMultiClassCombinator*>(comb) -> UseFlatPriorsInKL(false);
    }
    else if(engine == "rand_KL_flat")
    {
	std::cout << "using RAND with KL corrections and FLAT priors" << std::endl;
	comb = new RANDKLMultiClassCombinator(true);
	static_cast<RANDKLMultiClassCombinator*>(comb) -> UseFlatPriorsInKL(true);
    }
    else if(engine == "tree")
    {
	std::cout << "using TREE without KL corrections" << std::endl;
	comb = new TreeKLMultiClassCombinator(false);
    }
    else if(engine == "tree_KL")
    {
	std::cout << "using TREE with KL corrections" << std::endl;
	comb = new TreeKLMultiClassCombinator(true);
	static_cast<TreeKLMultiClassCombinator*>(comb) -> UseFlatPriorsInKL(false);
    }
    else if(engine == "tree_KL_flat")
    {
	std::cout << "using TREE with KL corrections and FLAT priors" << std::endl;
	comb = new TreeKLMultiClassCombinator(true);
	static_cast<TreeKLMultiClassCombinator*>(comb) -> UseFlatPriorsInKL(true);
    }
}

Mor18LIClassifier::~Mor18LIClassifier()
{  }

void Mor18LIClassifier::SetEngineParameter(TString parameter_name, float parameter_value)
{
    comb -> SetParameter(parameter_name, parameter_value);
}

void Mor18LIClassifier::SetPriors(float VBF_prior, float ggH_prior, float WHhadr_prior, float ZHhadr_prior, float WHlept_prior, float ZHlept_prior, float ZHMET_prior, float ttHhadr_prior, float ttHlept_prior)
{
    Mor18Config conf;

    // update the discriminant collection with the new prior weights
    coll = MLDiscriminantFactoryFullCategorySet::GenerateDiscriminantCollection(calibration_folder, conf, VBF_prior, ggH_prior, WHhadr_prior, ZHhadr_prior, WHlept_prior, ZHlept_prior, ZHMET_prior, ttHhadr_prior, ttHlept_prior);
}

// after the restructoring, only this method will remain, and it will overload the corresponding virtual method from the base class
int Mor18LIClassifier::ClassifyThisEvent(Tree* in)
{
    comb -> Evaluate(in, coll);
    TString winner = comb -> GetWinningCategory();
    float margin = comb -> GetWinningMargin();

    // VBF splitting: put all VBF-classified events with 2 or more jets into the VBF2j category, those with 1 or less jets into the VBF1j category (to be investigated: put a separate category for the latter events?)
    int VBF_cat = (in -> nCleanedJetsPt30 >= 2) ? VBF2jTaggedMor18 : VBF1jTaggedMor18;

    std::map<TString, int> conversion = {
	{"VBFH125", VBF_cat},
	{"ZHhadr", VHHadrTaggedMor18},
	{"WHhadr", VHHadrTaggedMor18},
	{"ZHlept", VHLeptTaggedMor18},
	{"WHlept", VHLeptTaggedMor18},
	{"ZHMET", VHMETTaggedMor18},
	{"ttHlept", ttHLeptTaggedMor18},
	{"ttHhadr", ttHHadrTaggedMor18},
	{"ggH125", UntaggedMor18}
    };

    return conversion[winner];
}
