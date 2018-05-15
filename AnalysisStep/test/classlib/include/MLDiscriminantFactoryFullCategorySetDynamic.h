#ifndef MLDiscriminantFactoryFullCategorySetDynamic_h
#define MLDiscriminantFactoryFullCategorySetDynamic_h

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <functional>

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Config.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/ConfigFileHandler.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/ConfigFileUtils.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/DiscriminantCollection.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/EventStream.h>

class MLDiscriminantFactoryFullCategorySetDynamic
{
public:
    static DiscriminantCollection* GenerateDiscriminantCollection(TString calibration_dir, TString config_path, Config& conf);

    static DiscriminantCollection* GenerateDiscriminantCollection(TString calibration_dir, TString config_path, Config& conf, std::map<TString, float> priors);

    static DiscriminantCollection* GenerateDiscriminantCollection(TString calibration_dir, TString config_path, Config& conf, float VBF_prior, float ggH_prior, float WHhadr_prior, float ZHhadr_prior, float WHlept_prior, float ZHlept_prior, float ZHMET_prior, float ttHhadr_prior, float ttHlept_prior, float bkg_prior, float qq_prior);

private:
    static TString ChangePath(TString original, TString new_head);

};

#endif
