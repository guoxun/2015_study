#ifndef HEDGEPARA_H
#define HEDGEPARA_H

#include <QWidget>

namespace Ui {
class HedgePara;
}

class HedgePara : public QWidget
{
    Q_OBJECT
    
public:
    explicit HedgePara(QWidget *parent = 0);
    ~HedgePara();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::HedgePara *ui;
};

#endif // HEDGEPARA_H
