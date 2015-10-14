#include <QFileDialog>
#include <QString>

#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tests.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_glDisplay(new glDisplay(this)),
    m_vector()
{
    //load interface .ui created  with QT Designer
    ui->setupUi(this);

    //create a connexion on the menu File-> Open DTM File via openDialog slot
    connect(ui->actionOpen_DTM_file, SIGNAL(triggered()), this, SLOT(openDialog()));
}

MainWindow::~MainWindow()
{
    delete m_glDisplay;
    delete ui;
}

void MainWindow::show()
{
    QMainWindow::show();
    m_glDisplay->show();
}

void MainWindow::openDialog() // Open a dialog to choose a file
{
    QString file = QFileDialog::getOpenFileName(this,"Open a file",QString(),"*.xyz");

    // Read the file with the coordinates
    readDTM(file,m_vector);

    testOnreadDTM(m_vector);

}

