#include "stdafx.h"
#include "PriceControl.h"

using namespace std;
/*

PriceManager类
用于记录整个收盘价 
通过指定日期序号获取收盘价

*/
PriceManager::PriceManager(){
	this->initAlldays();
}

//
PriceManager::~PriceManager(){

}

void PriceManager::initAlldays(){
	ifstream inFile;
	inFile.open("1986-2013.txt");
	if (!inFile.is_open())
	{
		cout << "cannot open the file" << endl;
		exit(EXIT_FAILURE);
	}

	int count = 0;
	while (!inFile.eof())
	{
		Daily di;
		string date;
		double price;
		inFile >> date>>price;
		di.date = date;
		di.price = price;
		mId.insert(map<int, Daily>::value_type(count, di));
		count++;
	}
	if (inFile.eof())
	{
		cout << "all was end" << endl;
		inFile.close();
	}
}

//返回映射
map<int, Daily> PriceManager::getMap(){
	return mId;
}

double PriceManager::getPrice(int num){
	map<int, Daily>::iterator iter;
	iter = mId.find(num);
	if (iter != mId.end())
	{
		cout << "Find, the value is "<< iter->second.price << endl;
		return iter->second.price;
	}
	else
	{
		cout << "Do not Find" << endl;
		return 0.0;
	}

}

Daily PriceManager::getDaily(int num){
	map<int, Daily>::iterator iter;

	iter = mId.find(num);
	if (iter != mId.end())
	{
		cout << "Find, the value is " << iter->second.price << endl;
		return iter->second;
	}
	else
	{
		cout << "Do not Find" << endl;
	}
}

//num为当天对应序号 t为计算均值所需要的天数
double PriceManager::getStrike(int num, int t){
	map<int, Daily>::iterator iter;
	iter = mId.find(num - t);
	double price = 0.0;
	for (size_t i = 0; i < t; i++)
	{
		price += iter->second.price;
		iter++;
	}
	return price/t;
}

void PriceManager::outputByNum(int num, int t){
	map<int, Daily>::iterator iter;
	iter = mId.find(num);
	ofstream off;
	off.open("fliterPrice.txt");
	int n = 0;
	for (size_t i = 0; i < t; i++)
	{
		off << iter->second.date << " " << iter->second.price << endl;
		iter++;
		n++;
	}
	if (off.eof())
	{
		cout <<n<< "all was end" << endl;
		off.close();
	}
}

/*

BlackScholes算法类
对应算法接口

*/

BlackScholes::BlackScholes(){

}

BlackScholes::~BlackScholes(){

}


//正态分布
double BlackScholes::normalDistribution(double u){
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
double BlackScholes::option_price_call_BlackScholes(double s, double x, double r, double sigma, double T){

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
double BlackScholes::option_price_put_BlackScholes(double s, double x, double r, double sigma, double T){
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
double BlackScholes::option_price_implied_volatility_call_BlackScholes_newton(double s, double x, 
	double r, double time, double option_price){

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
double BlackScholes::option_price_implied_volatility_put_BlackScholes_newton(double s, double x, double r, 
	double time, double option_price){

	const int MAX = 100;
	const double ACCURACY = 1.0e-4;
	double time_qrt = sqrt(time);
	double sigma = (option_price / s) / (0.398*time_qrt);//
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

//二分法估算看跌期权隐含波动率
double BlackScholes::option_price_implied_volatility_call_BlackScholes_bisection(double s, double x, double r,
	double time, double option_price){
	const double ACCURACY = 1.0e-5;
	const int MAX_ITERATIONS = 100;
	const double ERROR = -1e40;

	double sigma_low = 1e-5;
	double sigma_high = 0.3;

	for (size_t i = 0; i < MAX_ITERATIONS; i++)
	{
		double sigma = (sigma_low + sigma_high)*0.5;
		double	price = option_price_call_BlackScholes(s, x, r, sigma, time);
		double test = (price - option_price);
		if (fabs(test)<ACCURACY)
		{
			return sigma;
		}
		if (test<0.0)
		{
			sigma_low = sigma;
		}
		else {
			sigma_high = sigma;
		}
	}
}

//二分法估算看跌期权隐含波动率
double BlackScholes::option_price_implied_volatility_put_BlackScholes_bisection(double s, double x, double r, 
	double time, double option_price){
	const double ACCURACY = 1.0e-5;
	const int MAX_ITERATIONS = 100;
	const double ERROR = -1e40;

	double sigma_low = 1e-5;
	double sigma_high = 0.3;

	for (size_t i = 0; i < MAX_ITERATIONS; i++)
	{
		double sigma = (sigma_low + sigma_high)*0.5;
		double	price = option_price_put_BlackScholes(s, x, r, sigma, time);
		double test = (price - option_price);
		if (fabs(test)<ACCURACY)
		{
			return sigma;
		}
		if (test<0.0)
		{
			sigma_low = sigma;
		}
		else {
			sigma_high = sigma;
		}
	}
}

//欧式看涨期权重要参数估计
void BlackScholes::option_price_partials_call_BlackSchloes(double &s, double &r, double &x, double &sigma, double &time,
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
void BlackScholes::option_price_partials_put_BlackSchloes(double &s, double &r, double &x, double &sigma, double &time,
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
long double BlackScholes::getSigma(long double a,long double b,long double c, int t){
	long double sigma = a / (1 - b - c);
	return pow(sigma*t, 0.5);
}

//看涨期权购买者损益
double BlackScholes::option_price_call_buy(double s, double x,double pre){
	if (s < x) return -pre;
	if (s >= x) return s - x - pre;
}

//看涨期权卖出者损益
double BlackScholes::option_price_call_sell(double s, double x, double pre){
	if (s < x) return pre;
	if (s >= x) return x - s + pre;
}

//看跌期权购买者损益
double BlackScholes::option_price_put_buy(double s, double x, double pre){
	if (s <= x) return x - s - pre;
	if (s > x)  return -pre;
}

//看跌期权卖出者的损益
double BlackScholes::option_price_put_sell(double s, double x, double pre){
	if (s <= x) return s - x + pre;
	if (s > x) return pre;
}

/*

ArbitStrategy类  策略类基类

*/

ArbitStrategy::ArbitStrategy(int num){
	this->n = num;
}



double ArbitStrategy::getAsset(){
	return asset;
}

void ArbitStrategy::setAsset(double t){
	asset = t;
}

//从文件中获取套利数据
void ArbitStrategy::setPrice(Price* pt){

	//n为该类数据
	for (size_t i = 0; i < n; i++)
	{
		price->optionPrice[i] = pt->optionPrice[i];
		price->strikePrice[i] = pt->strikePrice[i];
	}
}

Price* ArbitStrategy::getPrice(){
	return price;
}

//套利评估
/*double ArbitStrategy::ArbitENow(){
	cout << "基类套利算法  不需要实现" << endl;
		return 0;
}*/

int ArbitStrategy::getN(){
	return n;
}

//蝶式套利 买入一高一低看涨期权  卖出两个中间价看涨期权
//索引0123分别对应 01 对应一高一低看涨期权 23对应卖出的两个相同中间价看涨期权
BufferFly::BufferFly(Price *p){
	n = 3;
	price = p;
	//this->setPrice("1.txt");
}

//立即价值 将对应的四份套利当前价值求和
double BufferFly::ArbitENow(ofstream &outFile){
	double all=0.0;//double型初始化方式
	for (size_t i = 0; i < 2; i++)
	{
		outFile << "第" << i << "份期权为" <<
			bs->option_price_call_buy(asset, price->strikePrice[i], price->optionPrice[i]) << "收益" << endl;
	    all +=	bs->option_price_call_buy(asset, price->strikePrice[i], price->optionPrice[i]);
	}
	//两个卖出的相同中间价看涨期权
	outFile << "两个卖出的中间价的期权为" <<
		bs->option_price_call_sell(asset, price->strikePrice[2], price->optionPrice[2]) << "收益" << endl;
	double ai = 2*bs->option_price_call_sell(asset, price->strikePrice[2], price->optionPrice[2]);
	all += ai;
	return all;
}

double BufferFly::ArbitENow(){
	double all = 0.0;//double型初始化方式
	for (size_t i = 0; i < 2; i++)
	{
		cout << "第" << i << "份期权为" <<
			bs->option_price_call_buy(asset, price->strikePrice[i], price->optionPrice[i]) << "收益" << endl;
		all += bs->option_price_call_buy(asset, price->strikePrice[i], price->optionPrice[i]);
	}
	//两个卖出的相同中间价看涨期权
	cout << "两个卖出的中间价的期权为" <<
		bs->option_price_call_sell(asset, price->strikePrice[2], price->optionPrice[2]) << "收益" << endl;
	double ai = 2 * bs->option_price_call_sell(asset, price->strikePrice[2], price->optionPrice[2]);
	all += ai;
	return all;
}

//鹰式套利 买入
Hawk::Hawk(Price *p){
	n = 4;
	price = p;
}

//立即价值 鹰式套利与蝶式基本一致 为了方便分开描述 最大不同在卖出期权 
double Hawk::ArbitENow(ofstream &outFile){
	double all = 0.0;
	for (size_t i = 0; i <  2; i++)
	{
		outFile << "第" << i << "份期权为" <<
			bs->option_price_call_buy(asset, price->strikePrice[i], price->optionPrice[i]) << "收益" << endl;
		all += bs->option_price_call_buy(asset, price->strikePrice[i], price->optionPrice[i]);
	}
	//两个卖出期权  一个略高于中间价 一个略低于中间价
	for (size_t i = 2; i < n; i++)
	{
		outFile << "第" << i << "份期权为" <<
			bs->option_price_call_sell(asset, price->strikePrice[i], price->optionPrice[i]) << "收益" << endl;
		all += bs->option_price_call_sell(asset, price->strikePrice[i], price->optionPrice[i]);
	}
	return all;
}

double Hawk::ArbitENow(){
	double all = 0.0;
	for (size_t i = 0; i < 2; i++)
	{
		cout << "第" << i << "份期权为" <<
			bs->option_price_call_buy(asset, price->strikePrice[i], price->optionPrice[i]) << "收益" << endl;
		all += bs->option_price_call_buy(asset, price->strikePrice[i], price->optionPrice[i]);
	}
	//两个卖出期权  一个略高于中间价 一个略低于中间价
	for (size_t i = 2; i < n; i++)
	{
		cout << "第" << i << "份期权为" <<
			bs->option_price_call_sell(asset, price->strikePrice[i], price->optionPrice[i]) << "收益" << endl;
		all += bs->option_price_call_sell(asset, price->strikePrice[i], price->optionPrice[i]);
	}
	return all;
}
//跨式与宽跨式 买入一个看涨期权 一个看跌期权 跨式两者价格相同  宽跨式两者价格相差较大
Straddle::Straddle(Price *p){
	n = 2 ;
	//this->setPrice("3.txt");
	price = p;
}

double Straddle::ArbitENow(ofstream &outFile){
	double all = 0.0;
	outFile << "第" << 0 << "份期权为" <<
		bs->option_price_call_buy(asset, price->strikePrice[0], price->optionPrice[0]) << "收益" << endl;
	all += bs->option_price_call_buy(asset, price->strikePrice[0], price->optionPrice[0]);
	outFile << "第" << 1 << "份期权为" <<
		bs->option_price_call_buy(asset, price->strikePrice[1], price->optionPrice[1]) << "收益" << endl;
	all += bs->option_price_call_buy(asset, price->strikePrice[1], price->optionPrice[1]);
	return all;
}

double Straddle::ArbitENow(){
	double all = 0.0;
	cout << "第" << 0 << "份期权为" <<
		bs->option_price_call_buy(asset, price->strikePrice[0], price->optionPrice[0]) << "收益" << endl;
	all += bs->option_price_call_buy(asset, price->strikePrice[0], price->optionPrice[0]);
	cout << "第" << 1 << "份期权为" <<
		bs->option_price_call_buy(asset, price->strikePrice[1], price->optionPrice[1]) << "收益" << endl;
	all += bs->option_price_call_buy(asset, price->strikePrice[1], price->optionPrice[1]);
	return all;
}
