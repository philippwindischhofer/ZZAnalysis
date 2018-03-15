#ifndef Discriminant_h
#define Discriminant_h

// C++
#include <iostream>
#include <fstream>
#include <string>
#include <functional>

#include <boost/range/combine.hpp>

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

#include <ZZAnalysis/AnalysisStep/test/classlib/include/EventStream.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Tree.h>

class Discriminant
{
public:
    Discriminant(TString calib_dir);
    ~Discriminant();

    float Evaluate(Tree* in);

    void AddComponent(TString name, const std::function<bool(Tree*)> cut, const std::function<float(Tree*)> disc);

    void SetH1Source(EventStream* H1_source);
    void SetH0Source(EventStream* H0_source);

    std::vector<TString> GetNames();
    std::vector<std::function<bool(Tree*)>> GetCuts();
    std::vector<std::function<float(Tree*)>> GetDiscs();

    EventStream* GetH1Source();
    EventStream* GetH0Source();
    
private:
    TString calib_dir;

    // the two streams of data this discriminant is supposed to separate. this is needed in order to compute the distributions of its pieces on the H1- and H0 files
    EventStream* H1_source;
    EventStream* H0_source;

    // a list of cuts and corresponding discriminants. when evaluating the global discriminant, this list is traversed one by one. the discriminant that corresponds to the first matching cut is evaluated
    std::vector<TString> names;
    std::vector<std::function<bool(Tree*)>> cuts;
    std::vector<std::function<float(Tree*)>> discs;

    // the calibration splines for each component
    std::vector<TSpline3*> H1_calines;
    std::vector<TSpline3*> H0_calines;

    // ... and their calibration status
    std::vector<bool> calibration_status;
};

#endif
