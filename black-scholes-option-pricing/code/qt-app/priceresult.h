#ifndef PRICERESULT_H
#define PRICERESULT_H

#include <QWidget>

namespace Ui {
class PriceResult;
}

class PriceResult : public QWidget
{
    Q_OBJECT
    
public:
    explicit PriceResult(QWidget *parent = 0);
    ~PriceResult();
    
private:
    Ui::PriceResult *ui;
};

#endif // PRICERESULT_H
