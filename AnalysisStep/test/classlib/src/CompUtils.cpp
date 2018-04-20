#include <ZZAnalysis/AnalysisStep/test/classlib/include/CompUtils.h>

float CompUtils::compare_punzi(TString indir_a, TString indir_b, TString name_a, TString name_b, TString infile_name, TString hist_name, TString outdir, TString outfile_name, float zoom_scale, Config* conf)
{
    std::vector<TString> hist_name_vec;
    hist_name_vec.push_back(hist_name);
    
    // need to load now the Punzi histograms that should be sitting in these two directories, then can make their ratio or compare them in some other way
    std::vector<TH1F*> punzi_a = read_histos(indir_a + infile_name, hist_name_vec);
    std::vector<TH1F*> punzi_b = read_histos(indir_b + infile_name, hist_name_vec);

    // evaluate the metric to quantify the Punzi quality difference between the two
    unsigned int number_bins = punzi_a[0] -> GetSize() - 2;

    std::vector<float> delta_pi;

    // take all but the "untagged" bin into account for the metric!
    for(unsigned int bin = 1; bin < number_bins; bin++)
    {
	float punzi_diff = (punzi_a[0] -> GetBinContent(bin + 1)) / (punzi_b[0] -> GetBinContent(bin + 1)) - 1.0;
	delta_pi.push_back(punzi_diff);
    }

    // take the vector of Punzi improvements and feed them into the utility function to judge the power of this WP:
    float metric = cost_func(delta_pi, 8.0, 2);
    
    std::vector<TH1F*> comp_vec;
    comp_vec.push_back(punzi_a[0]);
    comp_vec[0] -> Divide(punzi_b[0]);

    // now can make the ratio plot
    CatPlotter plotter;
    plotter.Construct(comp_vec, conf -> ordered_abs_cat_labels(), std::vector<TString>(), std::vector<float>(), "Punzi purity ratio", conf -> lumi());
    plotter.DrawLabel(name_a + " vs. " + name_b);
    plotter.GetStack() -> SetMinimum(1 - zoom_scale);
    plotter.GetStack() -> SetMaximum(1 + zoom_scale);
    plotter.AddLine(1.0);
    plotter.Update();
    plotter.SaveAs(outdir + outfile_name + ".pdf");

    ConfigFileHandler* handler = new ConfigFileHandler(outdir + outfile_name + ".conf");
    handler -> AddSection("Punzi");
    int bin = 1;
    for(auto label: conf -> ordered_abs_cat_labels_text())
    {
	handler -> AddField(label, comp_vec[0] -> GetBinContent(bin));
	bin++;
    }

    handler -> SaveConfiguration();

    return metric;
}

