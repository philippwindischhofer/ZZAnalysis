#ifndef histo_settings_h
#define histo_settings_h

std::vector<TString> file_names = {
    "ggH125",
    "VBFH125",
    "WplusH125",
    "WminusH125",
    "ZH125",
    "ttH125",
    "ZZTo4l", 
    "ggTo2e2mu_Contin_MCFM701",
    "ggTo2e2tau_Contin_MCFM701",
    "ggTo2mu2tau_Contin_MCFM701",
    "ggTo4e_Contin_MCFM701",
    "ggTo4mu_Contin_MCFM701",
    "ggTo4tau_Contin_MCFM701"
};

std::vector<TString> hist_names = {
    "ggHhist",
    "VBFhist",
    "WHhist",
    "ZHhist",
    "ttHhist",
    "qqTo4lhist",
    "ggTo4lhist"
};

std::vector<TString> source_labels = {
    "ggH",
    "VBF",
    "WH",
    "ZH",
    "ttH",
    "q#bar{q} #rightarrow ZZ #rightarrow 4l",
    "gg #rightarrow ZZ #rightarrow 4l"
};

std::vector<int> source_styles = {
    1001,
    1001,
    1001,
    1001,
    1001,
    3244,
    3244
};

enum hist_index {
    ggHhist = 0,
    VBFhist = 1,
    WHhist = 2,
    ZHhist = 3,
    ttHhist = 4,
    qqhist = 5,
    gghist = 6
};

std::vector<int> source_colors = {
    kBlue - 9,
    kGreen - 6,
    kRed - 7,
    kYellow - 7,
    kCyan - 6,
    kBlack,
    kGray
    //kBlue - 9,
    //kGreen - 6
};

#endif
