#include <QFileDialog>
#include <QString>

#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_vector()
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


    readDTM(file,m_vector);
    qDebug() << "La troisième ligne du tableau contient X = " << m_vector[2].x << " Y = " << m_vector[2].y << " Z = " << m_vector[3].z << endl;
}
