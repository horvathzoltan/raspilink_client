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

signals:
    void PushButtonActionTriggered(IMainView *sender);
    void ConnectionActionTriggered(IMainView *sender);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_connection_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
