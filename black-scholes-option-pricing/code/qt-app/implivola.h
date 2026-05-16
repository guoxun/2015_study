#ifndef IMPLIVOLA_H
#define IMPLIVOLA_H

#include <QWidget>
#include "PriceControl.h"
namespace Ui {
class ImpliVola;
}

class ImpliVola : public QWidget
{
    Q_OBJECT
    
public:
    explicit ImpliVola(QWidget *parent = 0);
    ~ImpliVola();
    double RunImplied(double s,double x,double r,double t,double optionPrice,int type);//type 0 看涨期权 type 1 看跌期权
    BlackScholes *bs;
private slots:
    void on_pushButton_clicked();

private:
    Ui::ImpliVola *ui;
};

#endif // IMPLIVOLA_H
