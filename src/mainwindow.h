#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "flowpath.h"
#include "dtm.h"
#include "flowpathview.h"
#include "gldisplay.h"
#include "ui_mainwindow.h"

#include <QList>
#include <QMainWindow>
#include <QGraphicsSceneMouseEvent>
#include <QProgressBar>
#include <QThread>
#include <QMutex>

namespace megafi
{


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;

protected:
    // Data
    megafi::DTM* m_dtm;
    QList<megafi::FlowPath*> m_flows;

    // Threads
private:
    QThread m_dtmThread;

protected:
    // Windows
    FlowPathView*    const m_flowPathViewDefaultWindow;
    const glDisplay* const m_glDisplay;
    QProgressBar*    const m_progressBar;

    // Properties
    megafi::FlowPathProps m_flowPathDefaults;


public:
    explicit MainWindow(QWidget* parent = NULL);
    ~MainWindow();

    void closeEvent(QCloseEvent *);

public slots:
    // GUI slots
    void openDialog();
    void close();
    void changeFlowPathProperties();
    void startComputation();
    void lockInterface();
    void unlockInterface();
    void updateDTMWidgets();
    void enableDTMWidgets();
    void disableDTMWidgets();
    void updateFlowWidgets();
    void enableFlowWidgets();
    void disableFlowWidgets();

    // Application slots
    void setClickedCoordinates(qglviewer::Vec mouse_world);
    void addFlow(unsigned long startIndex);

signals:
    void buildDTM(QString) const;
    void buildFlow(const megafi::DTM*, unsigned long) const;
    void DTMHasChanged() const;
    void flowsHaveChanged() const;
    void closeAll() const;
    void computeIndex(megafi::Point) const;

private:
    void deleteDTM();
    void deleteFlows();
};

}

#endif // MAINWINDOW_H
