#include <ZZAnalysis/AnalysisStep/test/classlib/include/Discriminant.h>

Discriminant::Discriminant(TString calib_dir)
{
    // don't use any nontrivial weights by default
    SetH1Weight(1.0);
    SetH0Weight(1.0);

    this -> calib_dir = calib_dir;
}

Discriminant::~Discriminant()
{ }

void Discriminant::SetDiscriminantName(TString disc_name)
{
    this -> disc_name = disc_name;
}

TString Discriminant::GetDiscriminantName()
{
    return disc_name;
}

std::vector<TString> Discriminant::GetNames()
{
    return names;
}
 
std::vector<std::function<bool(Tree*)>> Discriminant::GetCuts()
{
    return cuts;
}

std::vector<std::function<float(Tree*)>> Discriminant::GetDiscs()
{
    return discs;
}

void Discriminant::SetH1Source(EventStream* H1_source)
{
    this -> H1_source = H1_source;
}

void Discriminant::SetH0Source(EventStream* H0_source)
{
    this -> H0_source = H0_source;
}

void Discriminant::SetH1Weight(float weight)
{
    H1_weight = weight;
}

void Discriminant::SetH0Weight(float weight)
{
    H0_weight = weight;
}

float Discriminant::GetH1Weight()
{
    return H1_weight;
}

float Discriminant::GetH0Weight()
{
    return H0_weight;
}

EventStream* Discriminant::GetH1Source()
{
    return H1_source;
}

EventStream* Discriminant::GetH0Source()
{
    return H0_source;
}

void Discriminant::AddComponent(TString name, const std::function<bool(Tree*)> cut, const std::function<float(Tree*)> disc)
{
    names.push_back(name);
    cuts.push_back(cut);
    discs.push_back(disc);
   
    // check if a calibration file for this component exists
    TFile* calib_file = new TFile(calib_dir + name + "_calines.root");

    if(calib_file -> IsZombie())
    {
	// calibration does not yet exist
	std::cerr << "Warning: component " << name << " will be used without calibration" << std::endl;

	H1_calib_histos.push_back(NULL);
	H0_calib_histos.push_back(NULL);

	H1_calines.push_back(NULL);
	H0_calines.push_back(NULL);

	calibration_status.push_back(false);

	// have no KL corrections without calibration...
	//KL_corrections.push_back(0.0);
	D_01_KL.push_back(0.0);
	D_10_KL.push_back(0.0);
    }
    else
    {
	std::cout << "found calibration file for component " << name << "!" << std::endl;
	TSpline3* H1_spline = (TSpline3*)(calib_file -> Get("H1_distrib_smooth") -> Clone());
	TSpline3* H0_spline = (TSpline3*)(calib_file -> Get("H0_distrib_smooth") -> Clone());

	TH1F* H1_calib_histo = (TH1F*)(calib_file -> Get("H1_distrib") -> Clone());
	TH1F* H0_calib_histo = (TH1F*)(calib_file -> Get("H0_distrib") -> Clone());
	H1_calib_histo -> SetDirectory(0);
	H0_calib_histo -> SetDirectory(0);

	H1_calib_histos.push_back(H1_calib_histo);
	H0_calib_histos.push_back(H0_calib_histo);

	H1_calines.push_back(H1_spline);
	H0_calines.push_back(H0_spline);
	calibration_status.push_back(true);

	// compute also the KL corrections for each component, to be used later
	std::pair<float, float> KL_corr = ComputeKLCorrection(H1_calib_histo, H0_calib_histo);
	//KL_corrections.push_back(KL_corr);
	D_01_KL.push_back(KL_corr.first);
	D_10_KL.push_back(KL_corr.second);

	//std::cout << "computed KL correction for component " << name << ": " << KL_corr << std::endl;
    }

    calib_file -> Close();
}

std::pair<float, float> Discriminant::ComputeKLCorrection(TH1F* H1_calib_histo, TH1F* H0_calib_histo)
{
    // compute the (expected) Kullback-Leibler divergences, and take the difference between the two

    double D_10 = 0.0;
    double D_01 = 0.0;

    // if(H0_calib_histo -> GetSize() != H1_calib_histo -> GetSize())
    // {
    // 	std::cout << "Error: both H0 and H1 calibration histograms are supposed to have the same number of bins!";
    // 	return std::make_pair(0.0, 0.0);
    // }

    // important: both histograms can have different numbers of bins! (depends on the number of entries, i.e. the size of the training dataset)
    int number_samples = 5 * std::max(H1_calib_histo -> GetSize(), H0_calib_histo -> GetSize());
    
    std::cout << "using " << number_samples << " points for integration" << std::endl;

    float step_size = 1.0 / number_samples;
    for(float x = 0.0; x < 1.0; x += step_size)
    {
    	float p_H1 = std::max(H1_calib_histo -> Interpolate(x), 0.0);
    	float p_H0 = std::max(H0_calib_histo -> Interpolate(x), 0.0);

    	// float p_H1 = std::max(H1_calib_histo -> GetBinContent(H1_calib_histo -> FindBin(x)), 0.0);
    	// float p_H0 = std::max(H0_calib_histo -> GetBinContent(H0_calib_histo -> FindBin(x)), 0.0);

    	//std::cout << "bin " << i << ": " << p_H1 << " / " << p_H0 << " / " << H1_bin_width << " / " << H0_bin_width << std::endl;
	
    	if(p_H1 != 0.0 && p_H0 != 0.0)
    	{
    	    D_10 += p_H1 * TMath::Log(p_H1 / p_H0) * step_size;
    	    D_01 += p_H0 * TMath::Log(p_H0 / p_H1) * step_size;
    	}
    }

    // for(int i = 1; i <= H0_calib_histo -> GetSize() - 2; i++)
    // {
    // 	float p_H1 = std::max(H1_calib_histo -> GetBinContent(i), 0.0);
    // 	float p_H0 = std::max(H0_calib_histo -> GetBinContent(i), 0.0);
    // 	float H1_bin_width = std::max(H1_calib_histo -> GetBinWidth(i), 0.0);
    // 	float H0_bin_width = std::max(H0_calib_histo -> GetBinWidth(i), 0.0);

    // 	//std::cout << "bin " << i << ": " << p_H1 << " / " << p_H0 << " / " << H1_bin_width << " / " << H0_bin_width << std::endl;
	
    // 	if(p_H1 != 0.0 && p_H0 != 0.0)
    // 	{
    // 	    D_10 += p_H1 * TMath::Log(p_H1 / (std::max(p_H0, 0.00001f))) * H1_bin_width;
    // 	    D_01 += p_H0 * TMath::Log(p_H0 / (std::max(p_H1, 0.00001f))) * H0_bin_width;
    // 	}
    // }

    std::cout << "computed raw KL correction values: D_10 = " << D_10 << ", D_01 = " << D_01 << std::endl;

    // return the actual correction term, which is the difference between the two KL divergences
    //return 1.0 / 2.0 * (D_01 - D_10);
    return std::make_pair((float)D_01, (float)D_10);
}

float Discriminant::EvaluateLog(Tree* in)
{
    return TMath::Log(Evaluate(in));
}

float Discriminant::EvaluateKLCorrection(Tree* in, float H1_prior, float H0_prior)
{
    float retval = 0;

    // iterate through the list of components and check each of them until find a matching one
    for(auto tup: boost::combine(names, cuts, D_01_KL, D_10_KL))
    {
	std::function<bool(Tree*)> cut;
	TString name;
	float D_01;
	float D_10;
	boost::tie(name, cut, D_01, D_10) = tup;

	if(cut(in))
	{
	    //retval = KL_correction;

	    // this is for unequal *true* priors
	    //retval = H0_weight / (H0_weight + H1_weight) * D_01 - H1_weight / (H0_weight + H1_weight) * D_10;
	
	    // this is for equal *true* priors
	    //retval = 1.0 / 2.0 * (D_01 - D_10);
	
	    // in general
	    retval = H0_prior / (H0_prior + H1_prior) * D_01 - H1_prior / (H0_prior + H1_prior) * D_10;

	    break;
	}
    }
    
    return retval;
}

float Discriminant::Evaluate(Tree* in)
{
    float retval = 0;

    //std::cout << "n_jets = " << in -> nCleanedJetsPt30 << std::endl;

    // iterate through the list of components and check each of them
    for(auto tup: boost::combine(names, cuts, discs, H1_calines, H0_calines, H1_calib_histos, H0_calib_histos))
    {
	std::function<bool(Tree*)> cut;
	std::function<float(Tree*)> disc;
	TString name;
	TSpline3* H1_caline;
	TSpline3* H0_caline;
	TH1F* H1_calib_histo;
	TH1F* H0_calib_histo;
	boost::tie(name, cut, disc, H1_caline, H0_caline, H1_calib_histo, H0_calib_histo) = tup;

	if(cut(in))
	{
	    // found a matching component, now evaluate its attached discriminant
	    float raw_disc = disc(in);

	    // catch any NaN values here
	    if(!isfinite(raw_disc))
	    {
		return 0.0;
	    }

	    // for a calibrated discriminant, now evaluate the actual likelihood ratio (or an approximation thereof)

	    
	    //std::cout << "raw_disc = " << raw_disc << std::endl;

	    if((H1_calib_histo != NULL) && (H0_calib_histo != NULL))
	    {
		// don't use interpolation and smoothing at the moment
		//float calibrated_disc = (H1_calib_histo -> GetBinContent(H1_calib_histo -> FindBin(raw_disc))) / (0.00001 + H0_calib_histo -> GetBinContent(H0_calib_histo -> FindBin(raw_disc)));
		
		// this uses linear interpolation between two neighbouring bins
		float calibrated_disc = (H1_calib_histo -> Interpolate(raw_disc)) / (std::max(0.00001, H0_calib_histo -> Interpolate(raw_disc)));
		//float calibrated_interpolated_disc = (H1_caline -> Eval(raw_disc)) / (H0_caline -> Eval(raw_disc));

		//std::cout << calibrated_disc << " vs. " << calibrated_interpolated_disc << std::endl;

		retval = calibrated_disc * H1_weight / H0_weight; // apply the weights

		//std::cout << "LR = " << retval << std::endl;
	    }
	    else
	    {
		// if have no access to calibration information, return the raw (= uncalibrated) value instead!
		retval = raw_disc;
		//std::cout << "no_calib" << std::endl;
	    }
	    
	    break;
	}
    }

    return retval;
}
