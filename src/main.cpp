#include "q_debugstream.h"
#include "mainwindow.h"
#include "tests.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
#if QT_VERSION >= 0x050000
    qInstallMessageHandler(megafi::DebugStream::handle);
#else
    qInstallMsgHandler(megafi::DebugStream::handle);
#endif
    QApplication a(argc, argv);

#ifdef TESTS
    return !runTests();
#else

    megafi::MainWindow w;
    w.setWindowIcon(QIcon(QCoreApplication::applicationDirPath()+"/goutte.jpg"));
    w.setWindowTitle("MEGAFI : Modèle d'Ecoulement d'une Goutte dans les Alpes Franco-Italiennes");
    w.show();

    return a.exec();
#endif


}
