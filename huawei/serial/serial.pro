#-------------------------------------------------
#
# Project created by QtCreator 2016-04-22T23:47:22
#
#-------------------------------------------------

QT       += core gui
QT +=network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = serial
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    win_qextserialport.cpp \
    qextserialport.cpp \
    qextserialbase.cpp

HEADERS  += mainwindow.h \
    win_qextserialport.h \
    qextserialport.h \
    qextserialbase.h

FORMS    += mainwindow.ui
