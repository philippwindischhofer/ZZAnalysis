#ifndef me_discriminants_h
#define me_discriminants_h

#include <ZZAnalysis/AnalysisStep/interface/Discriminants.h>
#include <ZZAnalysis/AnalysisStep/interface/cConstants.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Tree.h>

// discriminants that do not make use of QG tagging
float DVBF2j_ME_disc(Tree* in);
float DVBF1j_ME_disc(Tree* in);
float DWHh_ME_disc(Tree* in);
float DZHh_ME_disc(Tree* in);

float DWHZH_ME_disc(Tree* in);
float DVBFWH_ME_disc(Tree* in);
float DVBFZH_ME_disc(Tree* in);

// versions of the discriminants that do make use of QG tagging
float DVBF2j_ME_QG_disc(Tree* in);

float generic_disc(float H1_ME, float H0_ME);

#endif