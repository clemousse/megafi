#include "mainwindow.h"
#include "dtm.h"
#include <QApplication>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();


    //w.openDialog();

    readDTM("C:\\Qt\\Tools\\QtCreator\\bin\\Projet\\Ecrins2.xyz") ;
    return a.exec();
}
