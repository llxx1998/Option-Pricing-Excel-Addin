#include "AmericanOption.h"
#include <utility>
#include <vector>


AhGenerator::AhGenerator(int n_x, double delta_t, double sig, double r) {
	for (int i = 0; i < n_x; ++i) {
		a.push_back(-0.5 * delta_t * pow(double(i) * sig, 2));
		b.push_back(1.0 + delta_t * (r + r * double(i) + pow(double(i) * sig, 2)));
		c.push_back(delta_t * (-r * double(i) - 0.5 * pow(sig * double(i), 2)));
	}
	a.pop_front();
	c.pop_back();
}

AmericanOption::~AmericanOption() {
	// Destructor
}

AmericanOption::AmericanOption(const AmericanOption& source):Option(source) {
	K = source.K;
	mesh_size = source.mesh_size;
	method = source.method;
	R = source.R;
}

AmericanOption& AmericanOption::operator = (const AmericanOption& source) {
	if (this == &source) return *this;
	Option::operator=(source);
	K = source.K;
	mesh_size = source.mesh_size;
	method = source.method;
	R = source.R;
	return *this;
}

double AmericanOption::Payoff(double S) {
	if (iscall == 1) {
		return std::max(S - K, 0.0);
	}
	else {
		return std::max(K - S, 0.0);
	}
}

std::vector<double> TDMASolver(const AhGenerator& ahg, std::vector<double>& d) {
	std::vector<double> a(ahg.a.begin(), ahg.a.end());
	std::vector<double> b(ahg.b.begin(), ahg.b.end());
	std::vector<double> c(ahg.c.begin(), ahg.c.end());
	auto n = d.size();
	c[0] = c[0] / b[0];
	d[0] = d[0] / b[0];

	for (size_t i = 1; i < size_t(n) - 1; ++i) {
		double temp = b[i] - a[i - 1] * c[i - 1];
		c[i] = c[i] / temp;
		d[i] = (d[i] - a[i - 1] * d[i - 1]) / temp;
	}

	d[n - 1] = (d[n - 1] - a[n - 2] * d[n - 2]) / (b[n - 1] - a[n - 2] * c[n - 2]);
	std::vector<double> x = d;
	for (auto i = n - 2; i > -1; --i) {
		x[i] = d[i] - c[i] * x[i + 1];
	}
	return x;
}

double AmericanOption::PDEAmericanOptionPricer() {
	int n_t = mesh_size, n_x = mesh_size;
	std::vector<std::vector<double>> mesh(n_t + 1, std::vector<double>(n_x + 1, 0.0));
	std::vector<std::pair<double, double>> d;
	double delta_t = T / double(n_t), delta_x = R / double(n_x);
	AhGenerator ahg(n_x, delta_t, sig, r);
	std::vector<double> exercise_now;

	// initialize
	for (int j = 0; j < n_x + 1; ++j) 
		mesh[n_t][j] = std::max(K - double(j) * delta_x, 0.0), 
		exercise_now.push_back(std::max(K - double(j) * delta_x, 0.0));
	
	for (int n = n_t - 1; n > -1; --n) {
		std::vector<double> Bnh;
		for (int i = 1; i < n_x; ++i) Bnh.push_back(mesh[n + 1][i]);
		Bnh[0] -= -0.5 * pow(sig, 2) * delta_t * K * exp(-r * double(n) * delta_t);
		std::vector<double> num = TDMASolver(ahg, Bnh);
		double bd = 0.0;
		for (int i = 0; i < num.size(); ++i) {
			if (num[i] < exercise_now[i + 1]) {
				num[i] = exercise_now[i + 1];
				bd = (i + 1) * delta_x;
			}
		}
		d.push_back(std::make_pair(double(n) * delta_t, bd));
		for (int i = 1; i < n_x; ++i) mesh[n][i] = num[i - 1];
	}
	for (int i = 0; i < n_t + 1; ++i) mesh[i][0] = K;
	for (int i = 0; i < n_t + 1; ++i) mesh[i][n_x] = 0;
	int pos = static_cast<int>(std::max((S_0 / delta_x), 0.0));
	return mesh[0][pos];
}

double AmericanOption::Pricer() {
	if (method == "PDE") {
		if (iscall == 0) {
			return PDEAmericanOptionPricer();
		}
		else {
			// given no dividend, not exercise early, price it as an European Option
			// // currently just call bsm function in math utilities
			return BlackScholesFormula(S_0, K, r, T, sig, iscall, 0, 0);
		}
	}
	if (method == "Binomial") {
		// upcoming feature
		return 0.0;
	}
	return 0.0;
}