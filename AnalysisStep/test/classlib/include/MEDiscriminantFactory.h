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
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Classifier.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/DiscriminantCollection.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/EventStream.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/me_discriminants.h>

class MEDiscriminantFactory
{
public:
    static DiscriminantCollection* GenerateDiscriminantCollection(TString out_folder, Config& conf);

    // delivers non-integrated, non-calibrated discriminants, where each component of a standard discriminant is packaged into its own discriminant. Thus, it makes for easy ROC curve generation
    static DiscriminantCollection* GenerateRawDiscriminantCollection(Config& conf, bool use_QG_tagging);

    // same as above, but additionally places cuts in such a way that the matrix element are only evaluated on those events that they would actually see in practice as well, i.e. only on those that have *not* yet been put in any other category further upstream in the selection
    static DiscriminantCollection* GenerateRawDiscriminantCollectionSequentialCut(Config& conf, bool use_QG_tagging);

};

#endif
