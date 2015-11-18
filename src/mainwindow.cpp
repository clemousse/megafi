#include "mainwindow.h"

#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <iostream>
#include <QKeySequence>
#include <QDebug>
#include <QTextBlock>
#include <QTextCursor>

using namespace megafi;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_dtm(NULL),
    m_flows(),
    m_dtmThread(),
    m_flowPathViewDefaultWindow(new FlowPathView(this)),
    m_glDisplay(new glDisplay(&m_dtm, reinterpret_cast< QList<const megafi::FlowPath*>* >(&m_flows))),
    m_progressBar(new QProgressBar())
{
    qRegisterMetaType<megafi::Point>("megafi::Point");
    qRegisterMetaType<QTextBlock>("QTextBlock");
    qRegisterMetaType<QTextCursor>("QTextCursor");

    m_flowPathDefaults.lineWidth = 5;
    m_flowPathDefaults.color.r   = 0;
    m_flowPathDefaults.color.g   = 0;
    m_flowPathDefaults.color.b   = 255;

    connect(this, SIGNAL(DTMHasChanged()), m_glDisplay, SLOT(reinit()));
    connect(this, SIGNAL(flowsHaveChanged()), m_glDisplay, SLOT(updateGL()));
    connect(m_glDisplay, SIGNAL(clicked(qglviewer::Vec)), this, SLOT(setClickedCoordinates(qglviewer::Vec)));

    //load interface .ui created  with QT Designer
    ui->setupUi(this);

    //create a connexion on the menu File-> Open DTM File via openDialog slot
    connect(ui->actionOpen_DTM_file, SIGNAL(triggered()),this, SLOT(openDialog()));
    //create a connexion on the menu View-> New DTM Path via show slot
    connect(ui->actionNew_DTM_Window, SIGNAL(triggered()), m_glDisplay, SLOT(show()));
    //create a connexion ont the menu View -> Customize paths
    connect(ui->actionCustomize_paths, SIGNAL(triggered()), this, SLOT(changeFlowPathProperties()));
    //create a connexion on the cross of the m_gl_display window to close it
    connect(ui->actionQuit, SIGNAL(triggered()),m_glDisplay, SLOT(close()));
    //create a connexion on the menu View-> Historic via showHis slot
    connect(ui->actionView_history, SIGNAL(triggered()),ui->dockWidget_His, SLOT(show()));
    //create a connexion on the menu File-> Quit via close slot (or cross)
    connect(ui->actionQuit, SIGNAL(triggered()),this, SIGNAL(closeAll()));
    connect(this, SIGNAL(closeAll()), this, SLOT(close()));
    connect(this, SIGNAL(closeAll()), m_glDisplay, SLOT(close()));
    //create a connexion on the radio button in calculating the flow path in mainwindow
    connect(ui->pushButton_Mouse, SIGNAL(toggled(bool)),m_glDisplay, SLOT(rbClick(bool)));

    //create a connexion on the radio button "btnQDebug" to redirect QDebug in QTextEdit

    connect(ui->btnComputation, SIGNAL(clicked()), this, SLOT(startComputation()));

    // Initialize progress bar
    m_progressBar->setWindowModality(Qt::NonModal);
    m_progressBar->setWindowTitle("Be patient please, file is being read!");
    m_progressBar->setFormat("Be patient please, file is being read!");
    m_progressBar->setTextVisible(true);
    m_progressBar->setGeometry(0,0,500,20);
}



MainWindow::~MainWindow()
{
    delete m_progressBar;
    delete m_glDisplay;
    delete m_flowPathViewDefaultWindow;
    deleteFlows();
    deleteDTM();
    delete ui;
}



void MainWindow::closeEvent(QCloseEvent* event)

{
    event->accept();
    emit closeAll();
}

void MainWindow::close()
{
    int rep = QMessageBox::question(this,"Quit ?","Do you really want to quit ?",QMessageBox::Yes | QMessageBox::No);
    if (rep == QMessageBox::Yes)
    {
        QMainWindow::close();
    }
}


void MainWindow::lockInterface()
{
    ui->actionOpen_DTM_file ->setEnabled(false);
    ui->actionExport_picture->setEnabled(false);
    ui->centralWidget       ->setEnabled(false);
}

void MainWindow::unlockInterface()
{
    ui->actionOpen_DTM_file ->setEnabled(true);
    ui->actionExport_picture->setEnabled(true);
    ui->centralWidget       ->setEnabled(true);
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

        // Actions related to dtm
        if(!file.isEmpty())
        {
            if(m_dtm)
            {
                deleteDTM();
                deleteFlows();
            }
            try
            {
                m_dtm = new megafi::DTM();

                // Initialize progress bar
                // All at 0 for the "infinite" aspect
                m_progressBar->setMaximum(0);
                m_progressBar->setMinimum(0);
                m_progressBar->setValue(0);
                m_progressBar->show();
                lockInterface();

                m_dtm->moveToThread(&m_dtmThread);
                // What happen at beginning of thread
                connect(this, SIGNAL(buildDTM(QString)), m_dtm, SLOT(buildDTM(QString)));
                // What happen at end of thread
                connect(m_dtm, SIGNAL(arrayRebuilt()), this, SLOT(unlockInterface()));
                connect(m_dtm, SIGNAL(arrayRebuilt()), this, SIGNAL(DTMHasChanged()));
                connect(m_dtm, SIGNAL(arrayRebuilt()), m_progressBar, SLOT(close()));
                // What happen afterwards
                connect(this, SIGNAL(computeIndex(megafi::Point)), m_dtm, SLOT(computeIndex(megafi::Point)));
                connect(m_dtm, SIGNAL(indexComputed(unsigned long)), this, SLOT(addFlow(unsigned long)));

                // Start thread
                m_dtmThread.start();
                emit buildDTM(file);
            }
            catch(const std::bad_alloc&)
            {
                m_dtm = NULL;
            }
        }
    }
}


void MainWindow::setClickedCoordinates(qglviewer::Vec mouse_world)
{
    ui->bxXcoord->setValue(mouse_world.x);
    ui->bxYcoord->setValue(mouse_world.y);
    ui->bxZcoord->setValue(mouse_world.z);
}

void MainWindow::startComputation()
{
    megafi::Point coords;
    coords.x = ui->bxXcoord->value();
    coords.y = ui->bxYcoord->value();
    coords.z = ui->bxZcoord->value();
    emit computeIndex(coords);
}

void MainWindow::addFlow(unsigned long startIndex)
{
   if(m_dtm)
    {
        megafi::FlowPath* const newFP =
                new megafi::FlowPath(&m_flowPathDefaults, ui->pathList, m_dtm->getMode());
        newFP->computePath(m_dtm, startIndex);
        m_flows.push_back(newFP);
        ui->pathList->addItem(newFP);
        connect(this, SIGNAL(buildFlow(const megafi::DTM*, unsigned long)), newFP, SLOT(buildArrays()));
        connect(newFP, SIGNAL(arrayRebuilt()), this, SIGNAL(flowsHaveChanged()));
        emit buildFlow(m_dtm, startIndex);
    }
}


void MainWindow::changeFlowPathProperties()
{
    m_flowPathViewDefaultWindow->changeProps(m_flowPathDefaults);
    if(m_flows.size())
    {
        if (  m_flows[0]->getMode() == megafi::MODE_VERTEX_ARRAY
           || m_flows[0]->getMode() == megafi::MODE_VERTEX_INDICES)
        {
            for(QList<megafi::FlowPath*>::iterator flow = m_flows.begin() ;
                flow != m_flows.end() ;
                ++flow)
            {
                (*flow)->buildArrays();
            }
        }
    }
}

void MainWindow::deleteDTM()
{
    if(m_dtm)
    {
        m_dtmThread.quit();
        m_dtmThread.wait();
        disconnect(m_dtm, SIGNAL(indexComputed(unsigned long)), this, SLOT(addFlow(unsigned long)));
        disconnect(this, SIGNAL(computeIndex(megafi::Point)), m_dtm, SLOT(computeIndex(megafi::Point)));
        disconnect(m_dtm, SIGNAL(arrayRebuilt()), this, SIGNAL(DTMHasChanged()));
        disconnect(m_dtm, SIGNAL(arrayRebuilt()), this, SLOT(unlockInterface()));
        disconnect(this, SIGNAL(buildDTM(QString)), m_dtm, SLOT(buildDTM(QString)));
        delete m_dtm;
        m_dtm = NULL;
    }
}

void MainWindow::deleteFlows()
{
    for(QList<megafi::FlowPath*>::iterator flow = m_flows.begin() ;
        flow != m_flows.end() ;
        ++flow)

    {
        disconnect(*flow, SIGNAL(arrayRebuilt()), this, SIGNAL(flowsHaveChanged()));
        disconnect(this, SIGNAL(buildFlow(const megafi::DTM*, unsigned long)), *flow, SLOT(computePath(const megafi::DTM*, unsigned long)));
        delete *flow;
        *flow = NULL;
    }
    ui->pathList->clear();
    m_flows.clear();
}
