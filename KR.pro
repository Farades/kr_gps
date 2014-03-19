#-------------------------------------------------
#
# Project created by QtCreator 2014-03-19T00:08:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KR
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    RightSidesC.cpp \
    myVector.cpp

HEADERS  += mainwindow.h \
    LSM.h \
    OscularToDecart.h \
    RandG.h \
    RightSides.h \
    Row.h \
    Simulator.h \
    User.h \
    White_noise.h \
    matrix.h \
    myVector.h

FORMS    += mainwindow.ui

CONFIG   += qwt

OTHER_FILES += \
    KR.res \
    KR.cbproj \
    KR.cbproj.local \
    KR.pro.user \
    main.dfm
