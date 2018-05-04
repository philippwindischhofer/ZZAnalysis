#include <ZZAnalysis/AnalysisStep/test/classlib/include/Mor18Config.h>

Mor18Config::Mor18Config(TString MCpath, float integrated_lumi, bool use_background, float mass_point) : Config(MCpath, integrated_lumi, use_background)
{ 
    // the needed data files depend on the chosen mass point (in principle, the file names corresponding to a certain mass point can be arbitrary, thus keep the conversion as general as possible!)
    std::map<float, TString> mass_point_translation;
    mass_point_translation.insert(std::make_pair(120.0, "120"));
    mass_point_translation.insert(std::make_pair(124.0, "124"));
    mass_point_translation.insert(std::make_pair(125.0, "125"));
    mass_point_translation.insert(std::make_pair(126.0, "126"));
    mass_point_translation.insert(std::make_pair(130.0, "130"));

    TString mass_point_suffix = mass_point_translation[mass_point];

    // set up the routing table for the data files
    routing.push_back(std::make_pair("ggH" + mass_point_suffix, new Routing(no_cut, "ggHhist")));
    routing.push_back(std::make_pair("VBFH" + mass_point_suffix, new Routing(no_cut, "VBFhist")));

    routing.push_back(std::make_pair("WplusH" + mass_point_suffix, new Routing(extraLeptons_0_cut, "WHXhist")));
    routing.push_back(std::make_pair("WminusH" + mass_point_suffix, new Routing(extraLeptons_0_cut, "WHXhist")));
    routing.push_back(std::make_pair("WplusH" + mass_point_suffix, new Routing(extraLeptons_1_cut, "WHlnuhist")));
    routing.push_back(std::make_pair("WminusH" + mass_point_suffix, new Routing(extraLeptons_1_cut, "WHlnuhist")));

    routing.push_back(std::make_pair("ZH" + mass_point_suffix, new Routing(extraNeutrinos_0_Leptons_0_cut, "ZHXhist")));
    routing.push_back(std::make_pair("ZH" + mass_point_suffix, new Routing(extraNeutrinos_2_cut, "ZHnunuhist")));
    routing.push_back(std::make_pair("ZH" + mass_point_suffix, new Routing(extraLeptons_2_cut, "ZH2lhist")));

    routing.push_back(std::make_pair("ttH" + mass_point_suffix, new Routing(extraLeptons_0_cut, "ttH0lhist")));
    routing.push_back(std::make_pair("ttH" + mass_point_suffix, new Routing(extraLeptons_1_cut, "ttH1lhist")));
    routing.push_back(std::make_pair("ttH" + mass_point_suffix, new Routing(extraLeptons_2_cut, "ttH2lhist")));

    if(use_background)
    {
	routing.push_back(std::make_pair("ZZTo4l", new Routing(no_cut, "qq4lhist")));

	routing.push_back(std::make_pair("ggTo2e2mu_Contin_MCFM701", new Routing(no_cut, "gg4lhist")));
	routing.push_back(std::make_pair("ggTo2e2tau_Contin_MCFM701", new Routing(no_cut, "gg4lhist")));
	routing.push_back(std::make_pair("ggTo2mu2tau_Contin_MCFM701", new Routing(no_cut, "gg4lhist")));
	routing.push_back(std::make_pair("ggTo4e_Contin_MCFM701", new Routing(no_cut, "gg4lhist")));
	routing.push_back(std::make_pair("ggTo4mu_Contin_MCFM701", new Routing(no_cut, "gg4lhist")));
	routing.push_back(std::make_pair("ggTo4tau_Contin_MCFM701", new Routing(no_cut, "gg4lhist")));

	routing.push_back(std::make_pair("AllData", new Routing(no_cut, "ZXhist")));
    }
}

std::vector<int> Mor18Config::categories()
{
    std::vector<int> categories = {
	UntaggedMor18,
	VBF1jTaggedMor18,
	VBF2jTaggedMor18,
	VHLeptTaggedMor18,
	VHHadrTaggedMor18,
	ttHLeptTaggedMor18,
	ttHHadrTaggedMor18,
	VHMETTaggedMor18
    };
    
    return categories;
}

TString Mor18Config::cat_label(int category)
{
    std::map<int, TString> mapping = {
	{UntaggedMor18, "Untagged"},
	{VBF1jTaggedMor18, "#splitline{VBF-1jet}{ tagged}"},
	{VBF2jTaggedMor18, "#splitline{VBF-2jet}{ tagged}"},
	{VHLeptTaggedMor18, "#splitline{VH-leptonic}{    tagged}"},
	{VHHadrTaggedMor18, "#splitline{VH-hadronic}{     tagged}"},
	{ttHLeptTaggedMor18, "#splitline{ttH leptonic}{   tagged}"},
	{ttHHadrTaggedMor18, "#splitline{ttH hadronic}{   tagged}"},
	{VHMETTaggedMor18, "#splitline{VH-E_{T}^{miss}}{ tagged}"}	
    };

    return mapping[category];
}

TString Mor18Config::cat_label_text(int category)
{
    std::map<int, TString> mapping = {
	{UntaggedMor18, "untagged"},
	{VBF1jTaggedMor18, "VBF-1j"},
	{VBF2jTaggedMor18, "VBF-2j"},
	{VHLeptTaggedMor18, "$VH$-lept"},
	{VHHadrTaggedMor18, "$VH$-hadr"},
	{ttHLeptTaggedMor18, "$ttH$-lept"},
	{ttHHadrTaggedMor18, "$ttH$-hadr"},
	{VHMETTaggedMor18, "$VH$-MET"}	
    };

    return mapping[category];
}


std::vector<TString> Mor18Config::abstract_categories()
{
    std::vector<TString> abs_categories = {
	"UntaggedMor18",
	"VBF1jTaggedMor18",
	"VBF2jTaggedMor18",
	"VHLeptTaggedMor18",
	"VHHadrTaggedMor18",
	"ttHLeptTaggedMor18",
	"ttHHadrTaggedMor18",
	"VHMETTaggedMor18"
    };

    return abs_categories;
}

TString Mor18Config::abs_cat_label(TString abs_cat)
{
   std::map<TString, TString> mapping = {
	{"UntaggedMor18", "Untagged"},
	{"VBF1jTaggedMor18", "#splitline{VBF-1jet}{ tagged}"},
	{"VBF2jTaggedMor18", "#splitline{VBF-2jet}{ tagged}"},
	{"VHLeptTaggedMor18", "#splitline{VH-leptonic}{    tagged}"},
	{"VHHadrTaggedMor18", "#splitline{VH-hadronic}{     tagged}"},
	{"ttHLeptTaggedMor18", "#splitline{ttH leptonic}{   tagged}"},
	{"ttHHadrTaggedMor18", "#splitline{ttH hadronic}{   tagged}"},
	{"VHMETTaggedMor18", "#splitline{VH-E_{T}^{miss}}{ tagged}"}	
    };

    return mapping[abs_cat];
}


TString Mor18Config::abs_cat_label_text(TString abs_cat)
{
   std::map<TString, TString> mapping = {
	{"UntaggedMor18", "Untagged"},
	{"VBF1jTaggedMor18", "VBF1j"},
	{"VBF2jTaggedMor18", "VBF2j"},
	{"VHLeptTaggedMor18", "VHlept"},
	{"VHHadrTaggedMor18", "VHhadr"},
	{"ttHLeptTaggedMor18", "ttHlept"},
	{"ttHHadrTaggedMor18", "ttHhadr"},
	{"VHMETTaggedMor18", "VHMET"}	
    };

    return mapping[abs_cat];
}

std::vector<SignalAssignment*> Mor18Config::signal_assignment()
{
    std::vector<SignalAssignment*> assignment = {
	new SignalAssignment("UntaggedMor18", "ggHhist", UntaggedMor18),

	new SignalAssignment("VBF1jTaggedMor18", "VBFhist", VBF1jTaggedMor18),
	new SignalAssignment("VBF2jTaggedMor18", "VBFhist", VBF2jTaggedMor18),

	new SignalAssignment("VHLeptTaggedMor18", "WHlnuhist", VHLeptTaggedMor18),
	new SignalAssignment("VHLeptTaggedMor18", "ZH2lhist", VHLeptTaggedMor18),

	new SignalAssignment("VHHadrTaggedMor18", "WHXhist", VHHadrTaggedMor18),
	new SignalAssignment("VHHadrTaggedMor18", "ZHXhist", VHHadrTaggedMor18),

	new SignalAssignment("ttHLeptTaggedMor18", "ttH1lhist", ttHLeptTaggedMor18),
	new SignalAssignment("ttHLeptTaggedMor18", "ttH2lhist", ttHLeptTaggedMor18),
	new SignalAssignment("ttHHadrTaggedMor18", "ttH0lhist", ttHHadrTaggedMor18),

	new SignalAssignment("VHMETTaggedMor18", "ZHnunuhist", VHMETTaggedMor18)
    };

    return assignment;
}

TString Mor18Config::storage_prefix()
{
    TString storage_prefix = "Mor18_";
    return storage_prefix;
}

TString Mor18Config::id()
{
    TString id = "Mor18";
    return id;
}
