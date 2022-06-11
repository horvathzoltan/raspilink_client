#include "dowork.h"
#include "logger.h"
#include "mainpresenter.h"
#include <QDebug>
#include <models/mainviewmodel.h>

MainPresenter::MainPresenter(QObject *parent) :QObject(parent)
{

}

void MainPresenter::appendView(IMainView *w)
{
    if(_views.contains(w)) return;
    _views.append(w);

    auto *view_obj = dynamic_cast<QObject*>(w);

    QObject::connect(view_obj, SIGNAL(PushButtonActionTriggered(IMainView *)),
                     this, SLOT(processPushButtonAction(IMainView *)));

    QObject::connect(view_obj, SIGNAL(ConnectionActionTriggered(IMainView *)),
                     this, SLOT(processConnectionAction(IMainView *)));

    //refreshView(w);
}


auto MainPresenter::init(DoWork::Params params) -> bool
{
    _isInited = false;
    _dowork.init(params);
    connect(&_dowork,SIGNAL(ResponseOkAction2(QString)),
            this,SLOT(onResponseOkAction2(QString)));

    //_result = { Result::State::NotCalculated, -1};
    _isInited = true;
    return true;
}

void MainPresenter::initView(IMainView *w) const {
   // MainViewModel::DoWorkRModel rm{"started",{}};
    //w->set_DoWorkRModel(rm);
};

void MainPresenter::processPushButtonAction(IMainView *sender){
    zTrace();
    MainViewModel::DoWorkModel m = sender->get_DoWorkModel();

    auto rm = _dowork.Work1(m);

    sender->set_PhoneView({rm.txt});
}

void MainPresenter::processConnectionAction(IMainView *sender){
    zTrace();

    _connectionActionSender = sender;
    QString msg = _dowork.GetCheckin();
}

void MainPresenter::onResponseOkAction2(QString msg)
{
    MainViewModel::ConnectionViewModelR rm = {msg};
    if(_connectionActionSender){
        _connectionActionSender->set_ConnectionView(rm);
    }

    _connectionActionSender = nullptr;
}
