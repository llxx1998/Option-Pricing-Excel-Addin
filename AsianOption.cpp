#include "AsianOption.h"
#include <unordered_map>
#include <numeric>
#include <algorithm>

AsianOption::~AsianOption() {
	// Destructor
}

AsianOption::AsianOption(const AsianOption& source): Option(source) {
	m = source.m;
	K = source.K;
	n = source.n;
}

AsianOption& AsianOption::operator = (const AsianOption& source) {
	if (this == &source) return *this;

	Option::operator=(source);

	m = source.m;
	K = source.K;
	n = source.n;

	return *this;
}

double AsianOption::DefaultMCPricer() {
	std::vector<double> S_vec(n, S_0);
	std::vector<double> prev_S_vec(n, S_0);
	std::vector<double> S_sum(n, 0.0);
	std::vector<double> z;

	for (int i = 0; i < m; ++i) {
		z = BoxMullerVector(n);
		for (int i = 0; i < n; ++i) {
			z[i] = z[i] * (sig * sqrt(T / m)); // sig * D_t * W(t)
			z[i] += (r - 0.5 * sqrt(sig)) * T / m;
			z[i] = exp(z[i]);
			S_vec[i] = prev_S_vec[i] * z[i];
			S_sum[i] += S_vec[i];
			prev_S_vec[i] = S_vec[i];
		}
	}

	double D = exp(-r * T); // Discount factor

	for (int i = 0; i < n; ++i) {
		S_sum[i] /= m;
		S_sum[i] -= K;
		S_sum[i] = (S_sum[i] > 0? S_sum[i]: 0);
		S_sum[i] *= D;
	}
		
	double price = double(std::accumulate(S_sum.begin(), S_sum.end(), 0)) / n;

	return price;
}

double AsianOption::AnalyticGeoAvgPricer() {
	std::vector<double> time_series;
	for (int i = 0; i < m; ++i) time_series.push_back((1 / double(m)) * T * (i + 1));
	std::vector<double> T_bar(m, 0.0);
	std::partial_sum(time_series.begin(), time_series.end(), T_bar);
	for (int i = 0; i < T_bar.size(); ++i) T_bar[i] /= m;

	auto series1 = time_series;
	std::reverse(series1.begin(), series1.end());
	std::vector<double> series2;
	return 0;
}

double AsianOption::AntitheticMCPricer() {

}

double AsianOption::Pricer(std::string method) {
	if (method == "Antithetic") return AntitheticMCPricer();
	return DefaultMCPricer();
}