#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>

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
