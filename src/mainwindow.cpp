#include "mainwindow.h"
#include "logwidget.h"

#include <limits>

#include <QFileDialog>
#include <QString>
#include <QMessageBox>
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
    m_progressBar(),
    m_contextMenu()
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

    // glDisplay signals
    //create a connexion on the menu View-> New DTM Path via show slot
    connect(ui->actionNew_DTM_Window, SIGNAL(triggered()), m_glDisplay, SLOT(show()));
    //create a connexion on the push button "selectionModeBtn" to activate the mode selection
    connect(ui->selectionModeBtn, SIGNAL(toggled(bool)),m_glDisplay, SLOT(rbClick(bool)));
    // refresh draw
    connect(this, SIGNAL(DTMHasChanged()), m_glDisplay, SLOT(reinit()));
    connect(this, SIGNAL(flowsHaveChanged()), m_glDisplay, SLOT(updateGL()));
    // point clicked
    connect(m_glDisplay, SIGNAL(clicked(qglviewer::Vec)), this, SLOT(setClickedCoordinates(qglviewer::Vec)));

    // Initialize progress bar
    m_progressBar.setWindowModality(Qt::NonModal);
    m_progressBar.setWindowTitle("Be patient please, file is being read!");
    m_progressBar.setFormat("Be patient please, file is being read!");
    m_progressBar.setTextVisible(true);
    m_progressBar.setGeometry(0,0,500,25);

    // Initialize context menu
    {
        QList<QAction*> actions = { ui->actionCMDelete, ui->actionCMRename, ui->actionCMCustomize };
        m_contextMenu.addActions(actions);
    }
}



MainWindow::~MainWindow()
{
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
                m_progressBar.setMaximum(0);
                m_progressBar.setMinimum(0);
                m_progressBar.setValue(0);
                m_progressBar.show();

                m_dtm->moveToThread(&m_dtmThread);
                // What happen at beginning of thread
                connect(this, SIGNAL(buildDTM(QString)), m_dtm, SLOT(buildDTM(QString)));
                // What happen at end of thread
                connect(m_dtm, SIGNAL(arrayRebuilt()), this, SLOT(unlockDTMWidgets()));
                connect(m_dtm, SIGNAL(arrayRebuilt()), this, SIGNAL(DTMHasChanged()));
                connect(m_dtm, SIGNAL(arrayRebuilt()), &m_progressBar, SLOT(close()));
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

void MainWindow::unlockDTMWidgets()
{
    emit lockDTMWidgets(false);
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
            connect(this, SIGNAL(buildFlow(const megafi::DTM*, unsigned long)), newFP, SLOT(buildArrays()));
            connect(newFP, SIGNAL(arrayRebuilt()), this, SIGNAL(flowsHaveChanged()));
            emit buildFlow(m_dtm, startIndex);

            ui->pathList->addItem(newFP);
            emit beFlows(true);
        }

        else
        {
            qWarning()<< "Sorry, calcul isn't possible : there's no point belonging to the DTM with theses coordinates.\n";
        }
    }
}


void MainWindow::deleteFlowPath(FlowPath *flow)
{
    disconnect(flow, SIGNAL(arrayRebuilt()), this, SIGNAL(flowsHaveChanged()));
    disconnect(this, SIGNAL(buildFlow(const megafi::DTM*, unsigned long)), flow, SLOT(computePath(const megafi::DTM*, unsigned long)));
    delete flow;

    ui->pathList->removeItemWidget(flow);
    m_flows.removeOne(flow);
    emit flowsHaveChanged();
    emit beFlows(!m_flows.isEmpty());
}

void MainWindow::openContextMenu(QPoint p)
{
    QListWidgetItem* selectedItem = ui->pathList->itemAt(p);
    QAction* const res = m_contextMenu.exec(mapToGlobal(p));

    if(res == ui->actionCMRename)
        ui->pathList->editItem(selectedItem);
    else if(res == ui->actionCMCustomize)
        changeFlowPathProperties(selectedItem);
    else if(res == ui->actionCMDelete)
        deleteFlowPath(static_cast<FlowPath*>(selectedItem));
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
        disconnect(this, SIGNAL(buildDTM(QString)), m_dtm, SLOT(buildDTM(QString)));
        disconnect(m_dtm, SIGNAL(arrayRebuilt()), this, SLOT(unlockDTMWidgets()));
        disconnect(m_dtm, SIGNAL(arrayRebuilt()), this, SIGNAL(DTMHasChanged()));
        disconnect(m_dtm, SIGNAL(arrayRebuilt()), &m_progressBar, SLOT(close()));
        disconnect(this, SIGNAL(computeIndex(megafi::Point)), m_dtm, SLOT(computeIndex(megafi::Point)));
        disconnect(m_dtm, SIGNAL(indexComputed(unsigned long)), this, SLOT(addFlow(unsigned long)));
        delete m_dtm;
        m_dtm = NULL;
    }

    emit lockDTMWidgets(true);
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
    emit beFlows(false);
}
