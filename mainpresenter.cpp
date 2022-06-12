#include "dowork.h"
#include "logger.h"
#include "mainpresenter.h"
#include <QDebug>
#include <models/viewmodel.h>
#include <models/responsemodel.h>

MainPresenter::MainPresenter(QObject *parent) :QObject(parent)
{

}

void MainPresenter::appendView(IMainView *w)
{
    if(_views.contains(w)) return;
    _views.append(w);

    auto *view_obj = dynamic_cast<QObject*>(w);

    QObject::connect(view_obj, SIGNAL(PushButtonActionTriggered(IMainView*)),
                     this, SLOT(processPushButtonAction(IMainView*)));

    QObject::connect(view_obj, SIGNAL(GetConnectionActionTriggered(IMainView*)),
                     this, SLOT(processGetConnectionAction(IMainView*)));

    QObject::connect(view_obj, SIGNAL(GetApiverActionTriggered(IMainView*)),
                     this, SLOT(processGetApiverAction(IMainView*)));

    QObject::connect(view_obj, SIGNAL(GetFeatureRequestActionTriggered(IMainView*)),
                     this, SLOT(processGetFeatureRequestAction(IMainView*)));
    //refreshView(w);
}


auto MainPresenter::init(const QString& host, int port) -> bool
{
    _isInited = false;
    _dowork.init(host, port);
    connect(&_dowork,SIGNAL(ResponseConnectionAction(ResponseModel::Checkin)),
            this,SLOT(onResponseConnectionAction(ResponseModel::Checkin)));
    connect(&_dowork,SIGNAL(ResponseGetApiverAction(ResponseModel::GetApiVer)),
            this,SLOT(onResponseGetApiverAction(ResponseModel::GetApiVer)));
    connect(&_dowork,SIGNAL(ResponseGetFeatureRequestAction(ResponseModel::GetFeature)),
            this,SLOT(onResponseGetFeatureRequestAction(ResponseModel::GetFeature)));

    //_result = { Result::State::NotCalculated, -1};
    _isInited = true;
    return true;
}

void MainPresenter::initView(IMainView *w) const {
   // MainViewModel::DoWorkRModel rm{"started",{}};
    //w->set_DoWorkRModel(rm);
};

/*GetConnection*/
void MainPresenter::processGetConnectionAction(IMainView *sender){
    auto guid = _dowork.GetCheckin();
    _senders.insert(guid,sender);
}

void MainPresenter::onResponseConnectionAction(ResponseModel::Checkin m)
{
    ViewModel::ConnectionR rm = {m.msg};
    if(_senders.contains(m.guid)){
        _senders[m.guid]->set_ConnectionView(rm);
        _senders.remove(m.guid);
    }
}

/*GetApiver*/
void MainPresenter::processGetApiverAction(IMainView *sender){
    auto guid = _dowork.GetApiver();
    _senders.insert(guid,sender);
}

void MainPresenter::onResponseGetApiverAction(ResponseModel::GetApiVer m)
{
    ViewModel::ApiverViewR rm = {m.msg};
    if(_senders.contains(m.guid)){
        _senders[m.guid]->set_ApiverView(rm);
        _senders.remove(m.guid);
    }
}

/*GetFeature*/
void MainPresenter::processGetFeatureRequestAction(IMainView *sender){
    auto guid = _dowork.GetFeatureRequest();
    _senders.insert(guid,sender);
}

void MainPresenter::onResponseGetFeatureRequestAction(ResponseModel::GetFeature m)
{
    ViewModel::FeatureRequestR rm = {m.msg};
    if(_senders.contains(m.guid)){
        _senders[m.guid]->set_FeatureRequestView(rm);
        _senders.remove(m.guid);
    }
}
