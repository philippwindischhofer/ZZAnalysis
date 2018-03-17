#include <ZZAnalysis/AnalysisStep/test/classlib/include/Config.h>

Config::Config()
{
    // set up the routing table
    routing.push_back(std::make_pair("ggH125", new Routing(no_cut, "ggHhist")));
    routing.push_back(std::make_pair("VBFH125", new Routing(no_cut, "VBFhist")));

    routing.push_back(std::make_pair("WplusH125", new Routing(extraLeptons_0_cut, "WHXhist")));
    routing.push_back(std::make_pair("WminusH125", new Routing(extraLeptons_0_cut, "WHXhist")));
    routing.push_back(std::make_pair("WplusH125", new Routing(extraLeptons_1_cut, "WHlnuhist")));
    routing.push_back(std::make_pair("WminusH125", new Routing(extraLeptons_1_cut, "WHlnuhist")));

    routing.push_back(std::make_pair("ZH125", new Routing(extraNeutrinos_0_Leptons_0_cut, "ZHXhist")));
    routing.push_back(std::make_pair("ZH125", new Routing(extraNeutrinos_2_cut, "ZHnunuhist")));
    routing.push_back(std::make_pair("ZH125", new Routing(extraLeptons_2_cut, "ZH2lhist")));

    routing.push_back(std::make_pair("ttH125", new Routing(extraLeptons_0_cut, "ttH0lhist")));
    routing.push_back(std::make_pair("ttH125", new Routing(extraLeptons_1_cut, "ttH1lhist")));
    routing.push_back(std::make_pair("ttH125", new Routing(extraLeptons_2_cut, "ttH2lhist")));

    routing.push_back(std::make_pair("ZZTo4l", new Routing(no_cut, "qq4lhist")));

    routing.push_back(std::make_pair("ggTo2e2mu_Contin_MCFM701", new Routing(no_cut, "gg4lhist")));
    routing.push_back(std::make_pair("ggTo2e2tau_Contin_MCFM701", new Routing(no_cut, "gg4lhist")));
    routing.push_back(std::make_pair("ggTo2mu2tau_Contin_MCFM701", new Routing(no_cut, "gg4lhist")));
    routing.push_back(std::make_pair("ggTo4e_Contin_MCFM701", new Routing(no_cut, "gg4lhist")));
    routing.push_back(std::make_pair("ggTo4mu_Contin_MCFM701", new Routing(no_cut, "gg4lhist")));
    routing.push_back(std::make_pair("ggTo4tau_Contin_MCFM701", new Routing(no_cut, "gg4lhist")));
}

std::vector<TString> Config::hist_names()
{
    std::vector<TString> signal = signal_hist_names();
    std::vector<TString> background = background_hist_names();

    std::vector<TString> hist_names(signal);
    hist_names.insert(hist_names.end(), background.begin(), background.end());

    return hist_names;    
}

std::vector<TString> Config::signal_hist_names()
{
    std::vector<TString> signal_hist_names = {
	"ggHhist", 
	"VBFhist",
	"WHXhist", 
	"WHlnuhist", 
	"ZHXhist", 
	"ZHnunuhist",
	"ZH2lhist", 
	"ttH0lhist", 
	"ttH1lhist", 
	"ttH2lhist"
    };
    
    return signal_hist_names;
}

std::vector<TString> Config::background_hist_names()
{    
    std::vector<TString> background_hist_names = {
	"qq4lhist", 
	"gg4lhist"
    };

    return background_hist_names;
}

TString Config::source_label(TString histname)
{
    std::map<TString, TString> mapping = {
	{"ggHhist", "ggH"}, 
	{"VBFhist", "VBF"}, 
	{"WHXhist", "WH, W #rightarrow X"},
	{"WHlnuhist", "WH, W #rightarrow l#nu"}, 
	{"ZHXhist", "ZH, Z #rightarrow X"},
	{"ZHnunuhist", "ZH, Z #rightarrow #nu#nu"},
	{"ZH2lhist", "ZH, Z #rightarrow 2l"},
	{"ttH0lhist", "t#bar{t}H, t#bar{t} #rightarrow 0l + X"},
	{"ttH1lhist", "t#bar{t}H, t#bar{t} #rightarrow 1l + X"},
	{"ttH2lhist", "t#bar{t}H, t#bar{t} #rightarrow 2l + X"},
	{"qq4lhist", "q#bar{q} #rightarrow ZZ #rightarrow 4l"},
	{"gg4lhist","gg #rightarrow ZZ #rightarrow 4l"}
    };

    return mapping[histname];
}

int Config::source_color(TString histname)
{
    std::map<TString, int> mapping = {
	{"ggHhist", kBlue - 9}, 
	{"VBFhist", kGreen - 6}, 
	{"WHXhist", kRed - 7},
	{"WHlnuhist", kRed - 6}, 
	{"ZHXhist", kYellow - 7},
	{"ZHnunuhist", kYellow - 3},
	{"ZH2lhist", kYellow + 2},
	{"ttH0lhist", kCyan - 6},
	{"ttH1lhist", kCyan - 2},
	{"ttH2lhist", kCyan + 3},
	{"qq4lhist", kGray},
	{"gg4lhist", kBlack}
    };

    return mapping[histname];
}

int Config::source_style(TString histname)
{
    std::map<TString, int> mapping = {
	{"ggHhist", 1001}, 
	{"VBFhist", 1001}, 
	{"WHXhist", 1001},
	{"WHlnuhist", 1001}, 
	{"ZHXhist", 1001},
	{"ZHnunuhist", 1001},
	{"ZH2lhist", 1001},
	{"ttH0lhist", 1001},
	{"ttH1lhist", 1001},
	{"ttH2lhist", 1001},
	{"qq4lhist", 3244},
	{"gg4lhist", 3244}
    };

    return mapping[histname];
}

// generates an empty set of histograms, as specified by this config class, and by the external info as to the number of bins
std::map<TString, TH1F*> Config::generate_empty_histmap(int number_bins, float lower, float upper)
{
    std::map<TString, TH1F*> histmap;
    std::vector<TString> names = hist_names();
    
    for(auto& name: names)
    {
	TH1F* cur = new TH1F(name, name, number_bins, lower, upper);
	cur -> SetFillColor(source_color(name));
	cur -> SetLineColor(source_color(name));
	cur -> SetFillStyle(source_style(name));

	histmap.insert(std::make_pair(name, cur));
    }

    return histmap;
}

int Config::abs_cat_number(TString category)
{
    std::vector<TString> cats = abstract_categories();

    // histogram bins in ROOT are 1-indexed!!
    return std::find(cats.begin(), cats.end(), category) - cats.begin();
}

int Config::bin_number(int category)
{
    std::vector<int> cats = categories();

    // TODO: add out-of-bounds checking!

    // histogram bins in ROOT are 1-indexed!!
    return std::find(cats.begin(), cats.end(), category) - cats.begin() + 1;
}

std::vector<TString> Config::ordered_cat_labels()
{
    std::vector<int> cats = categories();
    std::vector<TString> ordered_labels;

    for(auto cat: cats)
    {
	ordered_labels.push_back(cat_label(cat));
    }

    return ordered_labels;
}

std::vector<TString> Config::ordered_abs_cat_labels()
{
    std::vector<TString> cats = abstract_categories();
    std::vector<TString> ordered_labels;

    for(auto cat: cats)
    {
	ordered_labels.push_back(abs_cat_label(cat));
    }

    return ordered_labels;
}

// need to make sure that every key-value combination occurs only once, but each key can occur multiple times!
std::vector<std::pair<TString, int>> Config::bin_assignment()
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

std::vector<std::pair<TString, TString>> Config::histogram_assignment()
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

float Config::lumi()
{
    float lumi = 35.9;
    return lumi;
}

TString Config::MC_path()
{
    TString MC_path = "/data_CMS/cms/tsculac/CJLST_NTuples/";
    //TString MC_path = "/home/llr/cms/wind/CJLST_NTuples/";
    return MC_path;
}

TString Config::MC_filename()
{
    TString MC_filename = "/ZZ4lAnalysis.root";
    return MC_filename;
}
