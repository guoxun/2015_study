#ifndef OPTIONPRICE_H
#define OPTIONPRICE_H

#include <QWidget>
#include "PriceControl.h"
namespace Ui {
class OptionPrice;
}

class OptionPrice : public QWidget
{
    Q_OBJECT
    
public:
    explicit OptionPrice(QWidget *parent = 0);
    ~OptionPrice();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::OptionPrice *ui;
};

#endif // OPTIONPRICE_H
