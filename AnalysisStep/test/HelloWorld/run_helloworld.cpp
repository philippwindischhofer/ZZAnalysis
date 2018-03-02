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
#include <ZZAnalysis/AnalysisStep/test/HelloWorld/include/Tree.h>

int main( int argc, char *argv[] )
{
    std::cout << "Hello World" << std::endl;

    TFile* input_file = new TFile("/data_CMS/cms/tsculac/CJLST_NTuples/ggH125/ZZ4lAnalysis.root");

    TTree* input_tree = (TTree*)input_file -> Get("ZZTree/candTree");
    
    Tree* t = new Tree(input_tree);
    t -> Loop();

    return(0);
}
