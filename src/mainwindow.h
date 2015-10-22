#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dtm.h"
#include "gldisplay.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QGraphicsSceneMouseEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;

protected:
    QVector<Point> m_vector;
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
