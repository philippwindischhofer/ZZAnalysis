#include <ZZAnalysis/AnalysisStep/test/classlib/include/Discriminant.h>

Discriminant::Discriminant(TString calib_dir)
{
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

	H1_calines.push_back(H1_spline);
	H0_calines.push_back(H0_spline);
	calibration_status.push_back(true);

    }

    calib_file -> Close();
}

float Discriminant::Evaluate(Tree* in)
{
    float retval = 0;

    // iterate through the list of components and check each of them
    for(auto tup: boost::combine(names, cuts, discs))
    {
	std::function<bool(Tree*)> cut;
	std::function<float(Tree*)> disc;
	TString name;
	boost::tie(name, cut, disc) = tup;

	if(cut(in))
	{
	    // found a matching component, now evaluate its attached discriminant
	    retval = disc(in);
	    break;

	    std::cout << name << " matched" << std::endl;
	}
    }

    return retval;
}
