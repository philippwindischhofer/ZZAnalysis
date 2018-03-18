#ifndef me_discriminants_h
#define me_discriminants_h

#include <ZZAnalysis/AnalysisStep/interface/Discriminants.h>
#include <ZZAnalysis/AnalysisStep/interface/cConstants.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Tree.h>

float DVBF2j_ME_disc(Tree* in);
float DVBF1j_ME_disc(Tree* in);
float DWHh_ME_disc(Tree* in);
float DZHh_ME_disc(Tree* in);

float DWHZH_ME_disc(Tree* in);
float DVBFWH_ME_disc(Tree* in);
float DVBFZH_ME_disc(Tree* in);

float generic_disc(float H1_ME, float H0_ME);

#endif
