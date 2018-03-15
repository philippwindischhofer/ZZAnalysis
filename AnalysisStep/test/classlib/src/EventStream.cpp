#include <ZZAnalysis/AnalysisStep/test/classlib/include/EventStream.h>

void EventStream::AddEventSource(TString path, const std::function<bool(Tree*)>& cut)
{
    paths.push_back(path);
    cuts.push_back(cut);
}

std::vector<TString> EventStream::GetPaths()
{
    return paths;
}

std::vector<std::function<bool(Tree*)>> EventStream::GetCuts()
{
    return cuts;
}
