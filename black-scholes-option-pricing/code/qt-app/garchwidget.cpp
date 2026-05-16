#include "garchwidget.h"
#include "ui_garchwidget.h"
#include "iostream"
#include "arbitresult.h"
#include "QMessageBox"
using namespace std;

GarchWidget::GarchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GarchWidget)
{
    ui->setupUi(this);
  //  ui->nextButtonF->setEnabled(!ui->lineEditF->text().isEmpty());
  //  connect(ui->lineEditF,SIGNAL(textChanged(QString)),this,SLOT(button_Enabed()));
  //  connect(ui->pushButton,SIGNAL(clicked()),this,SLOT());
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(close()));
    this->setWindowTitle("GarchOption");
}

GarchWidget::~GarchWidget()
{
    delete ui;
}

void GarchWidget::RunOption(double a,double b,double c,int circle,int start,double r){
        PriceManager *pm = this->pmg;
        BlackScholes *bs = new BlackScholes();
        ArbitResult *ar = new ArbitResult();
        double sigma = bs->getSigma(a, b, c, circle);
        Price p;
        double strike = pm->getStrike(start, 5);
        double st = pm->getPrice(start);//期权合约首天价格
        //期权起止时间
        QString Start = pm->getDaily(start).date;
        QString End = pm->getDaily(start + circle).date;
        double vBS[4];
        double time = (double)circle / 242;
        //double r = 0.0275;//年化利率
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
        cout << time<<"年期蝶式套利组合价格收益:" << bf->ArbitENow() << endl;
        double price1 = bf->ArbitENow();
        //Hawk套利 鹰式套利  类似于蝶式  0 1表示一高一低 2 3表示一高一低的中间价 x0+x3=x1+x2
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
        cout <<time<< "年期鹰式套利组合价格收益" << hk->ArbitENow() << endl;
        double price2 = hk->ArbitENow();
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
        double price3 = stra->ArbitENow();
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
        double price4 = straB->ArbitENow();

        ar->setArbit(Start,End,price1,price2,price3,price4);
        ar->show();
}

void GarchWidget::on_pushButton_clicked()
{
    //首先进行参数分析  再决定是否套利
    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty()||ui->lineEdit_3->text().isEmpty()
            ||ui->lineEdit_4->text().isEmpty()||ui->lineEdit_5->text().isEmpty()||
            ui->lineEdit_6->text().isEmpty()){
        QMessageBox box;
        box.setWindowTitle(tr("Warning"));
        box.setIcon(QMessageBox::Warning);
        box.setText("Empty.Please check,again");
        box.setStandardButtons(QMessageBox::Ok);
        box.show();
        if(box.exec()==QMessageBox::Ok){

        }
    }else{
        this->RunOption(ui->lineEdit->text().toDouble(),
                        ui->lineEdit_2->text().toDouble(),
                        ui->lineEdit_3->text().toDouble(),
                        ui->lineEdit_4->text().toInt(),
                        ui->lineEdit_5->text().toInt(),
                        ui->lineEdit_6->text().toDouble());
    }
   // this->~GarchWidget();
}

void GarchWidget::setPm(PriceManager *p)
{
 this->pmg = p;
}
