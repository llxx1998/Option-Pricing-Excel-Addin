#pragma once
#ifndef Option_H
#define Option_H

#include <string>
#include "MathUtilities.h"

class Option {
protected:
	int iscall;
	double r;
	double sig;
	double S_0;
	double T;
public:
	Option() : iscall(1), r(0.01), sig(0.3), S_0(110), T(1) {};

	Option(int iscall, double r, double sig, double S_0, double T) :
		iscall(iscall), 
		r(r), 
		sig(sig), 
		S_0(S_0), 
		T(T) {};

	virtual ~Option();

	Option(const Option& Opt);

	Option& operator = (const Option& Opt);

	virtual double Pricer() = 0;
};

#endif // !Option_H

