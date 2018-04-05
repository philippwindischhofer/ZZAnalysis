#ifndef MLDiscriminantFactoryFullCategorySet_h
#define MLDiscriminantFactoryFullCategorySet_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <functional>

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/DiscriminantCollection.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/EventStream.h>

class MLDiscriminantFactoryFullCategorySet
{
public:
    static DiscriminantCollection* GenerateDiscriminantCollection(TString outdir, Config& conf);
    static DiscriminantCollection* GenerateDiscriminantCollection(TString outdir, Config& conf, float VBF_prior, float ggH_prior, float WHhadr_prior, float ZHhadr_prior, float WHlept_prior, float ZHlept_prior, float ZHMET_prior, float ttHhadr_prior, float ttHlept_prior);
};

#endif
