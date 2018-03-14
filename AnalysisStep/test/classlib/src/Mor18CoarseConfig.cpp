#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18CoarseConfig.h>

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
	"qqhist",
	"gghist"
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
	{"qqhist", 5},
	{"gghist", 6}
    };

    return hist_index_vals[desc];
}
