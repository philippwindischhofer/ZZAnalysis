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

std::vector<TString> Config::file_names()
{
    std::vector<TString> signal = signal_file_names();
    std::vector<TString> background = background_file_names();

    std::vector<TString> file_names(signal);
    file_names.insert(file_names.end(), background.begin(), background.end());

    return file_names;
}

std::vector<TString> Config::signal_file_names()
{
    std::vector<TString> signal_file_names = {"ggH125", "VBFH125", "WplusH125", "WminusH125", "ZH125", "ttH125", "bbH125", "tqH125"};
   
    return signal_file_names;
}

std::vector<TString> Config::signal_file_paths()
{
    std::vector<TString> file_names = signal_file_names();
    std::vector<TString> file_paths;

    for(auto& file: file_names)
    {
	file_paths.push_back(MC_path() + file + MC_filename());
    }

    return file_paths;
}

std::vector<TString> Config::background_file_paths()
{
    std::vector<TString> file_names = background_file_names();
    std::vector<TString> file_paths;

    for(auto& file: file_names)
    {
	file_paths.push_back(MC_path() + file + MC_filename());
    }

    return file_paths;
}

std::vector<TString> Config::file_paths()
{
    std::vector<TString> signal = signal_file_paths();
    std::vector<TString> background = background_file_paths();

    std::vector<TString> file_paths(signal);
    file_paths.insert(file_paths.end(), background.begin(), background.end());

    return file_paths;
}

std::vector<TString> Config::signal_source_labels()
{
    std::vector<TString> signal_source_labels = {
	"ggH", 
	"VBF", 
	"WH, W #rightarrow X",
	"WH, W #rightarrow l#nu", 
	"ZH, Z #rightarrow X",
	"ZH, Z #rightarrow #nu#nu",
	"ZH, Z #rightarrow 2l",
	"t#bar{t}H, t#bar{t} #rightarrow 0l + X",
	"t#bar{t}H, t#bar{t} #rightarrow 1l + X",
	"t#bar{t}H, t#bar{t} #rightarrow 2l + X"
    };
    
    return signal_source_labels;
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

std::vector<TString> Config::signal_source_labels_text()
{
    std::vector<TString> signal_source_labels_text = {
	"$ggH$",
	"VBF",
	"$WH, W \\rightarrow X$",
	"$WH, W \\rightarrow \\ell \\nu$",
	"$ZH, Z \\rightarrow X$",
	"$ZH, Z \\rightarrow \\nu \\nu$",
	"$ZH, Z \\rightarrow 2\\ell$",
	"$ttH, tt \\rightarrow 0\\ell$",
	"$ttH, tt \\rightarrow 1\\ell$",
	"$ttH, tt \\rightarrow 2\\ell$"
    };

    return signal_source_labels_text;
}

std::vector<TString> Config::signal_source_labels_merged_text()
{    
    std::vector<TString> signal_source_labels_merged_text = {
	"$ggH$", 
	"VBF", 
	"$WH$",
	"$ZH$",
	"$t\\bar{t}H$",
    };

    return signal_source_labels_merged_text;
}


std::vector<TString> Config::source_labels()
{
    std::vector<TString> signal = signal_source_labels();
    std::vector<TString> background = background_source_labels();

    std::vector<TString> source_labels(signal);
    source_labels.insert(source_labels.end(), background.begin(), background.end());

    return source_labels;
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

std::vector<int> Config::signal_source_styles()
{
    std::vector<int> signal_source_styles = {
	1001,
	1001,
	1001,
	1001,
	1001,
	1001,
	1001,
	1001,
	1001,
	1001
    };

    return signal_source_styles;
}

std::vector<int> Config::background_source_styles()
{
    std::vector<int> background_source_styles = {
	    3244,
	    3244
    };

    return background_source_styles;
}

std::vector<int> Config::source_styles()
{
    std::vector<int> signal = signal_source_styles();
    std::vector<int> background = background_source_styles();
    
    std::vector<int> source_styles(signal);
    source_styles.insert(source_styles.end(), background.begin(), background.end());
    
    return source_styles;        
}

std::vector<int> Config::source_colors()
{
    std::vector<int> signal = signal_source_colors();
    std::vector<int> background = background_source_colors();

    std::vector<int> source_colors(signal);
    source_colors.insert(source_colors.end(), background.begin(), background.end());

    return source_colors;    
}

std::vector<int> Config::signal_source_colors()
{    
    std::vector<int> signal_source_colors = {
	kBlue - 9, 
	kGreen - 6, 
	kRed - 7, 
	kRed - 6, 
	kYellow - 7, 
	kYellow - 3, 
	kYellow + 2,
	kCyan - 6,
	kCyan - 2,
	kCyan + 3
    };
    
    return signal_source_colors;
}

int Config::hist_index(TString desc)
{
    std::map<TString, int> hist_index_vals = {
	{"ggHhist", 0},
	{"VBFhist", 1},
	{"WHXhist", 2},
	{"WHlnuhist", 3},
	{"ZHXhist", 4},
	{"ZHnunuhist", 5},
	{"ZH2lhist", 6},
	{"ttH0lhist", 7},
	{"ttH1lhist", 8},
	{"ttH2lhist", 9},
	{"qq4lhist", 0},
	{"gg4lhist", 1}
	};

    return hist_index_vals[desc];
}

std::vector<TString> Config::background_file_names()
{    
    std::vector<TString> background_file_names = {
	"ZZTo4l", 
	"ggTo2e2mu_Contin_MCFM701",
	"ggTo2e2tau_Contin_MCFM701",
	"ggTo2mu2tau_Contin_MCFM701",
	"ggTo4e_Contin_MCFM701",
	"ggTo4mu_Contin_MCFM701",
	"ggTo4tau_Contin_MCFM701"
    };

    return background_file_names;
}

std::vector<TString> Config::background_source_labels()
{    
    std::vector<TString> background_source_labels = {
	"q#bar{q} #rightarrow ZZ #rightarrow 4l",
	"gg #rightarrow ZZ #rightarrow 4l"
    };

    return background_source_labels;
}

std::vector<TString> Config::background_source_labels_text()
{
    std::vector<TString> background_source_labels_text = {
	"$q\\bar{q} \\rightarrow ZZ \\rightarrow 4l$",
	"$gg \\rightarrow ZZ \\rightarrow 4l$"
    };

    return background_source_labels_text;
}

std::vector<TString> Config::background_hist_names()
{    
    std::vector<TString> background_hist_names = {
	"qq4lhist", 
	"gg4lhist"
    };

    return background_hist_names;
}

std::vector<int> Config::background_source_colors()
{
    std::vector<int> background_source_colors = {
	kBlack,
	kGray,
	kRed - 9,
	kRed - 6,
	kRed - 2,
	kRed + 3,
	kCyan - 6,
	kCyan - 2,
	kCyan + 3    
    };

    return background_source_colors;
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
