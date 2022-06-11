#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imainview.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public IMainView
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    MainViewModel::DoWorkModel get_DoWorkModel();
    void set_PhoneView(const MainViewModel::PhoneViewModelR& m);
    void set_ConnectionView(const MainViewModel::ConnectionViewModelR& m);
    void set_ApiverView(const MainViewModel::ApiverViewModelR& m);
    void set_FeatureRequestView(const MainViewModel::FeatureRequestViewModelR& m);

signals:
    void PushButtonActionTriggered(IMainView *sender);
    void GetConnectionActionTriggered(IMainView *sender);
    void GetApiverActionTriggered(IMainView *sender);
    void GetFeatureRequestActionTriggered(IMainView *sender);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_connection_clicked();
    void on_pushButton_getApiver_clicked();
    void on_pushButton_FeatureRequest_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
