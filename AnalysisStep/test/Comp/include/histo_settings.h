#ifndef comp_histo_settings_h
#define comp_histo_settings_h

#include "TString.h"

#define Mor18

float lumi = 35.9;

#ifdef Mor18

#define STORAGE_PREFIX "Mor18_"

#endif

#ifdef Mor17

#define STORAGE_PREFIX "Mor17_"

#endif

#endif // histo_settings_h
