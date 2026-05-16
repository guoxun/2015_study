#include "hedgepara.h"
#include "ui_hedgepara.h"
#include "QMessageBox"
#include "PriceControl.h"
#include "hedgeresult.h"

HedgePara::HedgePara(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HedgePara)
{
    ui->setupUi(this);
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(close()));
    this->setWindowTitle("HedgePara");
}

HedgePara::~HedgePara()
{
    delete ui;
}

void HedgePara::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty()||
     ui->lineEdit_3->text().isEmpty()||ui->lineEdit_4->text().isEmpty()||
            ui->lineEdit_5->text().isEmpty()){
        QMessageBox box;
        box.setWindowTitle(tr("Warning"));
        box.setIcon(QMessageBox::Warning);
        box.setText("Empty.Please check,again");
        box.setStandardButtons(QMessageBox::Ok);
        box.show();
        if(box.exec()==QMessageBox::Ok){

        }
    }else
    {

    if(!ui->radioButton->isChecked()&&!ui->radioButton_2->isChecked()){
        QMessageBox box;
        box.setWindowTitle(tr("Warning"));
        box.setIcon(QMessageBox::Warning);
        box.setText("Option style is empty.Please check,again");
        box.setStandardButtons(QMessageBox::Ok);
        box.show();
        if(box.exec()==QMessageBox::Ok){

        }
    }else{
    if(ui->radioButton->isChecked()){
        BlackScholes *bs = new BlackScholes();
        double Delta,Gamma,Theta,Vega,Rho;
        double s = ui->lineEdit->text().toDouble();
        double x = ui->lineEdit_2->text().toDouble();
        double r = ui->lineEdit_3->text().toDouble();
        double sigma = ui->lineEdit_4->text().toDouble();
        double t = ui->lineEdit_5->text().toDouble();
       bs->option_price_partials_call_BlackSchloes(s,r,x,sigma,t,Delta,Gamma,Theta,Vega,Rho);
       HedgeResult *hr = new HedgeResult();
       hr->setPara(Delta,Gamma,Theta,Vega,Rho);
       hr->show();
    }
    if(ui->radioButton_2->isChecked()){
        BlackScholes *bs = new BlackScholes();
        double Delta,Gamma,Theta,Vega,Rho;
        double s = ui->lineEdit->text().toDouble();
        double x = ui->lineEdit_2->text().toDouble();
        double r = ui->lineEdit_3->text().toDouble();
        double sigma = ui->lineEdit_4->text().toDouble();
        double t = ui->lineEdit_5->text().toDouble();
       bs->option_price_partials_put_BlackSchloes(s,r,x,sigma,t,Delta,Gamma,Theta,Vega,Rho);
       HedgeResult *hr = new HedgeResult();
       hr->setPara(Delta,Gamma,Theta,Vega,Rho);
       hr->show();
    }
    }
    }
}
