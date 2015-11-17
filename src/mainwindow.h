#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "flowpath.h"
#include "dtm.h"
#include "flowpathview.h"
#include "gldisplay.h"
#include "ui_mainwindow.h"
#include "q_debugstream.h"

#include <QList>
#include <QMainWindow>
#include <QGraphicsSceneMouseEvent>


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;

protected:
    // Data
    megafi::DTM* m_dtm;
    QList<megafi::FlowPath*> m_flows;


    // Windows
    FlowPathView* const m_flowPathViewDefaultWindow;
    glDisplay*    const m_glDisplay;

    // Properties
    megafi::FlowPathProps m_flowPathDefaults;


public:
    explicit MainWindow(QWidget* parent = NULL);
    ~MainWindow();
    void myMessageOutput();

public slots:
    void openDialog();
    void rebuildArrays();
    void setClickedCoordinates(const qglviewer::Vec& mouse_world);
    void addFlow(unsigned long startIndex);
    void changeFlowPathProperties();

signals:
    void dtmHasChanged(void) const;

protected:
    void closeEvent(QCloseEvent* event);

private:
    void deleteFlows();
};


#endif // MAINWINDOW_H
