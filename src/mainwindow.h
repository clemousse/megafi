#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "flowpath.h"
#include "dtm.h"
#include "gldisplay.h"
#include "flowpathview.h"
#include "ui_mainwindow.h"

#include <QList>
#include <QMainWindow>
#include <QGraphicsSceneMouseEvent>
#include <QThread>


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
    QThread m_flowsThread;
    QThread m_displayThread;

protected:
    // Windows
    FlowPathView* const m_flowPathViewDefaultWindow;
    const glDisplay* const m_glDisplay;

    // Properties
    megafi::FlowPathProps m_flowPathDefaults;

public:
    explicit MainWindow(QWidget* parent = NULL);
    ~MainWindow();
#if FALSE
    void editingPath();
#endif

    void addFlow(unsigned long startIndex);

public slots:
    // GUI slots
    void openDialog();
    void close();
    void changeFlowPathProperties();
    void lockInterface();
    void unlockInterface();

    // Application slots
    void setClickedCoordinates(const qglviewer::Vec& mouse_world);

signals:
    void buildDTM(QString) const;
    void buildFlow() const;
    void DTMHasChanged() const;
    void FlowsHaveChanged() const;

private:
    void deleteDTM();
    void deleteFlows();
};


#endif // MAINWINDOW_H
