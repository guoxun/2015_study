#ifndef PRICECONTROL_H
#define PRICECONTROL_H

#include "iostream"
#include <fstream>
#include "math.h"
#include <string>
#include <map>
#include <strstream>
using namespace std;
//期权价格类
struct Price{
	//int *premium;//权利金
	double strikePrice[4];//执行价格
	double optionPrice[4];//期权价格
};

//日期与价格
struct Daily{
	string date;
	double price;
};

//定价器参数处理
struct BlackSchlesArgu{
	int premium;//权利金
	double strikePrice;//行权价格 即执行价格
	double r;//无风险利率
	double sigma;//波动率
	double time;//行权时间
};

//定价器重要参考参数
struct ReferArgu{
	double Delta;
	double Gamma;
	double Theta;
	double Vega;
	double Rho;
};

//收盘价管理器
typedef map<int, Daily> mapIntDaily;

class PriceManager {
public:
	PriceManager();
	~PriceManager();
	void initAlldays();//从文件中读取日期和收盘价
	mapIntDaily getMap();
	double getPrice(int num);//根据序号返回价格
	Daily getDaily(int num);//根据序号返回日期价格
	double getStrike(int num,int t);//根据收盘价计算执行价格
	void outputByNum(int num, int t);//返回指定序号指定个数的价格
private:
	double *alldays;//记录所有收盘价记录
	string *num;//记录对应日期
	int dayNum;//记录日期数
	mapIntDaily mId;// 日期序号与价格映射
	//套利策略类
};

//定价器  欧式美式 看涨看跌 外加波动率  隐含波动率 重要参数
class BlackScholes{
public:
	BlackScholes();
	~BlackScholes();
	//期权定价算法
	double normalDistribution(double u);//正态
	long double getSigma(long double a,long double b,long double c, int t);//年化波动率计算器
	double option_price_call_BlackScholes(double s, double k, double r, double sigma, double T);//隐含波动率
	double option_price_put_BlackScholes(double s, double k, double r, double sigma, double T);
	double option_price_implied_volatility_call_BlackScholes_newton(double s, double k, double r, double T, double price);
	double option_price_implied_volatility_put_BlackScholes_newton(double s, double k, double r, double T, double price);
	double option_price_implied_volatility_put_BlackScholes_bisection(double s, double x, double r, double time, double option_price);
	double option_price_implied_volatility_call_BlackScholes_bisection(double s, double x, double r, double time, double option_price);
	void option_price_partials_call_BlackSchloes(double &s, double &r, double &x, double &sigma, double &time,
		double &Delta, double &Gamma, double &Theta, double &Vega, double &Rho);
	void option_price_partials_put_BlackSchloes(double &s, double &r, double &x, double &sigma, double &time,
		double &Delta, double &Gamma, double &Theta, double &Vega, double &Rho); 
	//期权损益算法 s为当前股票价格 x为执行价格 pre为期权价格
	double option_price_call_buy(double s,double x,double pre);
	double option_price_call_sell(double s,double x,double pre);
	double option_price_put_buy(double s,double x,double pre);
	double option_price_put_sell(double s,double x,double pre);
};

//套利策略基类 权利金 执行价格 期权价格 各三个 标的资产价格 一个
class ArbitStrategy{
public:
	ArbitStrategy(int n);//n表示期权个数
	ArbitStrategy(){};
	~ArbitStrategy(){};
	virtual double ArbitENow(ofstream &outFile) = 0;//套利损益立即价值 纯虚函数
	virtual double ArbitENow() = 0;//套利损益立即价值 纯虚函数
	void setAsset(double asset);
	double getAsset();
	void setPrice(Price* pt);//传入数据 参数对应文件名
	Price* getPrice();
	int getN();
	BlackScholes *bs;
protected:
	Price *price;
	double asset;//标的资产价格(当前)
	int n;//期权个数 根据个数不同实现如下不同子类
};

//蝶式  2015.4.3考虑要不要使用多态性  在发现立即收益各个策略不同时  多态性实现是必要的
class BufferFly :public ArbitStrategy{
public:
	//BufferFly(Price *p,double asst,int n);
	BufferFly(Price *p);
	~BufferFly();
	double ArbitENow(ofstream &outFile);
	double ArbitENow();
	Price* getPrice();
//	void setPrice(string file);//父类声明即可
};

//鹰式
class Hawk :public ArbitStrategy{
public:
	Hawk(Price *p);
	~Hawk();
	double ArbitENow(ofstream &outFile);
	double ArbitENow();
	double getAsset();
	void setPrice(Price* pt);//传入数据 参数对应文件名
	Price* getPrice();
	//	void setPrice(string file);//父类声明即可
};
//跨式 宽跨式都用此类 两者的差别在于宽跨式价格一高一低
class Straddle :public ArbitStrategy{
public:
	Straddle(Price *p);
	~Straddle();
	double ArbitENow(ofstream &outFile);
	double ArbitENow();
	Price* getPrice();
	//	void setPrice(string file);//父类声明即可
};

#endif


