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

double AsianOption::Payoff(double S) {
	if (iscall == 1) {
		return std::max(S - K, 0.0);
	}
	else {
		return std::max(K - S, 0.0);
	}
}

double AsianOption::DefaultMCPricer() {
	std::vector<double> S_vec(n, S_0);
	std::vector<double> prev_S_vec(n, S_0);
	std::vector<double> S_sum(n, 0.0);
	std::vector<double> z;

	for (int j = 0; j < m; ++j) {
		z = BoxMullerVector(n);
		for (int i = 0; i < n; ++i) {
			z[i] = z[i] * (sig * sqrt(T / m)); // sig * D_t * W(t)
			z[i] += (r - 0.5 * pow(sig, 2)) * T / m;
			z[i] = exp(z[i]);
			S_vec[i] = prev_S_vec[i] * z[i];
			S_sum[i] += S_vec[i];
			prev_S_vec[i] = S_vec[i];
		}
	}

	double D = exp(-r * T); // Discount factor

	for (int i = 0; i < n; ++i) {
		S_sum[i] /= m;
		S_sum[i] = Payoff(S_sum[i]);
		S_sum[i] = (S_sum[i] > 0? S_sum[i]: 0);
		S_sum[i] *= D;
	}
		
	double price = double(std::accumulate(S_sum.begin(), S_sum.end(), 0.0)) / n;

	return price;
}

double AsianOption::AnalyticGeoAvgPricer() {
	std::vector<double> time_series;
	for (int i = 0; i < m; ++i) time_series.push_back((1 / double(m)) * T * (i + 1));
	double T_bar = std::accumulate(time_series.begin(), time_series.end(), 0.0) / m;

	double s1_dot_s2 = 0;
	for (int i = 0; i < m; ++i) s1_dot_s2 += (2 * i + 1) * time_series[time_series.size() - i - 1];

	double sig2_bar, sig_bar, delta, d1, d2;
	sig2_bar = (pow(sig, 2) / (pow(m, 2) * T_bar)) * s1_dot_s2;
	sig_bar = sqrt(sig2_bar);
	delta = 0.5 * (pow(sig, 2) - sig2_bar);
	d1 = (log(S_0 / K) + (r - delta + 0.5 * sig2_bar) * T_bar) / (sig_bar * sqrt(T_bar));
	d2 = d1 - sig_bar * sqrt(T_bar);
	double n_d1 = normalCDF(d1);
	double n_d2 = normalCDF(d2);
	double price;
	if (iscall == 1) {
		price = exp(-delta * T_bar - r * (double(T) - T_bar)) * S_0 * n_d1 - exp(-r * double(T)) * K * n_d2;
	}
	else {
		price = - exp(-delta * T_bar - r * (double(T) - T_bar)) * S_0 * (1 - n_d1) + exp(-r * double(T)) * K * (1 - n_d2);
	}
	return price;
}


double AsianOption::AntitheticMCPricer() {
	std::vector<double> S_vec(n, S_0);
	std::vector<double> prev_S_vec(n, S_0);
	std::vector<double> S_sum(n, 0.0);
	std::vector<double> S_prod(n, 1.0);
	std::vector<double> z;

	for (int j = 0; j < m; ++j) {
		z = BoxMullerVector(n);
		for (int i = 0; i < n; ++i) {
			z[i] = z[i] * (sig * sqrt(T / m)); // sig * D_t * W(t)
			z[i] += (r - 0.5 * pow(sig, 2)) * T / m;
			z[i] = exp(z[i]);
			S_vec[i] = prev_S_vec[i] * z[i];
			S_sum[i] += S_vec[i];
			S_prod[i] *= S_vec[i];
			prev_S_vec[i] = S_vec[i];
		}
	}

	std::vector<double> C_vec;
	std::vector<double> Geo_C;

	double D = exp(-r * T); // Discount factor

	for (int i = 0; i < S_sum.size(); ++i) {
		C_vec.push_back(D * Payoff(S_sum[i] / double(m)));
		Geo_C.push_back(D * Payoff(pow(S_prod[i], 1 / m)));
	}

	double theo_price = AnalyticGeoAvgPricer();
	double b = Covariance(Geo_C, C_vec) / Covariance(Geo_C, Geo_C);

	for (int i = 0; i < C_vec.size(); ++i) C_vec[i] -= b * (Geo_C[i] - theo_price);
	return std::accumulate(C_vec.begin(), C_vec.end(), 0.0) / double(C_vec.size());
}

double AsianOption::Pricer() {
	if (method == "Antithetic") return AntitheticMCPricer();
	return DefaultMCPricer();
}