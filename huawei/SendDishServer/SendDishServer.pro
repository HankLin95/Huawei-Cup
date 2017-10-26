#-------------------------------------------------
#
# Project created by QtCreator 2016-04-19T15:58:37
#
#-------------------------------------------------

QT       += core gui
QT+=network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SendDishServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dbsingleton.cpp

HEADERS  += mainwindow.h \
    dbsingleton.h \
    sqlite3.h

FORMS    += mainwindow.ui
LIBS += -LC:\Qt\qtgui\SendDishServer -lsqlite3
