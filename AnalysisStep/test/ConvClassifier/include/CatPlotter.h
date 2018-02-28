#ifndef CatPlotter_h
#define CatPlotter_h

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
#include "THStack.h"
#include "TLatex.h"
#include "TLegend.h"

class CatPlotter
{
public:
    // makes a generic category plot
    static void Construct(std::vector<TH1F*> hists, std::vector<TString> cat_labels, std::vector<TString> source_labels, std::vector<float> yields, TString file);
};

#endif
