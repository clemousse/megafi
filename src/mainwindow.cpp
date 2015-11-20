#include "mainwindow.h"
#include "logwidget.h"

#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <iostream>
#include <QKeySequence>
#include <QDebug>
#include <QTextBlock>
#include <QTextCursor>
#include <limits>
#include <QTextStream>

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

    //load interface .ui created  with QT Designer
    ui->setupUi(this);

    //create a connexion on the menu File-> Open DTM File via openDialog slot
    connect(ui->actionOpen_DTM_file, SIGNAL(triggered()),this, SLOT(openDialog()));
    //create a connexion on the menu View-> New DTM Path via show slot
    connect(ui->actionNew_DTM_Window, SIGNAL(triggered()), m_glDisplay, SLOT(show()));
    //create a connexion ont the menu View-> Customize paths
    connect(ui->actionCustomize_paths, SIGNAL(triggered()), this, SLOT(changeFlowPathProperties()));
    //create close connexions
    connect(ui->actionQuit, SIGNAL(triggered()),this, SLOT(close()));
    //create a connexion on the menu View-> History via showHis slot
    connect(ui->actionView_history, SIGNAL(triggered()),ui->dockWidget_His, SLOT(show()));
    //create a connexion on the push button "selectionModeBtn" to activate the mode selection
    connect(ui->selectionModeBtn, SIGNAL(toggled(bool)),m_glDisplay, SLOT(rbClick(bool)));
    //create a connection between double-click on history and the change properties function
    connect(ui->pathList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(changeFlowPathProperties(QListWidgetItem*)));
    //create a connexion on the push button "btnComputation" to calculate the flow path in mainwindow
    connect(ui->btnComputation, SIGNAL(clicked()), this, SLOT(startComputation()));
    //create a connexion on the push button "pushButton_CL" to clear the log
    connect(ui->clearLogButton, SIGNAL(clicked()), ui->log, SLOT(clear()));
    //create a connexion on the menu File-> Export Paths
    connect(ui->actionExport_paths, SIGNAL(triggered()),this, SLOT(exportFlowPaths()));

    // Initialize progress bar
    m_progressBar->setWindowModality(Qt::NonModal);
    m_progressBar->setWindowTitle("Be patient please, file is being read!");
    m_progressBar->setFormat("Be patient please, file is being read!");
    m_progressBar->setTextVisible(true);
    m_progressBar->setGeometry(0,0,500,25);


    // Application signals
    connect(this, SIGNAL(DTMHasChanged()), m_glDisplay, SLOT(reinit()));
    connect(this, SIGNAL(flowsHaveChanged()), m_glDisplay, SLOT(updateGL()));
    connect(m_glDisplay, SIGNAL(clicked(qglviewer::Vec)), this, SLOT(setClickedCoordinates(qglviewer::Vec)));

    updateDTMWidgets();
    connect(this, SIGNAL(DTMHasChanged()), this, SLOT(updateDTMWidgets()));
    updateFlowWidgets();
    connect(this, SIGNAL(flowsHaveChanged()), this, SLOT(updateFlowWidgets()));
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


//close with cross button
void MainWindow::closeEvent(QCloseEvent* event)
{
       if(MainWindow::closeQuestion())
       {
           event->accept();
           m_glDisplay->close();
           MainWindow::close();
       }
       else
       {
           event->ignore();
       }
}

//close question called in slot close() already implemented
bool MainWindow::closeQuestion()
{
    int rep = QMessageBox::question(this,"Quit ?","Do you really want to quit ?",QMessageBox::Yes | QMessageBox::No);
    if (rep == QMessageBox::Yes)
    {
        return true;
    }
    else
    {
        return false;
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

void MainWindow::updateDTMWidgets()
{
    if(m_dtm)
        enableDTMWidgets();
    else
        disableDTMWidgets();
}

void MainWindow::enableDTMWidgets()
{
    ui->infosLbl          ->setEnabled(true);
    ui->selectionModeBtn  ->setEnabled(true);
    ui->coordinatesLbl    ->setEnabled(true);
    ui->coordinatesLblX   ->setEnabled(true);
    ui->coordinatesLblY   ->setEnabled(true);
    ui->coordinatesLblZ   ->setEnabled(true);
    ui->bxXcoord          ->setEnabled(true);
    ui->bxYcoord          ->setEnabled(true);
    ui->bxZcoord          ->setEnabled(true);
    ui->btnComputation    ->setEnabled(true);
    ui->clearLogButton    ->setEnabled(true);
    ui->dockWidget_His    ->setEnabled(true);
}

void MainWindow::disableDTMWidgets()
{
    ui->infosLbl          ->setEnabled(false);
    ui->selectionModeBtn  ->setEnabled(false);
    ui->coordinatesLbl    ->setEnabled(false);
    ui->coordinatesLblX   ->setEnabled(false);
    ui->coordinatesLblY   ->setEnabled(false);
    ui->coordinatesLblZ   ->setEnabled(false);
    ui->bxXcoord          ->setEnabled(false);
    ui->bxYcoord          ->setEnabled(false);
    ui->bxZcoord          ->setEnabled(false);
    ui->btnComputation    ->setEnabled(false);
    ui->clearLogButton    ->setEnabled(false);
    ui->dockWidget_His    ->setEnabled(false);

}

void MainWindow::updateFlowWidgets()
{
    if(m_flows.empty())
        disableFlowWidgets();
    else
        enableFlowWidgets();
}

void MainWindow::enableFlowWidgets()
{
}

void MainWindow::disableFlowWidgets()
{
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

    qWarning()<<"Congrats, you selected a point! You can now run the computation.\n";
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
        if(startIndex != std::numeric_limits<unsigned long>::max())

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

        else

        {
            qWarning()<< "Sorry, calcul isn't possible : there's no point belonging to the DTM with theses coordinates.\n";
        }
    }
}

void MainWindow::changeFlowPathProperties(QListWidgetItem* item)
{
    FlowPath* fpItem = static_cast<FlowPath*>(item);

    FlowPathProps* newProps = new FlowPathProps(m_flowPathDefaults);
    m_flowPathViewDefaultWindow->changeProps(*newProps);

    fpItem->setProperties(newProps);
    fpItem->buildArrays();
    fpItem->buildIcon();
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
                (*flow)->buildIcon();
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

void MainWindow::exportFlowPaths()
{
    //QString filter;
    QString flows = QFileDialog::getSaveFileName(this, "Save the flow paths", QString());
    /*if(filter == "Text files (*.txt)")
    {
         flows += ".txt";
    }

    else
    {
         flows += ".xml";
    }*/

         //creating of a file to export path in the release of the poject
            QFile file(flows);

            //openning the file in "read only" and checking the good opening
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

            //creating a stream "flux" to write in the file
            QTextStream stream(&file);

            //choosing the UTF-8 codec
            stream.setCodec("UTF-8");



            for(QList<megafi::FlowPath*>::iterator flow = m_flows.begin() ; flow != m_flows.end() ; ++flow)
            {
                stream <<(*flow)->QListWidgetItem::text();
                stream <<"\n";

                 for (int i=0 ; i <(*flow)->getNbVertices() ; i++)
                 {
                    const Point* const vertices =(*flow)->getVertices();
                    stream << vertices[i].x;
                    stream << " ";
                    stream << vertices[i].y;
                    stream << " ";
                    stream << vertices[i].z;
                    stream << "\n";
                 }
            }

}
