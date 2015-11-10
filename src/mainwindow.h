#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "flowpath.h"
#include "dtm.h"
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
    megafi::DTM* m_dtm;
    QList<megafi::FlowPath*> m_flows;
    glDisplay* const m_glDisplay;
    bool m_qdbg;


public:
     explicit MainWindow(QWidget* parent = NULL);
    ~MainWindow();
#if FALSE
     void editingPath();
#endif
    void activeQDebug();

public slots:
    void openDialog();
    void rebuildArrays();
    void setClickedCoordinates(const qglviewer::Vec& mouse_world);
    void addFlow(unsigned long startIndex);
    void qdClick(bool qdbg);

protected:
    void closeEvent(QCloseEvent* event);

private:
    void deleteFlows();
};


#endif // MAINWINDOW_H
