#include "MyRandom.h"

double BoxMullerRandNorm() {
	double result, x, y, sizeSquared;

	do {
		x = 2.0 * rand() / static_cast<double>(RAND_MAX) - 1;
		y = 2.0 * rand() / static_cast<double>(RAND_MAX) - 1;
		sizeSquared = pow(x, 2.0) + pow(y, 2.0);
	} while (sizeSquared >= 1.0);

	result = x * sqrt(-2.0 * log(sizeSquared) / sizeSquared);

	return result;
}

std::vector<double> BoxMullerVector(int n) {
	std::vector<double> result;
	for (int i = 0; i < n; ++i) result.push_back(BoxMullerRandNorm());
	return result;
}