#ifndef DiscriminantCollection_h
#define DiscriminantCollection_h

// C++
#include <iostream>
#include <fstream>
#include <string>

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
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Discriminant.h>

class DiscriminantCollection
{
public:
    float Evaluate(std::pair<TString, TString> combination, Tree* in);
    void AddDiscriminant(std::pair<TString, TString> combination, Discriminant* disc);
    std::map<std::pair<TString, TString>, Discriminant*> GetDiscs();

private:
    std::map<std::pair<TString, TString>, Discriminant*> discs;
};

#endif
