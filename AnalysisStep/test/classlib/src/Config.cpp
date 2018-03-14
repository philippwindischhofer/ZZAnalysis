#include <ZZAnalysis/AnalysisStep/test/classlib/include/Config.h>

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
	{"ZZ4lhist", 0},
	{"gg4lhist", 1}
	};

    return hist_index_vals[desc];
}

std::vector<TString> Config::background_file_names()
{    
    std::vector<TString> background_file_names = {
	"ZZTo4l", 
	"DYJetsToLL_M50",
	"TTTo2L2Nu",
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
	"ZZTo4l", 
	"ggTo4l"
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
    //TString MC_path = "/data_CMS/cms/tsculac/CJLST_NTuples/";
    TString MC_path = "/home/llr/cms/wind/CJLST_NTuples/";
    return MC_path;
}

TString Config::MC_filename()
{
    TString MC_filename = "/ZZ4lAnalysis.root";
    return MC_filename;
}
