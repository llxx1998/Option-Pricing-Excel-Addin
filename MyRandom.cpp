#include "MyRandom.h"

const double pi = 4.0 * atan(1.0);

double BoxMullerRandNorm() {
	double U1 = (rand() + 1.0) / (RAND_MAX + 1.0);
	double U2 = (rand() + 1.0) / (RAND_MAX + 1.0);
	return sqrt(-2.0 * log(U1)) * cos(2.0 * pi * U2);
}

std::vector<double> BoxMullerVector(int n) {
	std::vector<double> result;
	for (int i = 0; i < n; ++i) result.push_back(BoxMullerRandNorm());
	return result;
}