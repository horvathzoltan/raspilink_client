#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imainview.h"

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public IMainView
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void set_ConnectionView(const ViewModel::ConnectionR& m);
    void set_ApiverView(const ViewModel::ApiverViewR& m);
    void set_FeatureRequestView(const ViewModel::FeatureRequestR& m);

    void set_MediaView(const ViewModel::Media& m);

signals:
    void GetConnectionActionTriggered(IMainView *sender);
    void GetApiverActionTriggered(IMainView *sender);
    void GetFeatureRequestActionTriggered(IMainView *sender);

    void MediaRefreshActionTriggered(IMainView *sender);
    void DeviceRefreshActionTriggered(IMainView *sender);
    void CallsRefreshActionTriggered(IMainView *sender);

private slots:
    void on_pushButton_connection_clicked();
    void on_pushButton_getApiver_clicked();
    void on_pushButton_FeatureRequest_clicked();
    void onTimerTimeout();

private:
    Ui::MainWindow *ui;
    void setPage(ViewModel::Page page);
    QTimer timer;
    int tick=0;
    bool IsRefresh(ViewModel::State::ConnectionState s);
};
#endif // MAINWINDOW_H
