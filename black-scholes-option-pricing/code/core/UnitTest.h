#ifndef UNITTEST_H
#define UNITTEST_H

#include "math.h"
#include "iostream"
using namespace std;

void testBlackScholes();
double normalDistribution(double u);
double option_price_call_BlackScholes(double s, double k, double r, double sigma, double T);
double option_price_put_BlackScholes(double s, double k, double r, double sigma, double T);
double option_price_implied_volatility_call_BlackScholes_newton(double s, double k, double r, double T, double price);
double option_price_implied_volatility_put_BlackScholes_newton(double s, double x, double r, double time, double option_price);
void option_price_partials_call_BlackSchloes(double &s, double &r, double &x, double &sigma, double &time,
	double &Delta, double &Gamma, double &Theta, double &Vega, double &Rho);
void option_price_partials_put_BlackSchloes(double &s, double &r, double &x, double &sigma, double &time,
	double &Delta, double &Gamma, double &Theta, double &Vega, double &Rho);
double getSigma(double a, double b, double c, int t);

#endif