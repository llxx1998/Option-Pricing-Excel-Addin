#pragma once
#ifndef MathUtilities_H
#define MathUtilities_H

#include <cmath>
#include <vector>

double normalCDF(double x);

double Expectation(const std::vector<double>& v);

double Covariance(const std::vector<double>& v1, const std::vector<double>& v2);

double BlackScholesFormula(double S, double K, double r, double T, double sig, int iscall, double div, int isdivdiscrete);

#endif