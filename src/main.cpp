
#include <QtCore/QCoreApplication>
#include<QtGui>
#include<QTimer>
#include "mainwindow.h"
#include "dtm.h"
#include "tests.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef TESTS
    return !runTests();
#else

    MainWindow w;
    w.show();
    QTimer *timer = new QTimer();
    timer->connect(timer, SIGNAL(timeout()),& a, SLOT(draw()));
    timer->start(50);
#if FALSE
    w.editingPath();
#endif

    return a.exec();
#endif


}
