#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QAction>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QObject>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openDialog();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
