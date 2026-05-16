#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "PriceControl.h"
#include "garchwidget.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_I_triggered();

    void on_actionGarch_triggered();

    void on_action_H_triggered();

    void on_actionGarch_P_triggered();

    void on_action_Q_triggered();

    void on_action_E_triggered();

private:
    Ui::MainWindow *ui;
    PriceManager *pm;
    BlackScholes *bs;
    GarchWidget *gw;
};

#endif // MAINWINDOW_H
