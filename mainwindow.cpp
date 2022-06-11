#include "mainwindow.h"
#include "logger.h"
#include "ui_mainwindow.h"
#include <QDebug>

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


void MainWindow::on_pushButton_clicked()
{
    zTrace();
    emit PushButtonActionTriggered(this);
}

auto MainWindow::get_DoWorkModel() -> MainViewModel::DoWorkModel
{
    auto t = ui->label->text();
    bool isok;
    int i = t.toInt(&isok, 10);
    if(!isok) return {-1};
    return {i};
};


void MainWindow::on_pushButton_connection_clicked()
{
    zTrace();
    emit ConnectionActionTriggered(this);
}

/**/

void MainWindow::set_PhoneView(const MainViewModel::PhoneViewModelR &m)
{
    ui->label->setText(m.msg);
}

void MainWindow::set_ConnectionView(const MainViewModel::ConnectionViewModelR &m)
{
    ui->label->setText(m.msg);
}
