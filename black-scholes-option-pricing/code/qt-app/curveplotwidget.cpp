#include "curveplotwidget.h"
#include "ui_curveplotwidget.h"
#include <qwt_plot.h>
curvePlotWidget::curvePlotWidget(Qwidget *parent) :
    QwtPlot(parent),
    ui(new Ui::curvePlotWidget)
{
    ui->setupUi(this);

    //设置一些窗口熟悉
     setFrameStyle(QFrame::NoFrame);
     setLineWidth(0);
     setCanvasLineWidth(2);

     plotLayout()->setAlignCanvasToScales(true);

    //增加网格

     QwtPlotGrid *grid = new QwtPlotGrid;
     grid->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));
     grid->attach(this);

    //设置画布背景

     setCanvasBackground(QColor(29, 100, 141)); // nice blue

    //设置X与Y坐标范围
     setAxisScale(xBottom, 1, 75);
     setAxisScale(yLeft, -1, 1);

    //


    //新建一个曲线对象
     QwtPlotCurve *pCurve=new QwtPlotCurve("curve1");



    //输入数据

     QVector< double > xData;

     QVector<double> yData;

     for(int i=0;i<75;++i)
      xData.push_back(i+1);

     yData<<1<<0.048634<<0.655211<<0.320122<<0.130912<<0.182503<<0.163217<<0.167857<<0.169706<<0.15244<<0.17136<<0.184516<<0.183185<<0.16788<<0.150819<<0.154223<<0.149134<<0.126398<<0.090325<<-0.017047<<0.184973<<0.113727<<0.072852<<0.054324<<0.04943<<0.036473<<0.042876<<0.048972<<0.04963<<0.052114<<0.056796<<0.060517<<0.07844<<0.066472<<0.079221<<0.06061<<-0.018855<<0.457584<<0.104125<<0.282665<<0.066127<<0.064099<<0.065944<<0.013025<<0.054401<<0.027663<<0.038911<<0.03153<<0.040123<<0.038832<<0.03919<<0.048258<<0.050396<<0.063897<<0.062202<<0.067778<<0.074743<<0.063545<<0.066624<<0.09162<<-0.022548<<0.037526<<0.04687<<0.04425<<0.046449<<0.038345<<0.051492<<0.033624<<0.030668<<0.075395<<-0.016367<<-0.039846<<0.021928<<0<<0;


     pCurve->setSamples(xData,yData);

     pCurve->attach(this);

    //设置曲线颜色
     QPen pen;
     pen.setColor(QColor(255,0,0));

     pCurve->setPen(pen);

     //QwtPlotCurve::PaintAttribute

    //抗锯齿

    pCurve->setRenderHint(QwtPlotItem::RenderAntialiased,true);



    //增加缩放功能

    QwtPlotZoomer  *pZoomer= new QwtPlotZoomer(canvas());

    pZoomer->setRubberBandPen(QPen(Qt::red));

    //重绘

    replot();
}

curvePlotWidget::~curvePlotWidget()
{
    delete ui;
}
