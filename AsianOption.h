#pragma once
#ifndef AsianOption_H
#define AsianOption_H

#include "Option.h"
#include "MyRandom.h"

class AsianOption: public Option {
public:
	int m;
	double K;
	int n;

private:
	AsianOption() : Option(), m(12), K(100), n(10000) {};

	AsianOption(int iscall, double r, double sig, double S_0, double T, int m, double K, int n) :
		Option(iscall, r, sig, S_0, T),
		m(m), K(K), n(n) {};

	~AsianOption();

	AsianOption(const AsianOption& source);

	AsianOption& operator = (const AsianOption& source);

	double Pricer(std::string method);

	double DefaultMCPricer();

	double AntitheticMCPricer();

	double AnalyticGeoAvgPricer();

	double Payoff(double S);
};

#endif // !AsianOption.H

