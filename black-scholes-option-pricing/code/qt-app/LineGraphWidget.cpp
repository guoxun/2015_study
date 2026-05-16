#include "linegraphwidget.h"

LineGraphWidget::LineGraphWidget(int mix, int max, QWidget *parent) :

QWidget(parent),

rangeMin(mix),

rangeMax(max),

singleUnitX(10),

timer(NULL)

{

connect(this,SIGNAL(newDataCome()),this,SLOT(update()));

}

void LineGraphWidget::paintEvent(QPaintEvent *)

{

int i;

int a,b,c,d;

Counter = 0;

QPixmap pix(widgetX,widgetY);

QPainter painter(&pix);

pix.fill(Qt::white);

QPen pen;

pen.setBrush(Qt::darkGreen);

painter.setPen(pen);

QPen pen1;

pen1.setBrush(Qt::black);

painter.setRenderHint(QPainter::Antialiasing);

for(i = 0;i < sumUnitX; ++i) {

a = basePoint.x + i*singleUnitX;

b = basePoint.y;

c= a;

d = yAxisEndPoint.y;

painter.drawLine(a,b,c,d);

}

for(i = 0; i < sumUnitY+1; ++i) {

a = basePoint.x;

b = basePoint.y - singleUnitY*i;

c = xAxisEndPoint.x;

d = b;

painter.drawLine(a,b,c,d);

}

painter.setPen(pen1);

for (i = 0;i < (sumUnitY/2+1);++i) {

QRectF rectF(0,basePoint.y - singleUnitY*i*2 - 5,basePoint.x,(widgetY - basePoint.y)/3);

painter.drawText(rectF,Qt::AlignCenter,QString::number(rangeMin+i*20));

}

for (i = 0;i < sumUnitX/5;++i) {

QRectF rectF(basePoint.x+singleUnitX*(5*i+3),basePoint.y,singleUnitX*4,widgetY-basePoint.y);

painter.drawText(rectF,Qt::AlignCenter,QString::number(5*(i+1)*singleUnitX));

}

int size = xList.size();

while(Counter < (size -1)) {

painter.drawLine(basePoint.x + xList.value(Counter)*singleUnitX,

basePoint.y - ((yList.value(Counter) - rangeMin)/10.0)*singleUnitY,

basePoint.x + xList.value(Counter + 1)*singleUnitX,

basePoint.y - ((yList.value(Counter + 1) - rangeMin)/10.0)*singleUnitY);

Counter++;

}

painter.end();

painter.begin(this);

painter.drawPixmap(0,0,pix);

painter.end();

Counter = 0;

}

void LineGraphWidget::resizeEvent(QResizeEvent * e)

{

if (e->oldSize() != size()) {

initGraph();

}

}

void LineGraphWidget::showData(const int &data)

{

//这里还要进行相关的数据正确性检测，不能超过图的范围

if (data > rangeMax || data < rangeMin) {

return;

}

int xsize = xList.size();

int yValue = data;

if (xsize == sumUnitX) {

xList.clear();

yList.clear();

xsize = xList.length();

}

if (xsize >= 1) {

xList.insert(xsize,xsize);

yList.insert(xsize,yValue);

emit newDataCome();

}

else {

xList.insert(xsize,xsize);

yList.insert(xsize,yValue);

}

}

void LineGraphWidget::getSumUnitX()

{

if ((static_cast<int>(widgetX*0.85))%singleUnitX == 0){

sumUnitX = (static_cast<int>(widgetX*0.85))/singleUnitX;

}

else {

sumUnitX = (static_cast<int>(widgetX*0.85))/singleUnitX + 1;

}

}

void LineGraphWidget::getSingleUnitY()

{

int range = rangeMax - rangeMin;

if (range == 0) {

sumUnitY = range/10 + 1;

}

else {

sumUnitY = range/10 + 2;

}

singleUnitY = (static_cast<int>(widgetY*0.85))/sumUnitY;

}

void LineGraphWidget::initGraph()

{

widgetX = size().width();

widgetY = size().height();

basePoint.x = static_cast<int>(widgetX*0.1);

basePoint.y = static_cast<int>(widgetY*0.9);

yAxisEndPoint.x = basePoint.x;

yAxisEndPoint.y = static_cast<int>(widgetY*0.05);

xAxisEndPoint.x = static_cast<int>(widgetX*0.95);

xAxisEndPoint.y = basePoint.y;

getSumUnitX();

getSingleUnitY();

}

void LineGraphWidget::startUpdataTimer(int msecond)

{

if (timer != NULL) {

if (timer->isActive()) {

timer->stop();

}

delete timer;

}

disconnect(SIGNAL(newDataCome()));

timer = new QTimer;

connect(timer,SIGNAL(timeout()),this,SLOT(update()));

timer->start(msecond);

}

void LineGraphWidget::stopUpdataTimer()

{

if (timer == NULL) {

}

else {

timer->stop();

delete timer;

}

}
