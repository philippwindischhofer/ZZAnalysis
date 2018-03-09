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
#include "TLine.h"

class CatPlotter
{
public:
    CatPlotter();
    ~CatPlotter();

    // makes a generic category plot
    void Construct(std::vector<TH1F*> hists, std::vector<TString> cat_labels, std::vector<TString> source_labels, std::vector<float> yields, TString title, float lumi);
    void DrawLabel(TString label);
    void AddLine(double coord);
    void SaveAs(TString file);
    THStack* GetStack();
    void Update();

private:
    THStack* hs;
    TCanvas* canv;
    TPad* pad1;
    TPad* pad2;
    TLegend* leg;

    std::vector<double> line_coords;

    bool draw_legend;
    bool draw_label;
    std::vector<TString> cat_labels;
    std::vector<TString> source_labels;
    std::vector<float> yields;
    float lumi;
    TString title;
    TString label;

    void Redraw();
};

#endif
