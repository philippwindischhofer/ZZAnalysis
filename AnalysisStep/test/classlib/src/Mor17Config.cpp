#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor17Config.h>

std::vector<TString> Mor17Config::cat_labels()
{
    std::vector<TString> cat_labels = {
	"Untagged", 
	"#splitline{VBF-1jet}{ tagged}", 
	"#splitline{VBF-2jet}{ tagged}", 
	"#splitline{VH-leptonic}{    tagged}", 
	"#splitline{VH-hadronic}{     tagged}", 
	"ttH tagged", 
	"#splitline{VH-E_{T}^{miss}}{ tagged}"
    };

    return cat_labels;
}

std::vector<TString> Mor17Config::cat_labels_text()
{    
    std::vector<TString> cat_labels_text = {
	"untagged",
	"VBF-1j",
	"VBF-2j",
	"$VH$-lept",
	"$VH$-hadr",
	"$t\\bar{t}H$",
	"$VH$-MET"
    };

    return cat_labels_text;
}

int Mor17Config::bin_index(TString desc)
{
    std::map<TString, int> bin_index_vals = {
	{"untagged_bin", 1},
	{"VBF1jet_bin", 2},
	{"VBF2jet_bin", 3},
	{"VHleptonic_bin", 4},
	{"VHhadronic_bin", 5},
	{"ttH_bin", 6},
	{"VHMET_bin", 7}
    };

    return bin_index_vals[desc];
}

TString Mor17Config::storage_prefix()
{
    TString storage_prefix = "Mor17_";
    return storage_prefix;
}

TString Mor17Config::id()
{
    TString id = "Mor17";
    return id;
}
