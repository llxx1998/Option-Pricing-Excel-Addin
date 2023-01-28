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

double BlackScholesFormula(double S, double K, double r, double T, double sig, int iscall, int div, int isdivdiscrete = 0) {
    double adj_S;
    if (isdivdiscrete) {
        // cash amount
        adj_S = S - div;
    }
    else {
        // ratio amount
        adj_S = S * exp(- div * T);
    }
    double d1, d2;
    d1 = (log(adj_S / K) + (r + 0.5 * pow(sig, 2)) * T) / (sig * sqrt(T));
    d2 = d1 - sig * sqrt(T);

    if (iscall == 1) {
        return adj_S * normalCDF(d1) - K * exp(-r * T) * normalCDF(d2);
    }
    else {
        return - adj_S *(1 - normalCDF(d1)) + K * exp(-r * T) * (1 - normalCDF(d2));
    }
}