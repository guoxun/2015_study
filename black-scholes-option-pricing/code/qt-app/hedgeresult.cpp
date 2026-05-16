#include "hedgeresult.h"
#include "ui_hedgeresult.h"

HedgeResult::HedgeResult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HedgeResult)
{
    ui->setupUi(this);
    this->setWindowTitle("HedgeResult");
}

HedgeResult::~HedgeResult()
{
    delete ui;
}

void HedgeResult::setPara(double Delta, double Gamma, double Theta, double Vega, double Rho){
   //ui->label_Price_1->setText(QString::number(price1,'f',5));
    ui->label_6->setText(QString::number(Delta,'f',5));
    ui->label_7->setText(QString::number(Gamma,'f',5));
    ui->label_8->setText(QString::number(Theta,'f',5));
    ui->label_9->setText(QString::number(Vega,'f',5));
    ui->label_10->setText(QString::number(Rho,'f',5));
}
