#include "mainwindow.h"
#include "dtm.h"
#include <QApplication>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    QVector<Point> result;
    readDTM("/home/gtsi/ecoulement/data/Ecrins2.xyz", result);

    return a.exec();
}
