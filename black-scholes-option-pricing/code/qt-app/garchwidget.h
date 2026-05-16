#ifndef GARCHWIDGET_H
#define GARCHWIDGET_H

#include <QWidget>
#include "PriceControl.h"
namespace Ui {
class GarchWidget;
}

class GarchWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit GarchWidget(QWidget *parent = 0);
    ~GarchWidget();
    void RunOption(double a,double b,double c,int circle,int start,double r);
    void setPm(PriceManager *p);
private slots:

    void on_pushButton_clicked();

private:
    Ui::GarchWidget *ui;
    PriceManager *pmg;
};

#endif // GARCHWIDGET_H
