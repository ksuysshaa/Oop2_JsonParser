#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "facade.h"
#include "logic.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::string filePath = "";
    std::string textFile = "";

private slots:
    void on_btnOpenFile_clicked();
    void on_btnCheckFile_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
