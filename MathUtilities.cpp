#include "MathUtilities.h"
#include <numeric>

double normalCDF(double x) 
{
    return std::erfc(-x / std::sqrt(2)) / 2;
}

double Expectation(const std::vector<double>& v) {
    return std::accumulate(v.begin(), v.end(), 0.0) / double(v.size());
}

double Covariance(const std::vector<double>& v1, const std::vector<double>& v2) {
    if (v1.size() != v2.size()) return -2;
    double x = 0;
    for (int i = 0; i < v1.size(); ++i) {
        x += v1[i] * v2[i] / double(v1.size());
    }
    return x - Expectation(v1) * Expectation(v2);
}