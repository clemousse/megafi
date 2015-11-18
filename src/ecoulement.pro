#-------------------------------------------------
#
# Project created by QtCreator 2015-10-09T09:16:56
#
#-------------------------------------------------

QT       += core gui xml opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qt c++11

TARGET = ecoulement
TEMPLATE = app

LIBS += -lQGLViewer


SOURCES += main.cpp\
        mainwindow.cpp \
        dtm.cpp \
        dtm.inl \
        gldisplay.cpp \
        tests.cpp \
        drawable.cpp \
        flowpath.cpp \
    gldisplay.inl \
    debugstream.cpp \
    flowpathview.cpp \
    logwidget.cpp


HEADERS  += mainwindow.h \
            openf.h \
            dtm.h \
            gldisplay.h \
            tests.h \
            flowpath.h \
            drawable.h \
            draw.h \
    debugstream.h \
    flowpathview.h \
    logwidget.h


FORMS    += mainwindow.ui \
    flowpathview.ui
