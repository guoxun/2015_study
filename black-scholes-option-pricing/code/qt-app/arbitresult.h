#ifndef ARBITRESULT_H
#define ARBITRESULT_H

#include <QWidget>

namespace Ui {
class ArbitResult;
}

class ArbitResult : public QWidget
{
    Q_OBJECT
    
public:
    explicit ArbitResult(QWidget *parent = 0);
    ~ArbitResult();
    void setArbit(QString start,QString end,double price1,double price2,double price3,double price4);
private:
    Ui::ArbitResult *ui;
};

#endif // ARBITRESULT_H
