#include <QtGui/QApplication>
#include <QTextCodec>
#include <QMotifStyle>
#include "mainwindow.h"
//#include <curveplotwidget.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("cleanlooks");
    MainWindow w;
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    w.show();
    return a.exec();
}
