#ifndef EventStream_h
#define EventStream_h

#include <functional>

#include "TString.h"

#include <ZZAnalysis/AnalysisStep/test/classlib/include/Tree.h>

class EventStream
{
public:
    void AddEventSource(TString path, const std::function<bool(Tree*)>& cut);
    std::vector<TString> GetPaths();
    std::vector<std::function<bool(Tree*)>> GetCuts();

private:
    std::vector<TString> paths;
    std::vector<std::function<bool(Tree*)>> cuts;
};

#endif
