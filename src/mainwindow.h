#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dtm.h"
#include "gldisplay.h"
#include "ui_mainwindow.h"
#include <QMainWindow>


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    glDisplay* const m_glDisplay;

protected:
    QVector<Point> m_vector;

public:
     explicit MainWindow(QWidget* parent = NULL);
    ~MainWindow();

public slots:
    void show();
    void openDialog();



};

#endif // MAINWINDOW_H
