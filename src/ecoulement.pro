#-------------------------------------------------
#
# Project created by QtCreator 2015-10-09T09:16:56
#
#-------------------------------------------------

QT       += core gui xml opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ecoulement
TEMPLATE = app

LIBS += -lQGLViewer


SOURCES += main.cpp\
        mainwindow.cpp \
        dtm.cpp \
        gldisplay.cpp \
        tests.cpp


HEADERS  += mainwindow.h \
            openf.h \
            dtm.h \
            gldisplay.h \
            tests.h \
    draw.h

FORMS    += mainwindow.ui
