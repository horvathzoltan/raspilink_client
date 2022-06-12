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

/*UserActions*/
//void MainWindow::on_pushButton_clicked()
//{
//    emit PushButtonActionTriggered(this);
//}

void MainWindow::on_pushButton_connection_clicked()
{
    emit GetConnectionActionTriggered(this);
}

void MainWindow::on_pushButton_getApiver_clicked()
{
    emit GetApiverActionTriggered(this);
}

void MainWindow::on_pushButton_FeatureRequest_clicked()
{
    emit GetFeatureRequestActionTriggered(this);
}

/*GetViewModel*/
//auto MainWindow::get_DoWorkModel() -> ViewModel::DoWorkModel
//{
//    auto t = ui->label->text();
//    bool isok;
//    int i = t.toInt(&isok, 10);
//    if(!isok) return {-1};
//    return {i};
//};


/*SetView*/
//void MainWindow::set_PhoneView(const MainViewModel::PhoneViewModelR &m)
//{
//    ui->label->setText(m.msg);
//}

void MainWindow::set_ConnectionView(const ViewModel::ConnectionR &m)
{
    ui->label->setText(m.msg);
}

void MainWindow::set_ApiverView(const ViewModel::ApiverViewR &m)
{
    ui->label->setText(m.msg);
}

void MainWindow::set_FeatureRequestView(const ViewModel::FeatureRequestR &m)
{
     ui->label->setText(m.msg);
}






