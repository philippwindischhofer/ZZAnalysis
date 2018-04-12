#ifndef ROCGenerator_h
#define ROCGenerator_h

// C++
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include <boost/range/combine.hpp>

class ROCGenerator
{
public:
    ROCGenerator();
    ~ROCGenerator();
    void GenerateROC(std::vector<float> disc_values, std::vector<bool> true_values, std::vector<float> weight_values, int num_pts);
    std::pair<float, float> GenerateROCPoint(std::vector<float> disc_values, std::vector<bool> true_values, std::vector<float> weight_values, float threshold);
    float* GetH0Efficiency();
    float* GetH1Efficiency();
    float GetAUC();
    int GetNumPoints();

private:
    std::vector<float> H0_efficiency;
    std::vector<float> H1_efficiency;
};

#endif
