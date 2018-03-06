#ifndef histo_settings_h
#define histo_settings_h

#ifdef Mor18

#define STORAGE_PREFIX "Mor18_"

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

// bin 0 is actually the underflow bin -> relevant content starts at bin 1 only
enum bin_index {
    untagged_bin = 1,
    VBF1jet_bin = 2,
    VBF2jet_bin = 3,
    VHleptonic_bin = 4,
    VHhadronic_bin = 5,
    ttH_leptonic_bin = 6,
    ttH_hadronic_bin = 7,
    VHMET_bin = 8
};

#endif // Mor18

#ifdef Mor17

#define STORAGE_PREFIX "Mor17_"

std::vector<TString> cat_labels = {
    "Untagged", 
    "#splitline{VBF-1jet}{ tagged}", 
    "#splitline{VBF-2jet}{ tagged}", 
    "#splitline{VH-leptonic}{    tagged}", 
    "#splitline{VH-hadronic}{     tagged}", 
    "ttH tagged", 
    "#splitline{VH-E_{T}^{miss}}{ tagged}"
};

std::vector<TString> cat_labels_text = {
    "untagged",
    "VBF-1j",
    "VBF-2j",
    "$VH$-lept",
    "$VH$-hadr",
    "$t\\bar{t}H$",
    "$VH$-MET"
};

// bin 0 is actually the underflow bin -> relevant content starts at bin 1 only
enum bin_index {
    untagged_bin = 1,
    VBF1jet_bin = 2,
    VBF2jet_bin = 3,
    VHleptonic_bin = 4,
    VHhadronic_bin = 5,
    ttH_bin = 6,
    VHMET_bin = 7
};

#endif // Mor17

// settings that are valid for both categorizations    
std::vector<TString> signal_file_names = {"ggH125", "VBFH125", "WplusH125", "WminusH125", "ZH125", "ttH125", "bbH125", "tqH125"};

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

std::vector<TString> signal_source_labels_merged_text = {
    "$ggH$", 
    "VBF", 
    "$WH$",
    "$ZH$",
    "$t\\bar{t}H$",
};

// names of the signal histograms
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

enum signal_hist_index {
    ggHhist = 0,
    VBFhist = 1,
    WHXhist = 2,
    WHlnuhist = 3,
    ZHXhist = 4,
    ZHnunuhist = 5,
    ZH2lhist = 6,
    ttH0lhist = 7,
    ttH1lhist = 8,
    ttH2lhist = 9
};

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

std::vector<int> background_source_colors = {
    kBlue - 9,
    kGreen - 6,
    kRed - 9,
    kRed - 6,
    kRed - 2,
    kRed + 3,
    kCyan - 6,
    kCyan - 2,
    kCyan + 3    
};

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

/*
enum background_hist_index {
    ZZ4lhist = 0,
    DYhist = 1,
    TThist = 2,
    gg4lhist = 3
};
*/

enum background_hist_index {
    ZZ4lhist = 0,
    gg4lhist = 1
};

std::vector<TString> background_hist_names = {
    "ZZTo4l", 
    //"DYToLL",
    //"TTTo2L2Nu",
    "ggTo4l"
};

std::vector<TString> background_source_labels = {
    "q#bar{q} #rightarrow ZZ #rightarrow 4l",
    //"Z/#gamma^{*} #rightarrow ll",
    //"t#bar{t} #rightarrow 2l2#nu",
    "gg #rightarrow ZZ #rightarrow 4l"
};

std::vector<TString> background_source_labels_text = {
    "$q\\bar{q} \\rightarrow ZZ \\rightarrow 4l$",
    //"$Z/\\gamma^{*} \\rightarrow ll$",
    //"$t\\bar{t} \\rightarrow 2l2\\nu$",
    "$gg \\rightarrow ZZ \\rightarrow 4l$"
};

#endif // histo_settings_h
