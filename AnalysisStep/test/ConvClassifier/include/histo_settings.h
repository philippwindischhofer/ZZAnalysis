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
    "VBF1Jet",
    "VBF2Jet",
    "VH-leptonic",
    "VH-hadronic",
    "ttH-leptonic",
    "ttH-hadronic",
    "VHMET"
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
    "VBF1Jet",
    "VBF2Jet",
    "VH-leptonic",
    "VH-hadronic",
    "ttH",
    "VHMET"
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
    "ggH",
    "VBF",
    "WH, W->X",
    "WH, W->l nu",
    "ZH, Z->X",
    "ZH, Z->nu nu",
    "ZH, Z->2l",
    "ttH, tt->0l",
    "ttH, tt->1l",
    "ttH, tt->2l"
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
    "ttH2lhist"};

enum hist_index {
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

std::vector<TString> background_hist_names = {
    "ZZTo4l", 
    "DYToLL",
    "TTTo2L2Nu",
    "ggTo2e2mu",
    "ggTo2e2tau",
    "ggTo2mu2tau",
    "ggTo4e",
    "ggTo4mu",
    "ggTo4tau"
};

std::vector<TString> background_source_labels = {
    "ZZ #rightarrow 4l",
    "Z/#gamma^{*} #rightarrow ll",
    "t#bar{t} #rightarrow 2l2#nu",
    "gg #rightarrow ZZ #rightarrow 2e2#mu",
    "gg #rightarrow ZZ #rightarrow 2e2#tau",
    "gg #rightarrow ZZ #rightarrow 2#mu2#tau",
    "gg #rightarrow ZZ #rightarrow 4e",
    "gg #rightarrow ZZ #rightarrow 4#mu",
    "gg #rightarrow ZZ #rightarrow 4#tau"
};

std::vector<TString> background_source_labels_text = {
    "ZZ #rightarrow 4l",
    "Z/#gamma^{*} #rightarrow ll",
    "t#bar{t} #rightarrow 2l2#nu",
    "gg #rightarrow ZZ #rightarrow 2e2#mu",
    "gg #rightarrow ZZ #rightarrow 2e2#tau",
    "gg #rightarrow ZZ #rightarrow 2#mu2#tau",
    "gg #rightarrow ZZ #rightarrow 4e",
    "gg #rightarrow ZZ #rightarrow 4#mu",
    "gg #rightarrow ZZ #rightarrow 4#tau"
};

#endif // histo_settings_h
