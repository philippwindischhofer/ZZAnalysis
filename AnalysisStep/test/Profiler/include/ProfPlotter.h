#ifndef ProfPlotter_h
#define ProfPlotter_h

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

class ProfPlotter
{
public:
    ProfPlotter();
    ~ProfPlotter();

    void Construct(std::vector<TH1F*> hist_vec, std::vector<TString> source_labels, TString label_x, TString label_y, TString label_l, TString label_r, TString args);
    void SaveAs(TString outfile);

private:
    THStack* hs;
    TCanvas* canv;
    TPad* pad1;
    TPad* pad2;
    TLegend* leg;

    bool draw_legend;

    TString title;
    TString label_l;
    TString label_r;
    TString label_x;
    TString label_y;

    void Redraw(TString args);
};

#endif
