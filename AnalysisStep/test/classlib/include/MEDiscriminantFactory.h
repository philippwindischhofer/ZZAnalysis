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

    // delivers non-integrated, non-calibrated discriminants, where each component of a standard discriminant is packaged into its own discriminant. Thus, it makes for easy ROC curve generation
    static DiscriminantCollection* GenerateRawDiscriminantCollection(Config& conf, bool use_QG_tagging);
};

#endif
