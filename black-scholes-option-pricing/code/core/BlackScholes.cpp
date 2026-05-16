// BlackScholes.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "PriceControl.h"
using namespace std;

void testPriceManager();
void filterPrice();
double segimaByPrice(long double a,long double b,long double c);
void RunOfYearOption();
void RunOption(double a, double b, double c, int circle, int start);
void RunOptionCon(double a, double b, double c, int circle, int start);
int testFaler();
void RunSigma(double a);
const int CIRCLE = 242;//年交易日 常数
const double r = 0.0275;//年化利率

int _tmain(int argc, _TCHAR* argv[])
{
	/*RunSigma(0.010871);//242
	RunSigma(0.023388);//484
	RunSigma(0.012199);//726
	RunSigma(0.011478);//968
	RunSigma(0.011747);//1210
	
	RunSigma(0.015213);
	RunSigma(0.019513);
	RunSigma(0.014035);
	RunSigma(0.010366);//2178
	RunSigma(0.014659);//2420
	
	RunSigma(0.033345);//2662
	RunSigma(0.018342);//2904
	RunSigma(0.019220);//3146
	RunSigma(0.017238);//3388
	RunSigma(0.013741);//3630
	
	RunSigma(0.010938);//3872
	RunSigma(0.011408);//4114
	RunSigma(0.007232);//4356
	RunSigma(0.008913);//4598
	RunSigma(0.010843);//4840
	
	RunSigma(0.023200);//5082
	RunSigma(0.033064);//5324
	RunSigma(0.012609);//5566
	RunSigma(0.012758);//5808*/
/*	RunOption(3.065213623031844e-06, -0.06387483245975218, 1.019343002412134, 242,484);
	RunOption(2.34E-05, 0.387899, 0.528929, 242, 968);
	RunOption(5.26E-05, 0.047103, 0.543936,242,1210);
	RunOption(3.49E-05, 0.265506, 0.494314,242,1452);
	RunOption(7.81E-05, 0.402379, 0.271602,242,1694);//蝶式鹰式
	RunOption(6.36E-06, 0.085548, 0.899005,242, 1936);//蝶式 鹰式
	RunOption(6.21E-06, 0.083850, 0.887034, 242, 2178);//鹰式
	RunOption(1.17E-05, 0.093461, 0.803608, 242, 2420);
	RunOption(1.21E-05, 0.094274, 0.851991,242,2662);//跨式
	RunOption(4.08E-05, 0.129759, 0.821623,242,2904);//跨式
	RunOption(6.51E-05, -0.061932, 0.856048, 242, 3146);
	RunOption(2.23E-05, 0.016850, 0.925874, 242, 3388);
	RunOption(1.82E-05, 0.074742, 0.872851,242,3630);//蝶式鹰式
	RunOption(4.53E-06, -0.058326, 1.025313, 242, 3872);//蝶式鹰式
	RunOption(6.26E-05, -0.037880, 0.511454, 242, 4114);//鹰式
	RunOption(7.05E-05, -0.078326, 0.512872, 242, 4356);//鹰式
	RunOption(4.52E-05, -0.047216, 0.180763, 242, 4598);//鹰式
	RunOption(2.99E-06, 0.039879, 0.922933, 242, 4840);
	RunOption(2.03E-06, 0.048918, 0.940979, 242, 5082);//蝶式鹰式
	RunOption(5.39E-05, 0.179965, 0.712380,242,5324);//蝶式鹰式
	RunOption(2.53E-05, 0.121809, 0.856137, 242, 5566);//蝶式鹰式
	RunOption(-6.99E-07, -0.027040, 1.028226, 242, 5808);//碟 鹰
	RunOption(9.48E-06, 0.069891, 0.876793, 242, 6050);*///碟鹰
//	RunOption(5.74E-05, -0.288758, 1.144031, 20, 262);
//	PriceManager *pm = new PriceManager();
//	pm->outputByNum(484, 121);
/*	BlackScholes *bs = new BlackScholes();
	double s = 50;
	double x = 50;
	double ri = 0.1;
	double sigma = 0.3;
	double t = 0.5;
	double de;
	double ga;
	double the;
	double ve;
	double rh;
	bs->option_price_partials_call_BlackSchloes(s,x,ri,sigma,t,de,ga,the,ve,rh);
	filterPrice();*/
//	testFaler();
	return 0;
}
//实验数据分割算法
int testFaler(){
	PriceManager *pm = new PriceManager();
	mapIntDaily mi = pm->getMap();
	mapIntDaily::iterator it;
	int count = 0;
	it = mi.begin();
	while(it != mi.end())
	{
		int end = count + 242;
		strstream ss;
		string s;
		ss << count;
		ss >> s;
		s += ".txt";
		ofstream off;
		off.open(s);
		for (size_t i = count; i < end; i++)
		{
			off <<it->second.price << endl;
			it++;
			count = i;
			if (it == mi.end()){
				cout << " that's all" << endl;
				return 0;
			}
		}
		it++;
		count++;
	}
	return 0;
}

//恒生指数收盘价数据2013/01/02 至 2013/05/28 股价处理
//该函数用于找到对应收盘日的股价一个年周期为242天 找到对应的周期天数 按242天一个周期计算波动率 2015.4.7
void testPriceManager(){
	PriceManager *pm = new PriceManager();
	mapIntDaily mi = pm->getMap();
	mapIntDaily::iterator it;
	int count = 0;
	int circle = 0;
	ofstream off;
	off.open("priceOfyear.txt");

	for (it = mi.begin(); it != mi.end(); it++)
	{
		if (count % CIRCLE == 0 )
		{
			off << it->first <<" "<<it->second.date << " " << it->second.price<< endl;

			circle++;
		}
		count++;
	}
	off << mi.size() << endl;
	off << mi.find(10)->second.date << endl;
	off << circle << endl;
	off.close();
}

//期权价格选取
void filterPrice(){
	string filename = "19970910-19980902.txt";
	string outputName = "19970910-19980902 price.txt";
	ifstream inFile;
	ofstream outFile;
	inFile.open(filename);
	outFile.open(outputName);
	if (!inFile.is_open())
	{
		cout << "cannot open the file" << endl;
		exit(EXIT_FAILURE);
	}
	if (!outFile.is_open())
	{
		cout << "cannot open the file" << endl;
		exit(EXIT_FAILURE);
	}
	string prize[8000];
	string year[8000];
	int count = 0;
	while (!inFile.eof())
	{
		//in.getline(buffer, 100);
		//cout << buffer << endl;
		//string year;
		int j;
		inFile >> year[count];
		inFile >> prize[count];
		inFile >> j;
		count++;
	}
	cout << count;
	for (size_t i = 0; i < count; i++)
	{
	//	outFile << year[i];
	//	outFile << " ";
		outFile << prize[i];
		outFile << " ";
	//	outFile << i;
		outFile << " \r\n";
	}
	if (inFile.eof())
	{
		cout << "all was end" << endl;
		inFile.close();
		outFile.close();
	}
}

double segimaByPrice(long double a,long double b,long double c){
	BlackScholes *bs = new BlackScholes();
	return bs->getSigma(a, b, c, CIRCLE);
}

//该测试以1987年到1988年为基础的港股股价测试
//波动率获取 1987-1988年化波动率对应参数
//3.065213623031844e-06
//-0.06387483245975218
//1.019343002412134
//根据上述计算所得年化波动率为0.129063 map区间为[242,484) option区间为[484,726)
//认股价以股权售出日前5个交易日在联交所的平均收市价的80% 执行低价  执行中间价格为五日均价  执行高价取五日平均收市价的120%
//2690.300 2680.200 2671.400 2665.900 2651.900  对应执行价格定价为2137.55 
void RunOfYearOption(){
	PriceManager *pm = new PriceManager();
	BlackScholes *bs = new BlackScholes();
	double sigma = bs->getSigma(3.065213623031844e-06, -0.06387483245975218, 1.019343002412134,CIRCLE);
	double time = 1;
	Price p;
	double strike = pm->getStrike(CIRCLE*2, 5);//484 484之前的五天的均价不含484
	double st = pm->getPrice(CIRCLE*2);//期权合约首天价格 484
	double vBS[4];
	//Bufferfly套利 索引:0 1 表示一高一低 2表示中间价 该期权策略适合于市场波动率变化较小时的情况
	double rateBuffer = 0.8;//由rate知道 价差越大即rate越大 套利越多
	vBS[0] = bs->option_price_call_BlackScholes(st, (2 - rateBuffer)*strike, r, sigma, time);
	vBS[1] = bs->option_price_call_BlackScholes(st, rateBuffer*strike, r, sigma, time);
	vBS[2] = bs->option_price_call_BlackScholes(st, strike, r, sigma, time);
	p.optionPrice[0] = vBS[0];
	p.strikePrice[0] = (2 - rateBuffer)*strike;
	p.optionPrice[1] = vBS[1];
	p.strikePrice[1] = rateBuffer*strike;
	p.optionPrice[2] = vBS[2];
	p.strikePrice[2] = strike;

	BufferFly *bf = new BufferFly(&p);
	bf->setAsset(pm->getDaily(725).price);
	cout << "st: " << st << "strike: " << strike << "sigma: "<<sigma<<endl;
	cout<<"一年期蝶式套利价格收益:"<<bf->ArbitENow()<<endl;

	//Hawk套利 鹰式套利  类似于蝶式  0 1表示一高一低 2 3表示一高一低的中间价 x0+x3=x1+x2
	//鹰式套利比蝶式的好处为可以接受波动率的微小变化 但是收益可能较低
	memset(&p, 0, sizeof(p));
	double rateHawk1 = 0.5;//该比率为x0 x3对应比率 r1<r2
	double rateHawk2 = 0.8;
	vBS[0] = bs->option_price_call_BlackScholes(st, (2 - rateHawk1)*strike, r, sigma, time);
	vBS[1] = bs->option_price_call_BlackScholes(st, rateHawk1*strike, r, sigma, time);
	vBS[2] = bs->option_price_call_BlackScholes(st, (2 - rateHawk2)*strike, r, sigma, time);
	vBS[3] = bs->option_price_call_BlackScholes(st, rateHawk2*strike, r, sigma, time);
	p.optionPrice[0] = vBS[0];
	p.strikePrice[0] = (2 - rateHawk1)*strike;
	p.optionPrice[1] = vBS[1];
	p.strikePrice[1] = rateHawk1*strike;
	p.optionPrice[2] = vBS[2];
	p.strikePrice[2] = (2 - rateHawk2)*strike;
	p.optionPrice[3] = vBS[3];
	p.strikePrice[3] = rateHawk2*strike;

	Hawk *hk = new Hawk(&p);
	hk->setAsset(pm->getDaily(725).price);
	cout << "一年期鹰式套利价格收益" << hk->ArbitENow() << endl;

	//跨式套利 由于波动率的平稳  显然跨式套利没有收益
	memset(&p, 0, sizeof(p));
	vBS[0] = bs->option_price_call_BlackScholes(st,strike, r, sigma, time);
	vBS[1] = bs->option_price_put_BlackScholes(st, strike, r, sigma, time);
	p.optionPrice[0] = vBS[0];
	p.strikePrice[0] = strike;
	p.optionPrice[1] = vBS[1];
	p.strikePrice[1] = strike;
	
	Straddle *stra = new Straddle(&p);
	stra->setAsset(pm->getDaily(725).price);
	cout << "一年期跨式套利价格收益" << stra->ArbitENow() << endl;

	//宽跨式套利 strangle
	memset(&p, 0, sizeof(p));
	double rateStra = 0.8;
	vBS[0] = bs->option_price_call_BlackScholes(st, (2-rateStra)*strike, r, sigma, time);
	vBS[1] = bs->option_price_put_BlackScholes(st, rateStra*strike, r, sigma, time);
	p.optionPrice[0] = vBS[0];
	p.strikePrice[0] = strike;
	p.optionPrice[1] = vBS[1];
	p.strikePrice[1] = strike;

	Straddle *straB = new Straddle(&p);
	straB->setAsset(pm->getDaily(725).price);
	cout << "一年期宽跨式套利价格收益" << straB->ArbitENow() << endl;
}

//根据月化波动率计算的一个月的期权合约
//1987年12月14日起一个月对应波动率参数
//5.74E-05
//-0.288758
//1.144031
//sigma为波动率 circle为交易周期 circle =20
//1987/12/24(242)- 1988/01/22(261) 20 合约起始日为262 
//abc为波动率参数                                //   20         262  
//map区间为[242,261] option区间为[262,281]
//RunOfMonthOption可以实现任何时段的期权  不管年月日  只要时间序号合适
/*void RunOption(double a,double b,double c,int circle,int start){
	PriceManager *pm = new PriceManager();
	BlackScholes *bs = new BlackScholes();
	double sigma = bs->getSigma(a, b, c, circle);
	Price p;
	double strike = pm->getStrike(start, 5);
	double st = pm->getPrice(start);//期权合约首天价格
//	cout<<pm->getDaily(3630).date<<endl;
cout << pm->getDaily(3388).date << endl;
	double vBS[4];
	double time = (double)circle / 242;
	char fName[32] = { 0, };
	//该部分用于处理txt
	sprintf_s(fName, "re%d", start);
	string fn = fName;
	fn += ".txt";
	ofstream outFile;
	outFile.open(fn);
	ofstream &of = outFile;
	if (!outFile.is_open())
	{
		cout << "cannot open the file" << endl;
		exit(EXIT_FAILURE);
	}
	//Bufferfly套利 索引:0 1 表示一高一低 2表示中间价 该期权策略适合于市场波动率变化较小时的情况
	double rateBuffer = 0.8;//由rate知道 价差越大即rate越大 套利越多
	vBS[0] = bs->option_price_call_BlackScholes(st, (2 - rateBuffer)*strike, r, sigma, time);
	vBS[1] = bs->option_price_call_BlackScholes(st, rateBuffer*strike, r, sigma, time);
	vBS[2] = bs->option_price_call_BlackScholes(st, strike, r, sigma, time);
	p.optionPrice[0] = vBS[0];
	p.strikePrice[0] = (2 - rateBuffer)*strike;
	p.optionPrice[1] = vBS[1];
	p.strikePrice[1] = rateBuffer*strike;
	p.optionPrice[2] = vBS[2];
	p.strikePrice[2] = strike;

	BufferFly *bf = new BufferFly(&p);
	bf->setAsset(pm->getDaily(start+circle-1).price);
	cout << "st: " << st << " strike: " << strike << " sigma: " << sigma << endl;
	outFile << " " << st << "  " << strike << "  " << sigma;
	cout << time<<"年期蝶式套利组合价格收益:" << bf->ArbitENow() << endl;
	outFile <<  " " << bf->ArbitENow(of)<<" ";

	//Hawk套利 鹰式套利  类似于蝶式  0 1表示一高一低 2 3表示一高一低的中间价 x0+x3=x1+x2
	//鹰式套利比蝶式的好处为可以接受波动率的微小变化 但是收益可能较低
	memset(&p, 0, sizeof(p));
	double rateHawk1 = 0.5;//该比率为x0 x3对应比率 r1<r2
	double rateHawk2 = 0.8;
	vBS[0] = bs->option_price_call_BlackScholes(st, (2 - rateHawk1)*strike, r, sigma, time);
	vBS[1] = bs->option_price_call_BlackScholes(st, rateHawk1*strike, r, sigma, time);
	vBS[2] = bs->option_price_call_BlackScholes(st, (2 - rateHawk2)*strike, r, sigma, time);
	vBS[3] = bs->option_price_call_BlackScholes(st, rateHawk2*strike, r, sigma, time);
	p.optionPrice[0] = vBS[0];
	p.strikePrice[0] = (2 - rateHawk1)*strike;
	p.optionPrice[1] = vBS[1];
	p.strikePrice[1] = rateHawk1*strike;
	p.optionPrice[2] = vBS[2];
	p.strikePrice[2] = (2 - rateHawk2)*strike;
	p.optionPrice[3] = vBS[3];
	p.strikePrice[3] = rateHawk2*strike;

	Hawk *hk = new Hawk(&p);
	hk->setAsset(pm->getDaily(start + circle - 1).price);
	cout <<time<< "年期鹰式套利组合价格收益" << hk->ArbitENow() << endl;
	outFile <<" " << hk->ArbitENow(of) <<" ";
	//跨式套利 由于波动率的平稳  显然跨式套利没有收益
	memset(&p, 0, sizeof(p));
	vBS[0] = bs->option_price_call_BlackScholes(st, strike, r, sigma, time);
	vBS[1] = bs->option_price_put_BlackScholes(st, strike, r, sigma, time);
	p.optionPrice[0] = vBS[0];
	p.strikePrice[0] = strike;
	p.optionPrice[1] = vBS[1];
	p.strikePrice[1] = strike;

	Straddle *stra = new Straddle(&p);
	stra->setAsset(pm->getDaily(start + circle - 1).price);
	cout <<time<< "年期跨式套利组合价格收益" << stra->ArbitENow() << endl;
	outFile <<" " << stra->ArbitENow(of) <<" ";
	//宽跨式套利 strangle
	memset(&p, 0, sizeof(p));
	double rateStra = 0.8;
	vBS[0] = bs->option_price_call_BlackScholes(st, (2 - rateStra)*strike, r, sigma, time);
	vBS[1] = bs->option_price_put_BlackScholes(st, rateStra*strike, r, sigma, time);
	p.optionPrice[0] = vBS[0];
	p.strikePrice[0] = strike;
	p.optionPrice[1] = vBS[1];
	p.strikePrice[1] = strike;

	Straddle *straB = new Straddle(&p);
	straB->setAsset(pm->getDaily(start + circle - 1).price);
	cout <<time<< "年期宽跨式套利组合价格收益" << straB->ArbitENow() << endl;
	outFile <<" " << straB->ArbitENow(of)<<" ";
	outFile.close();
}*/

void RunOption(double a, double b, double c, int circle, int start){
	PriceManager *pm = new PriceManager();
	BlackScholes *bs = new BlackScholes();
	double sigma = bs->getSigma(a, b, c, circle);
	Price p;
	double strike = pm->getStrike(start, 5);
	double st = pm->getPrice(start);//期权合约首天价格
	//	cout<<pm->getDaily(3630).date<<endl;
	cout << pm->getDaily(3388).date << endl;
	double vBS[4];
	double time = (double)circle / 242;
	char fName[32] = { 0, };
	//该部分用于处理txt
	sprintf_s(fName, "re%d", start);
	string fn = fName;
	fn += ".txt";
	ofstream outFile;
	outFile.open(fn);
	ofstream &of = outFile;
	if (!outFile.is_open())
	{
		cout << "cannot open the file" << endl;
		exit(EXIT_FAILURE);
	}
	//Bufferfly套利 索引:0 1 表示一高一低 2表示中间价 该期权策略适合于市场波动率变化较小时的情况
	double rateBuffer = 0.8;//由rate知道 价差越大即rate越大 套利越多
	vBS[0] = bs->option_price_call_BlackScholes(st, (2 - rateBuffer)*strike, r, sigma, time);
	vBS[1] = bs->option_price_call_BlackScholes(st, rateBuffer*strike, r, sigma, time);
	vBS[2] = bs->option_price_call_BlackScholes(st, strike, r, sigma, time);
	p.optionPrice[0] = vBS[0];
	p.strikePrice[0] = (2 - rateBuffer)*strike;
	p.optionPrice[1] = vBS[1];
	p.strikePrice[1] = rateBuffer*strike;
	p.optionPrice[2] = vBS[2];
	p.strikePrice[2] = strike;

	BufferFly *bf = new BufferFly(&p);
	bf->setAsset(pm->getDaily(start + circle - 1).price);
	cout << "st: " << st << " strike: " << strike << " sigma: " << sigma << endl;
	outFile << "st: " << st << " strike: " << strike << " sigma: " << sigma << endl;
	cout << time << "年期蝶式套利组合价格收益:" << bf->ArbitENow() << endl;
	outFile << time << "年期蝶式套利组合价格收益:" << bf->ArbitENow(of) << endl;
	outFile << endl;
	//Hawk套利 鹰式套利  类似于蝶式  0 1表示一高一低 2 3表示一高一低的中间价 x0+x1=x2+x3
	//鹰式套利比蝶式的好处为可以接受波动率的微小变化 但是收益可能较低
	memset(&p, 0, sizeof(p));
	double rateHawk1 = 0.5;//该比率为x0 x3对应比率 r1<r2
	double rateHawk2 = 0.8;
	vBS[0] = bs->option_price_call_BlackScholes(st, 0.8*strike, r, sigma, time);
	vBS[1] = bs->option_price_call_BlackScholes(st, 1.2*strike, r, sigma, time);
	vBS[2] = bs->option_price_call_BlackScholes(st, 0.9*strike, r, sigma, time);
	vBS[3] = bs->option_price_call_BlackScholes(st, 1.1*strike, r, sigma, time);
	p.optionPrice[0] = vBS[0];
	p.strikePrice[0] = 0.8*strike;
	p.optionPrice[1] = vBS[1];
	p.strikePrice[1] = 1.2*strike;
	p.optionPrice[2] = vBS[2];
	p.strikePrice[2] = 0.9*strike;
	p.optionPrice[3] = vBS[3];
	p.strikePrice[3] = 1.1*strike;

	Hawk *hk = new Hawk(&p);
	hk->setAsset(pm->getDaily(start + circle - 1).price);
	cout << time << "年期鹰式套利组合价格收益" << hk->ArbitENow() << endl;
	outFile << time << "年期鹰式套利组合价格收益" << hk->ArbitENow(of) << endl;
	outFile << endl;
	//跨式套利 由于波动率的平稳  显然跨式套利没有收益
	memset(&p, 0, sizeof(p));
	vBS[0] = bs->option_price_call_BlackScholes(st, strike, r, sigma, time);
	vBS[1] = bs->option_price_put_BlackScholes(st, strike, r, sigma, time);
	p.optionPrice[0] = vBS[0];
	p.strikePrice[0] = strike;
	p.optionPrice[1] = vBS[1];
	p.strikePrice[1] = strike;

	Straddle *stra = new Straddle(&p);
	stra->setAsset(pm->getDaily(start + circle - 1).price);
	cout << time << "年期跨式套利组合价格收益" << stra->ArbitENow() << endl;
	outFile << time << "年期跨式套利组合价格收益" << stra->ArbitENow(of) << endl;
	outFile << endl;
	//宽跨式套利 strangle
	memset(&p, 0, sizeof(p));
	double rateStra = 0.8;
	vBS[0] = bs->option_price_call_BlackScholes(st, (2 - rateStra)*strike, r, sigma, time);
	vBS[1] = bs->option_price_put_BlackScholes(st, rateStra*strike, r, sigma, time);
	p.optionPrice[0] = vBS[0];
	p.strikePrice[0] = strike;
	p.optionPrice[1] = vBS[1];
	p.strikePrice[1] = strike;

	Straddle *straB = new Straddle(&p);
	straB->setAsset(pm->getDaily(start + circle - 1).price);
	cout << time << "年期宽跨式套利组合价格收益" << straB->ArbitENow() << endl;
	outFile << time << "年期宽跨式套利组合价格收益" << straB->ArbitENow(of) << endl;
	outFile << endl;
	outFile.close();
}

//计算历史波动率
void RunSigma(double a){
	ofstream outFile;
	outFile.open("sigma.txt", ios::app);
	a =a*pow(242,0.5);
	outFile<<a<<endl;
	outFile.close();
}

//隐含波动率的套利
//以t日内均价所得价格为波动率的套利
/*void RunImpliedOption(int t,int circle,int start){
	PriceManager *pm = new PriceManager();
	BlackScholes *bs = new BlackScholes();
	//double sigma = bs->getSigma(a, b, c, circle);
	Price p;
	double strike = pm->getStrike(start, 5);
	double st = pm->getPrice(start);//期权合约首天价格
	double vBS[4];
	double time = (double)circle / 242;
	//double sigma_call = bs->option_price_implied_volatility_call_BlackScholes_newton(st, strike, r,time, );
	//Bufferfly套利 索引:0 1 表示一高一低 2表示中间价 该期权策略适合于市场波动率变化较小时的情况
	double rateBuffer = 0.8;//由rate知道 价差越大即rate越大 套利越多
	vBS[0] = bs->option_price_call_BlackScholes(st, (2 - rateBuffer)*strike, r, sigma, time);
	vBS[1] = bs->option_price_call_BlackScholes(st, rateBuffer*strike, r, sigma, time);
	vBS[2] = bs->option_price_call_BlackScholes(st, strike, r, sigma, time);
	p.optionPrice[0] = vBS[0];
	p.strikePrice[0] = (2 - rateBuffer)*strike;
	p.optionPrice[1] = vBS[1];
	p.strikePrice[1] = rateBuffer*strike;
	p.optionPrice[2] = vBS[2];
	p.strikePrice[2] = strike;

	BufferFly *bf = new BufferFly(&p);
	bf->setAsset(pm->getDaily(start + circle - 1).price);
	cout << "st: " << st << " strike: " << strike << " sigma: " << sigma << endl;
	cout << time << "年期蝶式套利价格收益:" << bf->ArbitENow() << endl;

	//Hawk套利 鹰式套利  类似于蝶式  0 1表示一高一低 2 3表示一高一低的中间价 x0+x3=x1+x2
	//鹰式套利比蝶式的好处为可以接受波动率的微小变化 但是收益可能较低
	memset(&p, 0, sizeof(p));
	double rateHawk1 = 0.5;//该比率为x0 x3对应比率 r1<r2
	double rateHawk2 = 0.8;
	vBS[0] = bs->option_price_call_BlackScholes(st, (2 - rateHawk1)*strike, r, sigma, time);
	vBS[1] = bs->option_price_call_BlackScholes(st, rateHawk1*strike, r, sigma, time);
	vBS[2] = bs->option_price_call_BlackScholes(st, (2 - rateHawk2)*strike, r, sigma, time);
	vBS[3] = bs->option_price_call_BlackScholes(st, rateHawk2*strike, r, sigma, time);
	p.optionPrice[0] = vBS[0];
	p.strikePrice[0] = (2 - rateHawk1)*strike;
	p.optionPrice[1] = vBS[1];
	p.strikePrice[1] = rateHawk1*strike;
	p.optionPrice[2] = vBS[2];
	p.strikePrice[2] = (2 - rateHawk2)*strike;
	p.optionPrice[3] = vBS[3];
	p.strikePrice[3] = rateHawk2*strike;

	Hawk *hk = new Hawk(&p);
	hk->setAsset(pm->getDaily(start + circle - 1).price);
	cout << time << "年期鹰式套利价格收益" << hk->ArbitENow() << endl;

	//跨式套利 由于波动率的平稳  显然跨式套利没有收益
	memset(&p, 0, sizeof(p));
	vBS[0] = bs->option_price_call_BlackScholes(st, strike, r, sigma, time);
	vBS[1] = bs->option_price_put_BlackScholes(st, strike, r, sigma, time);
	p.optionPrice[0] = vBS[0];
	p.strikePrice[0] = strike;
	p.optionPrice[1] = vBS[1];
	p.strikePrice[1] = strike;

	Straddle *stra = new Straddle(&p);
	stra->setAsset(pm->getDaily(start + circle - 1).price);
	cout << time << "年期跨式套利价格收益" << stra->ArbitENow() << endl;

	//宽跨式套利 strangle
	memset(&p, 0, sizeof(p));
	double rateStra = 0.8;
	vBS[0] = bs->option_price_call_BlackScholes(st, (2 - rateStra)*strike, r, sigma, time);
	vBS[1] = bs->option_price_put_BlackScholes(st, rateStra*strike, r, sigma, time);
	p.optionPrice[0] = vBS[0];
	p.strikePrice[0] = strike;
	p.optionPrice[1] = vBS[1];
	p.strikePrice[1] = strike;

	Straddle *straB = new Straddle(&p);
	straB->setAsset(pm->getDaily(start + circle - 1).price);
	cout << time << "年期宽跨式套利价格收益" << straB->ArbitENow() << endl;
}*/

