
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
    w.setWindowTitle("MEGAFI : Modèle d'Ecoulement d'une Goutte dans les Alpes Franco-Italiennes");
    w.show();

#if FALSE
    w.editingPath();
#endif

    return a.exec();
#endif


}
