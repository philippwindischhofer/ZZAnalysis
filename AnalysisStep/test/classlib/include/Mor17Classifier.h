#ifndef Mor17Classifier_h
#define Mor17Classifier_h

// C++
#include <iostream>
#include <fstream>
#include <string>

// ROOT
#include "TApplication.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TGraphAsymmErrors.h"
#include "TMultiGraph.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TColor.h"
#include "Classifier.h"
#include "Tree.h"

class Mor17Classifier: public Classifier
{
public:
    Mor17Classifier();
    ~Mor17Classifier();

    int ClassifyEvent();
    int ClassifyThisEvent(Tree* in);
};

#endif
