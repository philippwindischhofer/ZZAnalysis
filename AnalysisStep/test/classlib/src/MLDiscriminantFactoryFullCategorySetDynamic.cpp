#include <ZZAnalysis/AnalysisStep/test/classlib/include/MLDiscriminantFactoryFullCategorySetDynamic.h>

DiscriminantCollection* MLDiscriminantFactoryFullCategorySetDynamic::GenerateDiscriminantCollection(TString outdir, Config& conf)
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

    return GenerateDiscriminantCollection(outdir, conf, VBF_prior, ggH_prior, WHhadr_prior, ZHhadr_prior, WHlept_prior, ZHlept_prior, ZHMET_prior, ttHhadr_prior, ttHlept_prior);

}

DiscriminantCollection* MLDiscriminantFactoryFullCategorySetDynamic::GenerateDiscriminantCollection(TString outdir, Config& conf, float VBF_prior, float ggH_prior, float WHhadr_prior, float ZHhadr_prior, float WHlept_prior, float ZHlept_prior, float ZHMET_prior, float ttHhadr_prior, float ttHlept_prior)
{
    DiscriminantCollection* coll = new DiscriminantCollection();

    return coll;
}

