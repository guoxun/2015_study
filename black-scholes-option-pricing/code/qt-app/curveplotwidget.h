#ifndef CURVEPLOTWIDGET_H
#define CURVEPLOTWIDGET_H

#include <QWidget>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_layout.h>
namespace Ui {
class curvePlotWidget;
}

class curvePlotWidget : public QwtPlot
{
    Q_OBJECT
    
public:
    explicit curvePlotWidget( Qwidget *parent = 0);
    ~curvePlotWidget();
    
private:
    Ui::curvePlotWidget *ui;
};

#endif // CURVEPLOTWIDGET_H
