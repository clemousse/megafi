#include "mainwindow.h"

#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <iostream>
#include <QKeySequence>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_dtm(NULL),
    m_flows(),
    m_glDisplay(new glDisplay(*this, &m_dtm, reinterpret_cast< QList<const megafi::FlowPath*>& >(m_flows)))
{
    connect(m_glDisplay, SIGNAL(needsRebuild()), this, SLOT(rebuildArrays()));

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
    deleteFlows();
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
            // Delete DTM
            delete m_dtm;
            m_dtm = NULL;
            deleteFlows();
            m_flows.clear();
        }
        try
        {
            m_dtm = new megafi::DTM(file);
            rebuildArrays();
        }
        catch(const std::bad_alloc&)
        {
            m_dtm = NULL;
        }
    }
}

void MainWindow::rebuildArrays()
{
    // Rebuild DTM
    if(m_dtm)
    {
        switch(m_dtm->getMode())
        {
        case megafi::MODE_LEGACY        :                       break;
        case megafi::MODE_VERTEX_ARRAY  : m_dtm->buildArrays(); break;
        case megafi::MODE_VERTEX_INDICES: m_dtm->buildArrays(); break;
        }

        // Rebuild flows
        for(QList<megafi::FlowPath*>::iterator flow = m_flows.begin() ;
            flow != m_flows.end() ;
            ++flow)
        {
            const megafi::Mode mode = (*flow)->getMode();
            switch(mode)
            {
            case megafi::MODE_LEGACY        :                         break;
            case megafi::MODE_VERTEX_ARRAY  : (*flow)->buildArrays(); break;
            case megafi::MODE_VERTEX_INDICES: (*flow)->buildArrays(); break;
            }
        }
    }
}

void MainWindow::addFlow(unsigned long startIndex)
{
    if(m_dtm)
    {
        megafi::FlowPath* const newFP =
                new megafi::FlowPath(*m_dtm, startIndex, m_dtm->getMode());
        m_flows.push_back(newFP);
    }
}

void MainWindow::deleteFlows()
{
    for(QList<megafi::FlowPath*>::iterator flow = m_flows.begin() ;
        flow != m_flows.end() ;
        ++flow)
    {
        delete *flow;
        *flow = NULL;
    }
}
