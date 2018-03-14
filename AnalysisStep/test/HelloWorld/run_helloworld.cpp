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
#include "TH1F.h"
#include "TGraph.h"
#include "TMultiGraph.h"

int main( int argc, char *argv[] )
{
    std::cout << "Hello World" << std::endl;

    TCanvas* canv = new TCanvas("canv", "canv", 800, 600);
    TPad* pad = new TPad("pad", "pad", 0.0, 0.0, 1.0, 1.0, kWhite, 0, 0);
    TMultiGraph* tmg = new TMultiGraph();

    std::vector<TGraph*> graphs;

    Double_t x[1000];
    Int_t n = 1000;
    for (Int_t i=0;i<n;i++) {
	x[i] = i*0.1;
    }
    
    TGraph* tg = new TGraph(n,x,x);
    tg -> SetLineColor(kBlack);
    graphs.push_back(tg);

    for(unsigned int i = 0; i < graphs.size(); i++)
    {
    	tmg -> Add(graphs.at(i));
    }

    TH1F* hist = new TH1F("hist", "hist", 100, 0.0, 1.0);
    
    canv -> cd();
    pad -> Draw();
    pad -> cd();
    
    tmg -> SetTitle("test");
    tmg -> Draw("AC*");
//    tmg -> Draw("same");

    //hist -> Draw();

    canv -> cd();
    pad -> Draw();
    gPad -> RedrawAxis();
    canv -> Update();

    canv -> SaveAs("test.pdf");

    return(0);
}
