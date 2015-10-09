#include <QFile>
#include <QString>
#include <vector>
using namespace std;

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //load interface .ui created  with QT Designer
    ui->setupUi(this);

    //create a connexion on the menu File-> Open DTM File via openDialog slot
    connect(ui->actionOpen_DTM_file, SIGNAL(triggered()), this, SLOT(openDialog()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openDialog() // Open a dialog to choose a file
{
        QString file = QFileDialog::getOpenFileName(this,"Open a file",QString(),"*.xyz");
        QMessageBox::information(this, "File","You selected : \n" + file);
}
