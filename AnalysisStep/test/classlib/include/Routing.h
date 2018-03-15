#ifndef Routing_h
#define Routing_h

#include "TString.h"
#include "Tree.h"

#include <functional>

class Routing
{
public:
Routing(const std::function<bool(Tree*)>& cut, TString hist_name) : cut(cut), hist_name(hist_name){ };

    const std::function<bool(Tree*)> cut;
    TString hist_name;
};

#endif
