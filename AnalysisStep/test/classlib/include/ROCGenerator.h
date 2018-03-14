#ifndef ROCGenerator_h
#define ROCGenerator_h

// C++
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class ROCGenerator
{
public:
    ROCGenerator();
    ~ROCGenerator();
    void GenerateROC(std::vector<float> disc_values, std::vector<bool> true_values, std::vector<float> weight_values, int num_pts);
    float* GetH0Efficiency();
    float* GetH1Efficiency();
    int GetNumPoints();

private:
    std::vector<float> H0_efficiency;
    std::vector<float> H1_efficiency;
};

#endif
