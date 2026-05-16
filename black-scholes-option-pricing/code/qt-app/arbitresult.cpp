#include "arbitresult.h"
#include "ui_arbitresult.h"

ArbitResult::ArbitResult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArbitResult)
{
    ui->setupUi(this);
    this->setWindowTitle("Arbit");
}

ArbitResult::~ArbitResult()
{
    delete ui;
}

void ArbitResult::setArbit(QString start, QString end, double price1, double price2, double price3, double price4){
  ui->label_3->setText(start);
  ui->label_4->setText(end);
  ui->label_Price_1->setText(QString::number(price1,'f',5));
  ui->label_Price_2->setText(QString::number(price2,'f',5));
  ui->label_Price_3->setText(QString::number(price3,'f',5));
  ui->label_Price_4->setText(QString::number(price4,'f',5));
}
