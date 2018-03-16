#ifndef MEDiscriminantFactory_h
#define MEDiscriminantFactory_h

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/DiscriminantCollection.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/EventStream.h>

#include <ZZAnalysis/AnalysisStep/interface/Discriminants.h>
#include <ZZAnalysis/AnalysisStep/interface/cConstants.h>

class MEDiscriminantFactory
{
public:
    static DiscriminantCollection* GenerateDiscriminantCollection(TString out_folder, Config& conf);
};

#endif
