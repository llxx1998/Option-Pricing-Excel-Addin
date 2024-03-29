﻿#include <cmath>
#include "xll_template.h"
#include "AsianOption.h"
#include "AmericanOption.h"

using namespace xll;


AddIn AsianOptionPricer(
	// Return double, C++ name of function, Excel name.
	Function(XLL_DOUBLE, "AsianOptionXLL", "AsianOption")
	// Array of function arguments.
	.Arguments({
		Arg(XLL_WORD, "iscall", "1 for call 0 for put"),
		Arg(XLL_DOUBLE, "r", "annual risk free rate"),
		Arg(XLL_DOUBLE, "sig", "annual volatility"),
		Arg(XLL_DOUBLE, "S_0", "spot price"),
		Arg(XLL_DOUBLE, "T", "time to maturity (year)"),
		Arg(XLL_LONG, "m", "period number"),
		Arg(XLL_DOUBLE, "K", "strike"),
		Arg(XLL_LONG, "n", "simulation number"),
		Arg(XLL_CSTRING4, "method", "Antithetic/Default")
		})
	// Function Wizard help.
	.FunctionHelp("Return the option price.")
	// Function Wizard category.
	.Category("OptionPricer")
	//
	.Documentation("This function is used to price Asian Options.")
);
// WINAPI calling convention must be specified
double WINAPI AsianOptionXLL(int iscall, double r, double sig, double S_0, double T, int m, double K, int n, char* method)
{
#pragma XLLEXPORT // must be specified to export function

	std::cout << std::string(method) << std::endl;
	AsianOption asop(iscall, r, sig, S_0, T, m, K, n, std::string(method));
	return asop.Pricer();
}

AddIn AmericanOptionPricer(
	// Return double, C++ name of function, Excel name.
	Function(XLL_DOUBLE, "AmericanOptionXLL", "AmericanOption")
	// Array of function arguments.
	.Arguments({
		Arg(XLL_WORD, "iscall", "1 for call 0 for put"),
		Arg(XLL_DOUBLE, "r", "annual risk free rate"),
		Arg(XLL_DOUBLE, "sig", "annual volatility"),
		Arg(XLL_DOUBLE, "S_0", "spot price"),
		Arg(XLL_DOUBLE, "T", "time to maturity (year)"),
		Arg(XLL_DOUBLE, "K", "strike"),
		Arg(XLL_LONG, "mesh_size", "mesh size"),
		Arg(XLL_DOUBLE, "r_k_ratio", "right boundary over K"),
		Arg(XLL_CSTRING4, "method", "PDE"),
		})
	// Function Wizard help.
	.FunctionHelp("Return the option price.")
	// Function Wizard category.
	.Category("OptionPricer")
	//
	.Documentation("This function is used to price American Options.")
);
// WINAPI calling convention must be specified
double WINAPI AmericanOptionXLL(int iscall, double r, double sig, double S_0, double T, double K, int mesh_size, double r_k_ratio, char* method)
{
#pragma XLLEXPORT // must be specified to export function
	AmericanOption ameop(iscall, r, sig, S_0, T, K, mesh_size, std::string(method), r_k_ratio * K);
	return ameop.Pricer();
}

AddIn EuropeanAnalyticPricer(
	// Return double, C++ name of function, Excel name.
	Function(XLL_DOUBLE, "EuropeanOptionXLL", "EuropeanOption")
	// Array of function arguments.
	.Arguments({
		Arg(XLL_DOUBLE, "S", "spot price"),
		Arg(XLL_DOUBLE, "K", "strike"),
		Arg(XLL_DOUBLE, "r", "annual risk free rate"),
		Arg(XLL_DOUBLE, "T", "time to maturity (year)"),
		Arg(XLL_DOUBLE, "sig", "annual volatility"),
		Arg(XLL_WORD, "iscall", "1 for call 0 for put"),
		Arg(XLL_DOUBLE, "div", "dividend (absolute/percentage)"),
		Arg(XLL_WORD, "isdivdiscrete", "1 for discrete dividend 0 otherwise")
		})
		// Function Wizard help.
	.FunctionHelp("Return the option price.")
	// Function Wizard category.
	.Category("OptionPricer")
	//
	.Documentation("This function is used to price European Options.")
);
// WINAPI calling convention must be specified
double WINAPI EuropeanOptionXLL(double S, double K, double r, double T, double sig, int iscall, double div, int isdivdiscrete)
{
#pragma XLLEXPORT // must be specified to export function
	return BlackScholesFormula(S, K, r, T, sig, iscall, div, isdivdiscrete);
}



// Press Alt-F8 then type 'XLL.MACRO' to call 'xll_macro'
// See https://xlladdins.github.io/Excel4Macros/
AddIn xai_macro(
	// C++ function, Excel name of macro
	Macro("xll_macro", "XLL.MACRO")
);
// Macros must have `int WINAPI (*)(void)` signature.
int WINAPI xll_macro(void)
{
#pragma XLLEXPORT
	// https://xlladdins.github.io/Excel4Macros/reftext.html
	// A1 style instead of default R1C1.
	OPER reftext = Excel(xlfReftext, Excel(xlfActiveCell), OPER(true));
	// UTF-8 strings can be used.
	Excel(xlcAlert, OPER("XLL.MACRO called with активный 细胞: ") & reftext);

	return TRUE;
}

#ifdef _DEBUG

// Create XML documentation and index.html in `$(TargetPath)` folder.
// Use `xsltproc file.xml -o file.html` to create HTML documentation.
Auto<Open> xao_template_docs([]() {

	return Documentation("OptionPricer", "Documentation for the OptionPricer add-in.");

});

#endif // _DEBUG
