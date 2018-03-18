#ifndef MEDiscriminantFactory_h
#define MEDiscriminantFactory_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <functional>

#include "TH1F.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/DiscriminantCollection.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/EventStream.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/me_discriminants.h>

class MEDiscriminantFactory
{
public:
    static DiscriminantCollection* GenerateDiscriminantCollection(TString out_folder, Config& conf);
};

#endif
