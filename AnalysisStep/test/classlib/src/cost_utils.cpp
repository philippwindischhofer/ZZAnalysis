#include <ZZAnalysis/AnalysisStep/test/classlib/include/cost_utils.h>

float weight_func(float delta_p, float alpha)
{
    float retval = 0.0;

    if(delta_p > 0.0)
    {
	retval = 1.0 / alpha * TMath::TanH(alpha * delta_p);
    }
    else
    {
	retval = delta_p;
    }

    retval -= 1.0 / alpha;

    return retval;
}

float cost_func(std::vector<float> delta_pi, float alpha, float m)
{
    float cost = 0.0;

    for(float cur_delta_pi: delta_pi)
    {
	cost += (TMath::Power(1.0 / alpha, m) - TMath::Power(weight_func(cur_delta_pi, alpha), m));
    }

    return cost;
}
