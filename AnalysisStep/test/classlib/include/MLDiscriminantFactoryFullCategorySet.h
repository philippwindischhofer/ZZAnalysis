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
};

#endif
