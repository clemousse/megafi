#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QtWidgets>
#include <QGraphicsSceneMouseEvent>
#include <iostream>
#include <QKeySequence>

#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tests.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_vector(),
    m_glDisplay(new glDisplay(this, m_vector))
{
    //load interface .ui created  with QT Designer
    ui->setupUi(this);

    //create a connexion on the menu File-> Open DTM File via openDialog slot
    connect(ui->actionOpen_DTM_file, SIGNAL(triggered()), this, SLOT(openDialog()));
    //create a connexion on the menu View-> New DTM Path via show slot
    connect(ui->actionNew_DTM_Window, SIGNAL(triggered()), m_glDisplay, SLOT(show()));
    //create a connexion on the cross of the m_gl_display window to close it
    connect(ui->actionQuit, SIGNAL(triggered()),m_glDisplay, SLOT(close()));
    //create a connexion on the menu View-> Legend via showLeg slot
    connect(ui->actionView_legend, SIGNAL(triggered()),ui->dockWidget_Leg, SLOT(show()));
    //create a connexion on the menu View-> History via showHis slot
    connect(ui->actionView_history, SIGNAL(triggered()),ui->dockWidget_His, SLOT(show()));
    //create a connexion on the menu File-> Quit via close slot (or cross)
    connect(ui->actionQuit, SIGNAL(triggered()),this, SLOT(close()));

}

MainWindow::~MainWindow()
{

    delete m_glDisplay;
    delete ui;
}

void MainWindow::show()
{
    QMainWindow::show();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
       int rep = QMessageBox::question(this,"Quit ?","Do you really want to quit ?",QMessageBox::Yes | QMessageBox::No);
       if (rep == QMessageBox::Yes)
       {
               event->accept();
               m_glDisplay->close();
       }

       else
       {
               event->ignore();
       }
}

void MainWindow::openDialog() // Open a dialog to choose a file
{
    QString file = QFileDialog::getOpenFileName(this,"Open a file",QString(),"*.xyz");

    // Read the file with the coordinates
    readDTM(file,m_vector);
    m_glDisplay->computeDataSize();
}


