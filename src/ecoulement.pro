#-------------------------------------------------
#
# Project created by QtCreator 2015-10-09T09:16:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ecoulement
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        dtm.cpp \
    gldisplay.cpp

HEADERS  += mainwindow.h \
            openf.h \
            dtm.h \
    gldisplay.h

FORMS    += mainwindow.ui
