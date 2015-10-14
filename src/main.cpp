#include "mainwindow.h"
#include "dtm.h"
#include "tests.h"

#include <QApplication>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef TESTS
    return !runTests();
#else

    MainWindow w;
    w.show();

    return a.exec();
#endif
}
