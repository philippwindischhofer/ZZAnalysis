#ifndef SignalAssignment_h
#define SignalAssignment_h

#include "TString.h"

class SignalAssignment
{
public:
SignalAssignment(TString category, TString histogram, int bin) : category(category), histogram(histogram), bin(bin) {};

    TString category;
    TString histogram;
    int bin;
};

#endif
