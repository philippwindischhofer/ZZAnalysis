#include <ZZAnalysis/AnalysisStep/test/Comp/include/comp_utils.h>
#include <ZZAnalysis/AnalysisStep/test/Comp/include/histo_settings.h>

#ifdef Mor18

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

void compare_punzi(TString indir_a, TString indir_b, TString name_a, TString name_b, TString infile_name, TString hist_name, TString outdir, TString outfile_name, float zoom_scale)
{
    std::vector<TString> hist_name_vec;
    hist_name_vec.push_back(hist_name);
    
    // need to load now the Punzi histograms that should be sitting in these two directories, then can make their ratio or compare them in some other way
    std::vector<TH1F*> punzi_a = read_histos(indir_a + infile_name, hist_name_vec);
    std::vector<TH1F*> punzi_b = read_histos(indir_b + infile_name, hist_name_vec);

    std::vector<TH1F*> comp_vec;
    comp_vec.push_back(punzi_a[0]);
    comp_vec[0] -> Divide(punzi_b[0]);

    // now can make the ratio
    CatPlotter plotter;
    plotter.Construct(comp_vec, cat_labels, std::vector<TString>(), std::vector<float>(), "Punzi purity ratio", lumi);
    plotter.DrawLabel(name_a + " vs. " + name_b);
    plotter.GetStack() -> SetMinimum(1 - zoom_scale);
    plotter.GetStack() -> SetMaximum(1 + zoom_scale);
    plotter.AddLine(1.0);
    plotter.Update();
    plotter.SaveAs(outdir + outfile_name + ".pdf");
}

