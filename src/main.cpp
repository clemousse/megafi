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
#if FALSE
    w.editingPath();
#endif

    return a.exec();
#endif
}
