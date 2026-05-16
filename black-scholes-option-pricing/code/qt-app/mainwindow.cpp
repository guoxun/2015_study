#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "PriceControl.h"
#include "implivola.h"
#include "optionprice.h"
#include "hedgepara.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pm = new PriceManager();
    bs = new BlackScholes();
    this->setWindowTitle("BlackScholesOption");
    this->setStyleSheet("background::yellow");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_I_triggered()
{
    QString fileName = QFileDialog::getOpenFileName//获得要打开的文件名
                       (this,"open file",NULL,"(*.txt)"";;All File(*.*)"";;(*.html)");
    pm->initAlldays(fileName);
    //读取完之后  显示一下起始 结束年代 以及重要参数
}

void MainWindow::on_actionGarch_triggered()
{
    gw = new GarchWidget();
    gw->setPm(this->pm);
    gw->show();
}

void MainWindow::on_action_H_triggered()
{
    //调用隐含波动率计算
    ImpliVola *im = new ImpliVola();
    im->show();
}

void MainWindow::on_actionGarch_P_triggered()
{
    //只定价  不套利
    OptionPrice *op = new OptionPrice();
    op->show();
}

void MainWindow::on_action_Q_triggered()
{
    this->close();
}

void MainWindow::on_action_E_triggered()
{
    HedgePara *hp = new HedgePara();
    hp->show();
}
