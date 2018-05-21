#ifndef RRMultiClassCombinator_h
#define RRMultiClassCombinator_h

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

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Tree.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/DiscriminantCollection.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/MultiClassCombinator.h>

class RRMultiClassCombinator: public MultiClassCombinator
{
public:
    RRMultiClassCombinator();
    ~RRMultiClassCombinator();

    std::map<TString, float> Evaluate(Tree* in, DiscriminantCollection* coll);
    std::map<TString, float> Evaluate(Tree* in, DiscriminantCollection* coll, std::vector<TString> categories);
};

#endif
