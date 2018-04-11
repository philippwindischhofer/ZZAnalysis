#ifndef DiscriminantCollection_h
#define DiscriminantCollection_h

// C++
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

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
#include "TMath.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Tree.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Discriminant.h>

class DiscriminantCollection
{
public:
    float Evaluate(std::pair<TString, TString> combination, Tree* in);
    float EvaluateLog(std::pair<TString, TString> combination, Tree* in);

    float EvaluateKLCorrection(std::pair<TString, TString> combination, Tree* in, bool use_flat_priors);

    void AddDiscriminant(std::pair<TString, TString> combination, Discriminant* disc);
    std::map<std::pair<TString, TString>, Discriminant*> GetDiscs();
    std::vector<std::pair<TString, TString>> GetCategoryPairs(); // returns only the actually existing discriminant pairs, not the opposite-direction ones
    std::vector<TString> GetCategories();
    Discriminant* GetDiscriminant(std::pair<TString, TString> combination);

private:
    std::map<std::pair<TString, TString>, Discriminant*> discs;
};

#endif
