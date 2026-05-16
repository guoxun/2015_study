#-------------------------------------------------
#
# Project created by QtCreator 2015-04-15T14:17:15
#
#-------------------------------------------------

QT       += core gui

TARGET = BlackScholes_01
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    PriceControl.cpp \
    garchwidget.cpp \
    hedgepara.cpp \
    arbitresult.cpp \
    implivola.cpp \
    optionprice.cpp \
    priceresult.cpp \
    hedgeresult.cpp

HEADERS  += mainwindow.h \
    PriceControl.h \
    garchwidget.h \
    hedgepara.h \
    arbitresult.h \
    implivola.h \
    optionprice.h \
    priceresult.h \
    hedgeresult.h

FORMS    += mainwindow.ui \
    garchwidget.ui \
    hedgepara.ui \
    arbitresult.ui \
    implivola.ui \
    optionprice.ui \
    priceresult.ui \
    hedgeresult.ui

RESOURCES += \
    menu.qrc

LIBS += -L"F:\Qt\4.8.2\lib" -lqwt
INCLUDEPATH += F:\Qt\4.8.2\include\QWT
include(F:\Qt\qwt-6.0.0\qwt.prf)
