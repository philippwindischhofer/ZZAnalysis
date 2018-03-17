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

int Mor18Config::number_categories()
{
    return categories().size();
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

int Mor18Config::number_abstract_categories()
{
    return abstract_categories().size();
}

int Mor18Config::abs_cat_number(TString category)
{
    std::vector<TString> cats = abstract_categories();

    // histogram bins in ROOT are 1-indexed!!
    return std::find(cats.begin(), cats.end(), category) - cats.begin();
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

// need to make sure that every key-value combination occurs only once, but each key can occur multiple times!
std::vector<std::pair<TString, int>> Mor18Config::bin_assignment()
{
    std::vector<std::pair<TString, int>> bin_assign;
    std::vector<SignalAssignment*> assignments = signal_assignment();

    for(auto& assignment: assignments)
    {
	auto cur = std::make_pair(assignment -> category, assignment -> bin);
	if(std::find(bin_assign.begin(), bin_assign.end(), cur) == bin_assign.end())
	    bin_assign.push_back(cur);
    }

    return bin_assign;
}

std::vector<std::pair<TString, TString>> Mor18Config::histogram_assignment()
{
    std::vector<std::pair<TString, TString>> histo_assign;
    std::vector<SignalAssignment*> assignments = signal_assignment();
    
    for(auto& assignment: assignments)
    {
	auto cur = std::make_pair(assignment -> category, assignment -> histogram);
	if(std::find(histo_assign.begin(), histo_assign.end(), cur) == histo_assign.end())
	    histo_assign.push_back(cur);
    }

    return histo_assign;
}

std::vector<TString> Mor18Config::ordered_cat_labels()
{
    std::vector<int> cats = categories();
    std::vector<TString> ordered_labels;

    for(auto cat: cats)
    {
	ordered_labels.push_back(cat_label(cat));
    }

    return ordered_labels;
}

int Mor18Config::bin_number(int category)
{
    std::vector<int> cats = categories();

    // TODO: add out-of-bounds checking!

    // histogram bins in ROOT are 1-indexed!!
    return std::find(cats.begin(), cats.end(), category) - cats.begin() + 1;
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

std::vector<TString> Mor18Config::cat_labels()
{
    std::vector<TString> cat_labels = {
	"Untagged", 
	"#splitline{VBF-1jet}{ tagged}", 
	"#splitline{VBF-2jet}{ tagged}", 
	"#splitline{VH-leptonic}{    tagged}", 
	"#splitline{VH-hadronic}{     tagged}", 
	"#splitline{ttH leptonic}{   tagged}", 
	"#splitline{ttH hadronic}{   tagged}",
	"#splitline{VH-E_{T}^{miss}}{ tagged}"
    };

    return cat_labels;
}

std::vector<TString> Mor18Config::cat_labels_text()
{    
    std::vector<TString> cat_labels_text = {
	"untagged",
	"VBF-1j",
	"VBF-2j",
	"$VH$-lept",
	"$VH$-hadr",
	"$ttH$-lept",
	"$ttH$-hadr",
	"$VH$-MET"
    };

    return cat_labels_text;
}

int Mor18Config::bin_index(TString desc)
{
    std::map<TString, int> bin_index_vals = {
	{"untagged_bin", 1},
	{"VBF1jet_bin", 2},
	{"VBF2jet_bin", 3},
	{"VHleptonic_bin", 4},
	{"VHhadronic_bin", 5},
	{"ttH_leptonic_bin", 6},
	{"ttH_hadronic_bin", 7},
	{"VHMET_bin", 8}
    };

    return bin_index_vals[desc];
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
