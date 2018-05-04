#ifndef cuts_h
#define cuts_h

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Tree.h>

float mZZ_cut_min();
float mZZ_cut_max();

bool no_cut(Tree* in);
bool mZZ_cut(Tree* in);
bool extraLeptons_0_cut(Tree* in);
bool extraLeptons_1_cut(Tree* in);
bool extraLeptons_2_cut(Tree* in);
bool extraLeptons_12_cut(Tree* in);
bool extraNeutrinos_0_cut(Tree* in);
bool extraNeutrinos_2_cut(Tree* in);
bool extraNeutrinos_0_Leptons_0_cut(Tree* in);
bool final_state_4e_cut(Tree* in);
bool final_state_2e2mu_cut(Tree* in);
bool final_state_4mu_cut(Tree* in);

int countAssocLeptons(Tree* in);
int countNeutrinos(Tree* in);

#endif
