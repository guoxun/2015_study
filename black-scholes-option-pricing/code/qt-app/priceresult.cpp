#include "priceresult.h"
#include "ui_priceresult.h"

PriceResult::PriceResult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PriceResult)
{
    ui->setupUi(this);
}

PriceResult::~PriceResult()
{
    delete ui;
}
