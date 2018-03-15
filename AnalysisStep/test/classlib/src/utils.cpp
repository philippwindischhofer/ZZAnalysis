#include <ZZAnalysis/AnalysisStep/test/classlib/include/utils.h>

std::vector<float> get_category_sums(std::vector<TH1F*> hist_vec)
{
    int number_bins = hist_vec[0] -> GetSize() - 2; // don't take the over- and underflow bins
    std::vector<float> sumvec(number_bins);
    
    for(unsigned int bin = 0; bin < sumvec.size(); bin++)
    {
	sumvec[bin] = 0;

	for(unsigned int hist = 0; hist < hist_vec.size(); hist++)
	{
	    // this is the total number of events that ended up in each bin
	    sumvec[bin] += (hist_vec[hist] -> GetBinContent(bin + 1));
	}
    }

    return sumvec;
}

std::vector<float> renormalize_histograms(std::vector<TH1F*> hist_vec)
{
    std::vector<float> sumvec = get_category_sums(hist_vec);
 
    for(unsigned int bin = 0; bin < sumvec.size(); bin++)
    {
	for(unsigned int hist = 0; hist < hist_vec.size(); hist++)
	{
	    float cur_bin = hist_vec[hist] -> GetBinContent(bin + 1);
	    hist_vec[hist] -> SetBinContent(bin + 1, cur_bin / sumvec[bin]);
	}
    }

    return sumvec;
}

void save_histos(TString file, std::vector<TH1F*>& hist_vec)
{
    TFile* outfile = new TFile(file, "recreate");
    outfile -> cd();

    for(unsigned int hist = 0; hist < hist_vec.size(); hist++)
    {
	hist_vec[hist] -> Write();
    }

    outfile -> Close();
    delete outfile;
}

std::vector<TH1F*> read_histos(TString file, std::vector<TString> hist_names)
{
    std::vector<TH1F*> hist_vec;
    TFile* infile = TFile::Open(file);

    for(unsigned int hist = 0; hist < hist_names.size(); hist++)
    {
	TH1F* cur_hist = (TH1F*)(infile -> Get(hist_names[hist]));
	cur_hist -> SetDirectory(0);
	hist_vec.push_back(cur_hist);
    }

    infile -> Close();
    //delete infile;

    return hist_vec;
}

float get_total_events(TH1F* hist)
{
    return hist -> GetSumOfWeights();
}

/*
 * B_cat ... number of background events in this category
 * S_cat ... number of signal events in this category (i.e. correctly classified signal events)
 * S_tot ... total number of signal events, in all categories
 * -> for constructing the "sensitivity" region of the analysis:
 *   a ... number of sigmas needed for a discovery (e.g. 5)
 *   b ... number of sigmas used for constructing a confidence interval (e.g. 2)
 */
float get_punzi_purity(float B_cat, float S_cat, float S_tot, float a, float b)
{
    float eff = S_cat / S_tot; // efficiency for selecting the signal into this category

    float punzi = eff / (1/8*TMath::Power(a,2) + 9/13*TMath::Power(b,2) + a * TMath::Sqrt(B_cat) + b/2 * TMath::Sqrt(TMath::Power(b,2) + 4 * a * TMath::Sqrt(B_cat) + 4 * B_cat));

    return punzi;
}
