#include "optionprice.h"
#include "ui_optionprice.h"
#include "QMessageBox"
OptionPrice::OptionPrice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionPrice)
{
    ui->setupUi(this);
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(close()));
    this->setWindowTitle("OptionPrice");
    this->setWindowIcon(QIcon(QPixmap(":/images/money.png")));
}

OptionPrice::~OptionPrice()
{
    delete ui;
}

void OptionPrice::on_pushButton_clicked()
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
        BlackScholes *bs = new BlackScholes();
    double callPrice =  bs->option_price_call_BlackScholes(ui->lineEdit->text().toDouble(),ui->lineEdit_2->text().toDouble(),
                                           ui->lineEdit_3->text().toDouble(),ui->lineEdit_4->text().toDouble(),
                                           ui->lineEdit_5->text().toDouble());
    double putPrice  =  bs->option_price_put_BlackScholes(ui->lineEdit->text().toDouble(),ui->lineEdit_2->text().toDouble(),
                                                          ui->lineEdit_3->text().toDouble(),ui->lineEdit_4->text().toDouble(),
                                                          ui->lineEdit_5->text().toDouble());

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
        QMessageBox box;
        box.setWindowTitle(tr("Result"));
        box.setIcon(QMessageBox::Information);
        box.setText( QString::number(callPrice,'f',5));
        box.setStandardButtons(QMessageBox::Ok);
        box.show();
        if(box.exec()==QMessageBox::Ok){

        }
    }
    if(ui->radioButton_2->isChecked()){
        QMessageBox box;
        box.setWindowTitle(tr("Result"));
        box.setIcon(QMessageBox::Information);
        box.setText( QString::number(putPrice,'f',5));
        box.setStandardButtons(QMessageBox::Ok);
        box.show();
        if(box.exec()==QMessageBox::Ok){

        }
    }
    }
    }
}
