#ifndef VotingMultiClassCombinator_h
#define VotingMultiClassCombinator_h

// C++
#include <iostream>
#include <fstream>
#include <string>
#include <limits>

// ROOT
#include "TApplication.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TStyle.h"
#include "TKDE.h"
#include "TSpline.h"
#include "TF1.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Tree.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/DiscriminantCollection.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/MultiClassCombinator.h>

class VotingMultiClassCombinator: public MultiClassCombinator
{
public:
    VotingMultiClassCombinator();
    ~VotingMultiClassCombinator();

    virtual std::map<TString, float> Evaluate(Tree* in, DiscriminantCollection* coll);

};

#endif
