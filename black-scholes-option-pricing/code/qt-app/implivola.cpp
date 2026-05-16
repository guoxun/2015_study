#include "implivola.h"
#include "ui_implivola.h"
#include "QMessageBox"
ImpliVola::ImpliVola(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImpliVola)
{
    ui->setupUi(this);
     connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(close()));
     bs = new BlackScholes();
      this->setWindowTitle("Implied volatility");
}

ImpliVola::~ImpliVola()
{
    delete ui;
}

double ImpliVola::RunImplied(double s,double x,double r,double t,double optionPrice,int type){
    if(type == 0){
     return  bs->option_price_implied_volatility_call_BlackScholes_newton(s,x,r,t,optionPrice);
    }else if(type == 1){
     return bs->option_price_implied_volatility_put_BlackScholes_newton(s,x,r,t,optionPrice);
    }
}

void ImpliVola::on_pushButton_clicked()
{
    //首先进行参数分析  再决定是否套利
    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty()||ui->lineEdit_3->text().isEmpty()
            ||ui->lineEdit_4->text().isEmpty()||ui->lineEdit_5->text().isEmpty()){
        QMessageBox box;
        box.setWindowTitle(tr("Warning"));
        box.setIcon(QMessageBox::Warning);
        box.setText("Empty.Please check,again");
        box.setStandardButtons(QMessageBox::Ok);
        box.show();
        if(box.exec()==QMessageBox::Ok){

        }
    }else{
        if(!ui->radioButton->isChecked()&&!ui->radioButton_2->isChecked()){
            QMessageBox box;
            box.setWindowTitle(tr("Warning"));
            box.setIcon(QMessageBox::Warning);
            box.setText("Option style is empty.Please check,again");
            box.setStandardButtons(QMessageBox::Ok);
            box.show();
            if(box.exec()==QMessageBox::Ok){

            }}else{
        if(ui->radioButton->isChecked()){
          double result = this->RunImplied(ui->lineEdit->text().toDouble(),
                            ui->lineEdit_2->text().toDouble(),
                            ui->lineEdit_3->text().toDouble(),
                            ui->lineEdit_4->text().toDouble(),
                            ui->lineEdit_5->text().toDouble(),0);
            QMessageBox box;
            box.setWindowTitle(tr("Result"));
            box.setIcon(QMessageBox::Information);
            box.setText( QString::number(result,'f',5));
            box.setStandardButtons(QMessageBox::Ok);
            box.show();
            if(box.exec()==QMessageBox::Ok){

            }
        }
        if(ui->radioButton_2->isChecked()){
           double result = this->RunImplied(ui->lineEdit->text().toDouble(),
                            ui->lineEdit_2->text().toDouble(),
                            ui->lineEdit_3->text().toDouble(),
                            ui->lineEdit_4->text().toDouble(),
                            ui->lineEdit_5->text().toDouble(),1);
           QMessageBox box;
           box.setWindowTitle(tr("Result"));
           box.setIcon(QMessageBox::Information);
           box.setText( QString::number(result,'f',5));
           box.setStandardButtons(QMessageBox::Ok);
           box.show();
           if(box.exec()==QMessageBox::Ok){

           }
        }
            }
    }
    //this->~ImpliVola();
}

