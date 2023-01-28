#pragma once
#ifndef AmericanOption_H
#define AmericanOption_H

#include "Option.h"
#include "MathUtilities.h"
#include <deque>

class AmericanOption : public Option {
public:
	double K;
	int mesh_size;
	std::string method;
	double R;

	AmericanOption() : K(100), mesh_size(1024), method("PDE"), R(200) {};

	AmericanOption(int iscall, double r, double sig, double S_0, double T, double K, int mesh_size, std::string method, double R) :
		Option(iscall, r, sig, S_0, T), 
		K(K), mesh_size(mesh_size), method(method), R(R) {};

	~AmericanOption();

	AmericanOption(const AmericanOption& source);

	AmericanOption& operator = (const AmericanOption& source);

	double PDEAmericanOptionPricer();

	double Payoff(double S);

	double Pricer();
};

struct AhGenerator {
	std::deque<double> a;
	std::deque<double> b;
	std::deque<double> c;

	AhGenerator(int n_x, double delta_t, double sig, double r);
};

#endif
