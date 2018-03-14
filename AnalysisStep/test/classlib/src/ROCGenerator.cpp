#include <ZZAnalysis/AnalysisStep/test/classlib/include/ROCGenerator.h>

ROCGenerator::ROCGenerator()
{ }

ROCGenerator::~ROCGenerator()
{ }

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

    for(double threshold = 0.0; threshold < 1.0; threshold += 1.0 / num_pts)
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
