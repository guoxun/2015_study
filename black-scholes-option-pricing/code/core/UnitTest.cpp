#include "stdafx.h"
#include "UnitTest.h"
using namespace std;





//各个定价公式测试
void testBlackScholes(){
	cout << option_price_call_BlackScholes(50, 50, 0.10, 0.30, 0.5) << endl;
	cout << option_price_put_BlackScholes(50, 50, 0.10, 0.30, 0.5) << endl;
	cout << option_price_implied_volatility_call_BlackScholes_newton(21, 20, 0.1, 0.25, 1.875) << endl;
}

//正态分布的实现
double normalDistribution(double u){
	double y = abs(u);
	double y2 = y*y;
	double z = exp(-0.5 * y2) * 0.398942280401432678;
	double p = 0;
	int k = 28;
	double s = -1;
	double fj = k;

	if (y>3){
		//当y>3时
		for (int i = 1; i <= k; i++){
			p = fj / (y + p);
			fj = fj - 1.0;
		}
		p = z / (y + p);
	}
	else{
		//当y<3时
		for (int i = 1; i <= k; i++){
			p = fj * y2 / (2.0*fj + 1.0 + s * p);
			s = -s;
			fj = fj - 1.0;
		}
		p = 0.5 - z * y / (1 - p);
	}
	if (u>0) p = 1.0 - p;
	return p;
}

//欧式看涨期权
double option_price_call_BlackScholes(double s, double x, double r, double sigma, double T){

	double v1 = log(s / x);
	double v2 = (r + 0.5*pow(sigma, 2))*T;
	double v3 = (r - 0.5*pow(sigma, 2))*T;
	double v4 = sigma*pow(T, 0.5);
	double v5 = x*pow(exp(1.0), 0 - r*T);
	double vsT1 = normalDistribution((v1 + v2) / v4);
	double vsT2 = normalDistribution((v1 + v3) / v4);

	double vsT = vsT1*s - vsT2*v5;
	return vsT;
}


//欧式看跌期权
double option_price_put_BlackScholes(double s, double x, double r, double sigma, double T){
	double v1 = log(s / x);
	double v2 = (r + 0.5*pow(sigma, 2))*T;
	double v3 = (r - 0.5*pow(sigma, 2))*T;
	double v4 = sigma*pow(T, 0.5);
	double v5 = x*pow(exp(1.0), 0 - r*T);
	double vsT1 = normalDistribution(-(v1 + v2) / v4);
	double vsT2 = normalDistribution(-(v1 + v3) / v4);
	double vsT = vsT2*v5 - vsT1*s;
	return vsT;
}

//牛顿迭代法估算看涨期权隐含波动率
double option_price_implied_volatility_call_BlackScholes_newton(double s, double x, double r, double time, double option_price){

	const int MAX = 100;
	const double ACCURACY = 1.0e-4;
	double time_qrt = sqrt(time);
	double sigma = (option_price / s) / (0.398*time_qrt);
	for (size_t i = 0; i < MAX; i++)
	{
		double	price = option_price_call_BlackScholes(s, x, r, sigma, time);
		double diff = option_price - price;
		if (fabs(diff) < ACCURACY) return sigma;
		double d1 = (log(s / x) + r*time) / (sigma*time_qrt) + 0.5*sigma*time_qrt;
		double vega = s*time_qrt*normalDistribution(d1);
		sigma = sigma + diff / vega;
	}
	return -99e10;
}

//牛顿迭代法估算看跌期权隐含波动率
double option_price_implied_volatility_put_BlackScholes_newton(double s, double x, double r, double time, double option_price){

	const int MAX = 100;
	const double ACCURACY = 1.0e-4;
	double time_qrt = sqrt(time);
	double sigma = (option_price / s) / (0.398*time_qrt);//0.398?
	for (size_t i = 0; i < MAX; i++)
	{
		double	price = option_price_put_BlackScholes(s, x, r, sigma, time);
		double diff = option_price - price;
		if (fabs(diff) < ACCURACY) return sigma;
		double d1 = (log(s / x) + r*time) / (sigma*time_qrt) + 0.5*sigma*time_qrt;
		double vega = s*time_qrt*normalDistribution(d1);
		sigma = sigma + diff / vega;
	}
	return -99e10;
}

//欧式看涨期权重要参数估计
void option_price_partials_call_BlackSchloes(double &s, double &r, double &x, double &sigma, double &time,
	double &Delta, double &Gamma, double &Theta, double &Vega, double &Rho){
	double time_sqrt = sqrt(time);
	double d1 = (log(s / x) + r*time) / (sigma*time_sqrt) + 0.5*sigma*time_sqrt;
	double d2 = d1 - (s*sigma*time_sqrt);
	Delta = normalDistribution(d1);
	Gamma = normalDistribution(d1) / (s*sigma*time_sqrt);
	Theta = -(s*sigma*normalDistribution(d1)) / (2 * time_sqrt) - r*x*exp(-r*time)*normalDistribution(d2);
	Vega = s*time_sqrt*normalDistribution(d1);
	Rho = x*time * exp(-r*time)*normalDistribution(d2);
}

//欧式看跌期权重要参数估计
void option_price_partials_put_BlackSchloes(double &s, double &r, double &x, double &sigma, double &time,
	double &Delta, double &Gamma, double &Theta, double &Vega, double &Rho){
	double time_sqrt = sqrt(time);
	double d1 = (log(s / x) + r*time) / (sigma*time_sqrt) + 0.5*sigma*time_sqrt;
	double d2 = d1 - (s*sigma*time_sqrt);
	Delta = normalDistribution(d1) - 1;
	Gamma = normalDistribution(d1) / (s*sigma*time_sqrt);
	Theta = -(s*sigma*normalDistribution(d1)) / (2 * time_sqrt) + r*x*exp(-r*time)*normalDistribution(-d2);
	Vega = s*time_sqrt*normalDistribution(d1);
	Rho = -x*time * exp(-r*time)*normalDistribution(-d2);
}

//波动率计算器 garch方程三个参数 t为对应时间  计算年化波动率或是月化波动率
double getSigma(double a, double b, double c, int t){
	double sigma = a / (1 - b - c);
	return pow(sigma*t, 0.5);
}