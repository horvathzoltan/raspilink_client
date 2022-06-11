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

    QObject::connect(view_obj, SIGNAL(GetConnectionActionTriggered(IMainView *)),
                     this, SLOT(processGetConnectionAction(IMainView *)));

    QObject::connect(view_obj, SIGNAL(GetApiverActionTriggered(IMainView *)),
                     this, SLOT(processGetApiverAction(IMainView *)));

    QObject::connect(view_obj, SIGNAL(GetFeatureRequestActionTriggered(IMainView *)),
                     this, SLOT(processGetFeatureRequestAction(IMainView *)));
    //refreshView(w);
}


auto MainPresenter::init(DoWork::Params params) -> bool
{
    _isInited = false;
    _dowork.init(params);
    connect(&_dowork,SIGNAL(ResponseConnectionAction(CheckinResponseModel)),
            this,SLOT(onResponseConnectionAction(CheckinResponseModel)));
    connect(&_dowork,SIGNAL(ResponseGetApiverAction(QString)),
            this,SLOT(onResponseGetApiverAction(QString)));
    connect(&_dowork,SIGNAL(ResponseGetFeatureRequestAction(QString)),
            this,SLOT(onResponseGetFeatureRequestAction(QString)));

    //_result = { Result::State::NotCalculated, -1};
    _isInited = true;
    return true;
}

void MainPresenter::initView(IMainView *w) const {
   // MainViewModel::DoWorkRModel rm{"started",{}};
    //w->set_DoWorkRModel(rm);
};

void MainPresenter::processPushButtonAction(IMainView *sender){
    MainViewModel::DoWorkModel m = sender->get_DoWorkModel();

    auto rm = _dowork.Work1(m);

    sender->set_PhoneView({rm.txt});
}
/*GetConnection*/
void MainPresenter::processGetConnectionAction(IMainView *sender){
    auto guid = _dowork.GetCheckin();
    _senders.insert(guid,sender);
}

void MainPresenter::onResponseConnectionAction(CheckinResponseModel m)
{
    MainViewModel::ConnectionViewModelR rm = {m.msg};
    if(_senders.contains(m.guid)){
        _senders[m.guid]->set_ConnectionView(rm);
        _senders.remove(m.guid);
    }
}
/*GetApiver*/

void MainPresenter::processGetApiverAction(IMainView *sender){
    //_apiverActionSender = sender;
    auto guid = dowork.GetApiver();
    _senders.insert(guid,sender);
}

void MainPresenter::onResponseGetApiverAction(QString msg)
{
    MainViewModel::ApiverViewModelR rm = {msg};
    if(_senders.contains(m.guid)){
        _senders[m.guid]->set_ApiverView(rm);
        _senders.remove(m.guid);
    }

}

void MainPresenter::processGetFeatureRequestAction(IMainView *sender){
    _featureRequestActionSender = sender;
    _dowork.GetFeatureRequest();
}

void MainPresenter::onResponseGetFeatureRequestAction(QString msg)
{
    MainViewModel::FeatureRequestViewModelR rm = {msg};
    if(_featureRequestActionSender){
        _featureRequestActionSender->set_FeatureRequestView(rm);
    }

    _featureRequestActionSender = nullptr;
}
