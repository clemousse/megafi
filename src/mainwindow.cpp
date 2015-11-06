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
    m_dtm(NULL),
    m_flows(),
    m_glDisplay(new glDisplay(*this, &m_dtm, m_flows))
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
    if(m_dtm) delete m_dtm;
    delete ui;
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
    QString file;

    // Actions related to the dialog window
    {
        QFileDialog fDlg;

        // Don't accept empty file or directory; only one file
        fDlg.setFileMode(QFileDialog::ExistingFile);
        fDlg.setWindowTitle("Choose the DTM file");

        if(fDlg.exec())
        {
            QStringList selectedFiles;
            // Get the list of selected files
            selectedFiles = fDlg.selectedFiles();
            if(selectedFiles.length() == 1)
                file = selectedFiles.first();
        }
    }

    if(!file.isEmpty())
    {
        if(m_dtm)
        {
            delete m_dtm;
            m_dtm = NULL;
        }
        try
        {
            m_dtm = new DTM(file);
        }
        catch(const std::bad_alloc&)
        {
            m_dtm = NULL;
        }
    }
}

