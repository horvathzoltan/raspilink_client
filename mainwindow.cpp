#include "mainwindow.h"
#include "logger.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_status_2->setText("started");
    setPage(ViewModel::Page::main);
    timer.setParent(this);
    QObject::connect(&timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
    timer.start(1000);
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
    setPage(m.page);

    ui->label_media->setText(m.state.mediaMsg);
    ui->label_device->setText(m.state.deviceMsg);
    ui->label_calls->setText(m.state.callsMsg);

    if(IsRefresh(m.state.mediaState)){
        emit MediaRefreshActionTriggered(this);
    }
    if(IsRefresh(m.state.deviceState)){
        emit DeviceRefreshActionTriggered(this);
    }
    if(IsRefresh(m.state.callsState)){
        emit CallsRefreshActionTriggered(this);
    }
}

bool MainWindow::IsRefresh(ViewModel::State::ConnectionState s){
    if(s==ViewModel::State::ConnectionState::changed) return true;
    if(s==ViewModel::State::ConnectionState::created) return true;
    if(s==ViewModel::State::ConnectionState::deleted) return true;
    return false;
}


void MainWindow::setPage(ViewModel::Page page){
    if(page==ViewModel::Page::noChange) return;
    QString pageName = ViewModel::PageToPageName(page);
    auto p = ui->tabWidget->findChild<QWidget *>(pageName);
    if(p==nullptr) return;
    if(ui->tabWidget->currentWidget()==p) return;
    int page_ix = ui->tabWidget->indexOf(p);
    if(page_ix==-1) return;
    ui->tabWidget->setCurrentIndex(page_ix);
}

void MainWindow::set_ApiverView(const ViewModel::ApiverViewR &m)
{
    ui->label_api->setText(m.msg);
}

void MainWindow::set_FeatureRequestView(const ViewModel::FeatureRequestR &m)
{
    ui->label_features->setText(m.msg);
}


void MainWindow::onTimerTimeout()
{
    emit GetConnectionActionTriggered(this);
    ui->label_status_2->setText("tick: "+QString::number(tick));
    tick++;
}

/*set media*/
void MainWindow::set_MediaView(const ViewModel::Media &m)
{
    ui->label_album->setText(m.media.album);
    ui->label_artist->setText(m.media.artist);
    ui->label_title->setText(m.media.title);
    ui->label_status->setText(Model::Media::StatusToString(m.media.status));
}

