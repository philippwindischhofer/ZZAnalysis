#ifndef TreeKLMultiClassCombinator_h
#define TreeKLMultiClassCombinator_h

// C++
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cstdlib>

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

class TreeKLMultiClassCombinator: public MultiClassCombinator
{
public:
    TreeKLMultiClassCombinator(bool use_KL);
    ~TreeKLMultiClassCombinator();

    virtual std::map<TString, float> Evaluate(Tree* in, DiscriminantCollection* coll);

private:
    std::vector<TString> TreeTournament(std::vector<TString> players, Tree* in, DiscriminantCollection* coll);
    int iterations = 10;
    bool use_KL = false;
};

#endif
