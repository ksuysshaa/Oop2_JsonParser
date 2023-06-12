#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "iostream"
#include "fstream"
#include "logic.h"
#include "facade.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnOpenFile_clicked()
{
    filePath = QFileDialog::getOpenFileName(this, " ", "C:\\Users", "*.json").toStdString();
    if (filePath == "")
        ui->lblFilePath->setText("File was not opened");
    else {
        FileWorker file(filePath);
        Facade facade(&file, nullptr);
        textFile = facade.readOperation();
        ui->plainTextEdit->setPlainText(QString::fromStdString(textFile));
    }
}

void MainWindow::on_btnCheckFile_clicked()
{
    try {
        JsonChecker checker(textFile);
        Facade facade(nullptr ,&checker);
        facade.checkOperation();
        std::string message = "File is correct!";
        ui->lblAnswer->setText(QString::fromStdString(message));
    } catch (JsonError& error) {
        const char* jsonError = error.what();
        ui->lblAnswer->setText(jsonError);
    } catch (SystemError& error) {
        const char* systemError = error.what();
        ui->lblAnswer->setText(systemError);
    }
}

