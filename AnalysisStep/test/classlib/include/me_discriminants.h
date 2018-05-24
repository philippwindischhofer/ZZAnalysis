#ifndef me_discriminants_h
#define me_discriminants_h

#include <iostream>
#include <cmath>
#include <cfloat>

#include "TMath.h"

#include <ZZAnalysis/AnalysisStep/interface/Discriminants.h>
#include <ZZAnalysis/AnalysisStep/interface/cConstants.h>
#include <ZZAnalysis/AnalysisStep/test/classlib/include/Tree.h>

// signal / background discriminant
float D_bkg_ME_disc(Tree* in);

// discriminants that do not make use of QG tagging
float DVBF2j_ME_disc(Tree* in);
float DVBF2j_bkg_ME_disc(Tree* in);

float DVBF1j_ME_disc(Tree* in);
float DWHh_ME_disc(Tree* in);
float DZHh_ME_disc(Tree* in);
float DVH_bkg_ME_disc(Tree* in);

float DWHZH_ME_disc(Tree* in);
float DVBFWH_ME_disc(Tree* in);
float DVBFZH_ME_disc(Tree* in);

// versions of the discriminants that do make use of QG tagging
float DVBF2j_ME_QG_disc(Tree* in);
float DVBF1j_ME_QG_disc(Tree* in);
float DWHh_ME_QG_disc(Tree* in);
float DZHh_ME_QG_disc(Tree* in);

float DWHZH_ME_QG_disc(Tree* in);
float DVBFWH_ME_QG_disc(Tree* in);
float DVBFZH_ME_QG_disc(Tree* in);

float generic_disc(float H1_ME, float H0_ME);
float generic_Pg_Pq(Tree* in);

#endif
