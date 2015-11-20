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
#include <QPoint>
#include <QMenu>
#include <QListWidgetItem>

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
    FlowPathView* const m_flowPathViewDefaultWindow;
    glDisplay*    const m_glDisplay;
    QProgressBar        m_progressBar;
    QMenu               m_contextMenu;

    // Properties
    megafi::FlowPathProps m_flowPathDefaults;


public:
    explicit MainWindow(QWidget* parent = NULL);
    ~MainWindow();

    void closeEvent(QCloseEvent *);

signals:
    void lockDTMWidgets(bool) const;
    void buildDTM(QString) const;
    void buildFlow(const megafi::DTM*, unsigned long) const;
    void DTMHasChanged() const;
    void flowsHaveChanged() const;
    void beFlows(bool) const;
    void computeIndex(megafi::Point) const;

public slots:
    // GUI slots
    void openDialog();
    void startComputation();
    bool closeQuestion();
    void openContextMenu(QPoint p);
    void changeFlowPathProperties();
    void deleteFlowPath(FlowPath* flow);
    void changeFlowPathProperties(QListWidgetItem* item);
    void unlockDTMWidgets();

    // Application slots
    void setClickedCoordinates(qglviewer::Vec mouse_world);
    void addFlow(unsigned long startIndex);

private:
    void deleteDTM();
    void deleteFlows();
};

}

#endif // MAINWINDOW_H
