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
#include <random>

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

    virtual std::map<TString, float> Evaluate(Tree* in, DiscriminantCollection* coll, std::vector<TString> categories, int iterations);
    virtual std::map<TString, float> Evaluate(Tree* in, DiscriminantCollection* coll);

    void UseFlatPriorsInKL(bool use_flat_priors);

private:
    std::vector<TString> TreeTournament(std::vector<TString> players, Tree* in, DiscriminantCollection* coll);

    /* int min_iterations = 10; */
    /* int max_iterations = 5000; */

    bool use_KL = false;
    bool use_flat_priors = true;
};

#endif
