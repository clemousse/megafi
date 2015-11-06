#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "flowpath.h"
#include "dtm.h"
#include "gldisplay.h"
#include "ui_mainwindow.h"
#include <QList>
#include <QMainWindow>
#include <QGraphicsSceneMouseEvent>


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;

protected:
    const DTM* m_dtm;
    QList<FlowPath> m_flows;
    glDisplay* const m_glDisplay;

    void closeEvent(QCloseEvent* event);


public:
     explicit MainWindow(QWidget* parent = NULL);
    ~MainWindow();

public slots:
    void show();
    void openDialog();
};

#endif // MAINWINDOW_H
