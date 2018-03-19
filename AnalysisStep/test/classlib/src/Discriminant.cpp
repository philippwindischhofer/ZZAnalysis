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
	calibration_status.push_back(false);
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
    }

    calib_file -> Close();
}

float Discriminant::Evaluate(Tree* in)
{
    float retval = 0;

    std::cout << "n_jets = " << in -> nCleanedJetsPt30 << std::endl;

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

	    // for a calibrated discriminant, now evaluate the actual likelihood ratio (or an approximation thereof)
	    //retval = (H1_caline -> Eval(raw_disc)) / (H0_caline -> Eval(raw_disc));
	    
	    std::cout << "raw_disc = " << raw_disc << std::endl;

	    // don't use interpolation at the moment
	    retval = (H1_calib_histo -> GetBinContent(H1_calib_histo -> FindBin(raw_disc))) / (H0_calib_histo -> GetBinContent(H0_calib_histo -> FindBin(raw_disc)));
	    retval = retval * H1_weight / H0_weight; // apply the weights

	    //std::cout << "LR = " << retval << std::endl;
	    
	    break;
	}
    }

    return retval;
}
