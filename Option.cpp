#include "Option.h"

Option::~Option() {
	// destructor
}

Option::Option(const Option& Opt) {
	iscall = Opt.iscall;
	r = Opt.r;
	sig = Opt.sig;
	S_0 = Opt.S_0;
	T = Opt.T;
}

Option& Option::operator = (const Option& Opt) {
	if (this == &Opt) return *this;
	iscall = Opt.iscall;
	r = Opt.r;
	sig = Opt.sig;
	S_0 = Opt.S_0;
	T = Opt.T;
	return *this;
}