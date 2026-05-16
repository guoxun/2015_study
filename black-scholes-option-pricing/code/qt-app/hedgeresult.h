#ifndef HEDGERESULT_H
#define HEDGERESULT_H

#include <QWidget>

namespace Ui {
class HedgeResult;
}

class HedgeResult : public QWidget
{
    Q_OBJECT
    
public:
    explicit HedgeResult(QWidget *parent = 0);
    ~HedgeResult();
    void setPara( double Delta,double Gamma,double Theta,double Vega,double Rho);
private:
    Ui::HedgeResult *ui;
};

#endif // HEDGERESULT_H
