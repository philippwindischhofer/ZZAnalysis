#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18CoarseConfig.h>

Mor18CoarseConfig::Mor18CoarseConfig(): Mor18Config()
{
    // set up the routing table
    routing.push_back(std::make_pair("ggH125", new Routing(no_cut, "ggHhist")));
    routing.push_back(std::make_pair("VBFH125", new Routing(no_cut, "VBFhist")));

    routing.push_back(std::make_pair("WplusH125", new Routing(no_cut, "WHhist")));
    routing.push_back(std::make_pair("WminusH125", new Routing(no_cut, "WHhist")));

    routing.push_back(std::make_pair("ZH125", new Routing(no_cut, "ZHhist")));

    routing.push_back(std::make_pair("ttH125", new Routing(no_cut, "ttHhist")));

    routing.push_back(std::make_pair("ZZTo4l", new Routing(no_cut, "qq4lhist")));

    routing.push_back(std::make_pair("ggTo2e2mu_Contin_MCFM701", new Routing(no_cut, "gg4lhist")));
    routing.push_back(std::make_pair("ggTo2e2tau_Contin_MCFM701", new Routing(no_cut, "gg4lhist")));
    routing.push_back(std::make_pair("ggTo2mu2tau_Contin_MCFM701", new Routing(no_cut, "gg4lhist")));
    routing.push_back(std::make_pair("ggTo4e_Contin_MCFM701", new Routing(no_cut, "gg4lhist")));
    routing.push_back(std::make_pair("ggTo4mu_Contin_MCFM701", new Routing(no_cut, "gg4lhist")));
    routing.push_back(std::make_pair("ggTo4tau_Contin_MCFM701", new Routing(no_cut, "gg4lhist")));
}

TString Mor18CoarseConfig::source_label(TString histname)
{
    std::map<TString, TString> mapping = {
	{"ggHhist", "ggH"}, 
	{"VBFhist", "VBF"}, 
	{"WHhist", "WH"},
	{"ZHhist", "ZH"},
	{"ttHhist", "t#bar{t}H"},
	{"qq4lhist", "q#bar{q} #rightarrow ZZ #rightarrow 4l"},
	{"gg4lhist","gg #rightarrow ZZ #rightarrow 4l"}
    };

    return mapping[histname];
}

int Mor18CoarseConfig::source_color(TString histname)
{
    std::map<TString, int> mapping = {
	{"ggHhist", kBlue - 9}, 
	{"VBFhist", kGreen - 6}, 
	{"WHhist", kRed - 7},
	{"ZHhist", kYellow - 7},
	{"ttHhist", kCyan - 6},
	{"qq4lhist", kGray},
	{"gg4lhist", kBlack}
    };

    return mapping[histname];
}

int Mor18CoarseConfig::source_style(TString histname)
{
    std::map<TString, int> mapping = {
	{"ggHhist", 1001}, 
	{"VBFhist", 1001}, 
	{"WHhist", 1001},
	{"ZHhist", 1001},
	{"ttHhist", 1001},
	{"qq4lhist", 3244},
	{"gg4lhist", 3244}
    };

    return mapping[histname];
}

std::vector<TString> Mor18CoarseConfig::signal_source_labels()
{
    std::vector<TString> signal_source_labels = {
	"ggH",
	"VBF",
	"WH",
	"ZH",
	"ttH"
    };

    return signal_source_labels;
}

std::vector<int> Mor18CoarseConfig::signal_source_styles()
{
    std::vector<int> signal_source_styles = {
	1001,
	1001,
	1001,
	1001,
	1001
    };

    return signal_source_styles;
}

std::vector<TString> Mor18CoarseConfig::signal_hist_names()
{
    std::vector<TString> signal_hist_names = {
	"ggHhist",
	"VBFhist",
	"WHhist",
	"ZHhist",
	"ttHhist"
    };

    return signal_hist_names;
}

std::vector<int> Mor18CoarseConfig::signal_source_colors()
{
    std::vector<int> signal_source_colors = {
	kBlue - 9,
	kGreen - 6,
	kRed - 7,
	kYellow - 7,
	kCyan - 6
    };
    
    return signal_source_colors;
}

std::vector<TString> Mor18CoarseConfig::background_source_labels()
{
    std::vector<TString> background_source_labels = {
	"q#bar{q} #rightarrow ZZ #rightarrow 4l",
	"gg #rightarrow ZZ #rightarrow 4l"
    };

    return background_source_labels;
}

std::vector<int> Mor18CoarseConfig::background_source_styles()
{
    std::vector<int> background_source_styles = {
	3244,
	3244
    };

    return background_source_styles;
}

std::vector<TString> Mor18CoarseConfig::background_hist_names()
{
    std::vector<TString> background_hist_names = {
	"qq4lhist",
	"gg4lhist"
    };

    return background_hist_names;
}

std::vector<int> Mor18CoarseConfig::background_source_colors()
{
    std::vector<int> background_source_colors = {
	kBlack,
	kGray
    };
    
    return background_source_colors;
}

int Mor18CoarseConfig::hist_index(TString desc)
{
    std::map<TString, int> hist_index_vals = {
	{"ggHhist", 0},
	{"VBFhist", 1},
	{"WHhist", 2},
	{"ZHhist", 3},
	{"ttH", 4},
	{"qq4lhist", 5},
	{"gg4lhist", 6}
    };

    return hist_index_vals[desc];
}
