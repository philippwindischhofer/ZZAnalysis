#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>

Mor18Config::Mor18Config() : Config()
{ }

std::vector<int> Mor18Config::categories()
{
    std::vector<int> categories = {
	UntaggedMor18,
	VBF1jTaggedMor18,
	VBF2jTaggedMor18,
	VHLeptTaggedMor18,
	VHHadrTaggedMor18,
	ttHLeptTaggedMor18,
	ttHHadrTaggedMor18,
	VHMETTaggedMor18
    };
    
    return categories;
}

TString Mor18Config::cat_label(int category)
{
    std::map<int, TString> mapping = {
	{UntaggedMor18, "Untagged"},
	{VBF1jTaggedMor18, "#splitline{VBF-1jet}{ tagged}"},
	{VBF2jTaggedMor18, "#splitline{VBF-2jet}{ tagged}"},
	{VHLeptTaggedMor18, "#splitline{VH-leptonic}{    tagged}"},
	{VHHadrTaggedMor18, "#splitline{VH-hadronic}{     tagged}"},
	{ttHLeptTaggedMor18, "#splitline{ttH leptonic}{   tagged}"},
	{ttHHadrTaggedMor18, "#splitline{ttH hadronic}{   tagged}"},
	{VHMETTaggedMor18, "#splitline{VH-E_{T}^{miss}}{ tagged}"}	
    };

    return mapping[category];
}

TString Mor18Config::cat_label_text(int category)
{
    std::map<int, TString> mapping = {
	{UntaggedMor18, "untagged"},
	{VBF1jTaggedMor18, "VBF-1j"},
	{VBF2jTaggedMor18, "VBF-2j"},
	{VHLeptTaggedMor18, "$VH$-lept"},
	{VHHadrTaggedMor18, "$VH$-hadr"},
	{ttHLeptTaggedMor18, "$ttH$-lept"},
	{ttHHadrTaggedMor18, "$ttH$-hadr"},
	{VHMETTaggedMor18, "$VH$-MET"}	
    };

    return mapping[category];
}


std::vector<TString> Mor18Config::abstract_categories()
{
    std::vector<TString> abs_categories = {
	"UntaggedMor18",
	"VBF1jTaggedMor18",
	"VBF2jTaggedMor18",
	"VHLeptTaggedMor18",
	"VHHadrTaggedMor18",
	"ttHLeptTaggedMor18",
	"ttHHadrTaggedMor18",
	"VHMETTaggedMor18"
    };

    return abs_categories;
}

TString Mor18Config::abs_cat_label(TString abs_cat)
{
   std::map<TString, TString> mapping = {
	{"UntaggedMor18", "Untagged"},
	{"VBF1jTaggedMor18", "#splitline{VBF-1jet}{ tagged}"},
	{"VBF2jTaggedMor18", "#splitline{VBF-2jet}{ tagged}"},
	{"VHLeptTaggedMor18", "#splitline{VH-leptonic}{    tagged}"},
	{"VHHadrTaggedMor18", "#splitline{VH-hadronic}{     tagged}"},
	{"ttHLeptTaggedMor18", "#splitline{ttH leptonic}{   tagged}"},
	{"ttHHadrTaggedMor18", "#splitline{ttH hadronic}{   tagged}"},
	{"VHMETTaggedMor18", "#splitline{VH-E_{T}^{miss}}{ tagged}"}	
    };

    return mapping[abs_cat];
}

std::vector<SignalAssignment*> Mor18Config::signal_assignment()
{
    std::vector<SignalAssignment*> assignment = {
	new SignalAssignment("UntaggedMor18", "ggHhist", UntaggedMor18),

	new SignalAssignment("VBF1jTaggedMor18", "VBFhist", VBF1jTaggedMor18),
	new SignalAssignment("VBF2jTaggedMor18", "VBFhist", VBF2jTaggedMor18),

	new SignalAssignment("VHLeptTaggedMor18", "WHlnuhist", VHLeptTaggedMor18),
	new SignalAssignment("VHLeptTaggedMor18", "ZH2lhist", VHLeptTaggedMor18),

	new SignalAssignment("VHHadrTaggedMor18", "WHXhist", VHHadrTaggedMor18),
	new SignalAssignment("VHHadrTaggedMor18", "ZHXhist", VHHadrTaggedMor18),

	new SignalAssignment("ttHLeptTaggedMor18", "ttH1lhist", ttHLeptTaggedMor18),
	new SignalAssignment("ttHLeptTaggedMor18", "ttH2lhist", ttHLeptTaggedMor18),
	new SignalAssignment("ttHHadrTaggedMor18", "ttH0lhist", ttHHadrTaggedMor18),

	new SignalAssignment("VHMETTaggedMor18", "ZHnunuhist", VHMETTaggedMor18)
    };

    return assignment;
}

TString Mor18Config::storage_prefix()
{
    TString storage_prefix = "Mor18_";
    return storage_prefix;
}

TString Mor18Config::id()
{
    TString id = "Mor18";
    return id;
}
