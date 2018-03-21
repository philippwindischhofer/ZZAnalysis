#ifndef MLDiscriminantFactory_h
#define MLDiscriminantFactory_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <functional>

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/DiscriminantCollection.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/EventStream.h>

class MLDiscriminantFactory
{
public:
    static DiscriminantCollection* GenerateDiscriminantCollection(Config& conf);
};

#endif