/*************************************************

折线图绘制

author 周翔

2012-8-14

used by GuoXun

2015-4-29
*************************************************/

#ifndef LINEGRAPHWIDGET_H

#define LINEGRAPHWIDGET_H

#include <QWidget>

#include <QPen>

#include <QPainter>

#include <QResizeEvent>

#include <QTimer>

struct point

{

int x;

int y;

};

class LineGraphWidget : public QWidget

{

Q_OBJECT

public:

//mix和max为折线的y轴的数据范围

explicit LineGraphWidget(int mix, int max,QWidget *parent = 0);

void setSingleUnitX(const int &value) {singleUnitX = value;}//设置x轴的一个单位占多少个像素，默认10

//支持定时器定时更新

void startUpdataTimer(int msecond);

void stopUpdataTimer();

signals:

//支持实时更新

void newDataCome();//有新数据到来时发出的折线图更新信号

public slots:

void showData(const int &data);//接受来之外部的数据

protected:

void paintEvent(QPaintEvent *);

void resizeEvent(QResizeEvent *e);

private:

QList<int> xList;

QList<int> yList;

int Counter;

//窗口的长和宽

int widgetX;

int widgetY;

//折线图的显示范围

int rangeMin;

int rangeMax;

//根据窗口和范围得出的单位长，x轴,y轴

int singleUnitX;

int singleUnitY;

//根据窗口得出的多少个单位,x轴,y轴

int sumUnitX;

int sumUnitY;

point basePoint;//折线图的原点

point xAxisEndPoint;//X轴的终点

point yAxisEndPoint;//Y轴的终点

QTimer *timer;

void getSumUnitX();

void getSingleUnitY();

void initGraph();

};

#endif // LINEGRAPHWIDGET_H

