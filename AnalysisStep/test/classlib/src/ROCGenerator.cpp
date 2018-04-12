#include <ZZAnalysis/AnalysisStep/test/classlib/include/ROCGenerator.h>

ROCGenerator::ROCGenerator()
{ }

ROCGenerator::~ROCGenerator()
{ }

std::pair<float, float> ROCGenerator::GenerateROCPoint(std::vector<float> disc_values, std::vector<bool> true_values, std::vector<float> weight_values, float threshold)
{
    float H1_eff = 0.0;
    float H0_eff = 0.0;

    float H1_sum = 0.0;
    float H0_sum = 0.0;

    for(unsigned int i = 0; i < disc_values.size(); i++)
    {
	if(true_values.at(i))
	{
	    H1_sum += weight_values.at(i);
	}
	else
	{
	    H0_sum += weight_values.at(i);
	}
    }

    // find the range of the discriminating variable
    auto min_el = std::min_element(std::begin(disc_values), std::end(disc_values));
    auto max_el = std::max_element(std::begin(disc_values), std::end(disc_values));

    std::cout << "min. value of the discriminating variable = " << *min_el << std::endl;
    std::cout << "max. value of the discriminating variable = " << *max_el << std::endl;

    for(unsigned int i = 0; i < disc_values.size(); i++)
    {
	if(disc_values.at(i) > threshold)
	{
	    if(true_values.at(i))
	    {
		H1_eff += weight_values.at(i);
	    }
	    else
	    {
		H0_eff += weight_values.at(i);
	    }
	}
    }
    
    H1_eff /= H1_sum;
    H0_eff /= H0_sum;

    return std::make_pair(H1_eff, H0_eff);
}

void ROCGenerator::GenerateROC(std::vector<float> disc_values, std::vector<bool> true_values, std::vector<float> weight_values, int num_pts)
{
    // clear the old ones
    H0_efficiency.clear();
    H1_efficiency.clear();

    float H1_sum = 0.0;
    float H0_sum = 0.0;

    for(unsigned int i = 0; i < disc_values.size(); i++)
    {
	if(true_values.at(i))
	{
	    H1_sum += weight_values.at(i);
	}
	else
	{
	    H0_sum += weight_values.at(i);
	}
    }

    // find the range of the discriminating variable
    auto min_el = std::min_element(std::begin(disc_values), std::end(disc_values));
    auto max_el = std::max_element(std::begin(disc_values), std::end(disc_values));

    std::cout << "min. value of the discriminating variable = " << *min_el << std::endl;
    std::cout << "max. value of the discriminating variable = " << *max_el << std::endl;

    for(double threshold = *min_el; threshold < *max_el; threshold += (*max_el - *min_el) / num_pts)
    {
	float cur_H1_eff = 0.0;
	float cur_H0_eff = 0.0;

	for(unsigned int i = 0; i < disc_values.size(); i++)
	{
	    if(disc_values.at(i) > threshold)
	    {
		if(true_values.at(i))
		{
		    cur_H1_eff += weight_values.at(i);
		}
		else
		{
		    cur_H0_eff += weight_values.at(i);
		}
	    }
	}

	cur_H1_eff /= H1_sum;
	cur_H0_eff /= H0_sum;

	H1_efficiency.push_back(cur_H1_eff);
	H0_efficiency.push_back(cur_H0_eff);
    }
    
    H1_efficiency.push_back(0.0);
    H0_efficiency.push_back(0.0);
}

float ROCGenerator::GetAUC()
{
    // first, build the combined data that is contained in the ROC
    std::vector<std::pair<float, float>> xy_data;
    for(auto tup: boost::combine(H0_efficiency, H1_efficiency))
    {
	float H0_eff;
	float H1_eff;
	boost::tie(H0_eff, H1_eff) = tup;

	xy_data.push_back(std::make_pair(H0_eff, H1_eff));
    }

    // then, sort everything in order of increasing H0 efficiency (which is the x-axis label)
    std::sort(xy_data.begin(), xy_data.end(), [&](std::pair<float, float>& a, std::pair<float, float>& b){return a.first < b.first;});

    // then, can easily integrate it
    float auc = 0.0;

    for(unsigned int i = 0; i < xy_data.size() - 1; i++)
    {
	auc += 0.5 * (xy_data[i].second + xy_data[i+1].second) * (xy_data[i+1].first - xy_data[i].first);
    }

    return auc;
}

float* ROCGenerator::GetH0Efficiency()
{
    return H0_efficiency.data();
}

float* ROCGenerator::GetH1Efficiency()
{
    return H1_efficiency.data();
}

int ROCGenerator::GetNumPoints()
{
    return H1_efficiency.size();
}
