#include "dowork.h"
#include "logger.h"
#include "mainpresenter.h"
#include <QDebug>
#include <models/viewmodel.h>
#include <models/responsemodel.h>
#include <models/model.h>

MainPresenter::MainPresenter(QObject *parent) :QObject(parent)
{

}

void MainPresenter::appendView(IMainView *w)
{
    if(_views.contains(w)) return;
    _views.append(w);

    auto *view_obj = dynamic_cast<QObject*>(w);


    QObject::connect(view_obj, SIGNAL(GetConnectionActionTriggered(IMainView*)),
                     this, SLOT(processGetConnectionAction(IMainView*)));

    QObject::connect(view_obj, SIGNAL(GetApiverActionTriggered(IMainView*)),
                     this, SLOT(processGetApiverAction(IMainView*)));

    QObject::connect(view_obj, SIGNAL(GetFeatureRequestActionTriggered(IMainView*)),
                     this, SLOT(processGetFeatureRequestAction(IMainView*)));

    QObject::connect(view_obj, SIGNAL(MediaRefreshActionTriggered(IMainView*)),
                     this, SLOT(processMediaRefreshAction(IMainView*)));

    QObject::connect(view_obj, SIGNAL(DeviceRefreshActionTriggered(IMainView*)),
                     this, SLOT(processDeviceRefreshAction(IMainView*)));

    QObject::connect(view_obj, SIGNAL(CallsRefreshActionTriggered(IMainView*)),
                     this, SLOT(processCallsRefreshAction(IMainView*)));

    QObject::connect(view_obj, SIGNAL(GetCurrentWeatherActionTriggered(IMainView*)),
                     this, SLOT(processGetCurrentWeatherAction(IMainView*)));
    //refreshView(w);    
}


auto MainPresenter::init(const MainPresenterInit& m) -> bool
{
    _isInited = false;
    _dowork.init({m.settings});
    connect(&_dowork,SIGNAL(ResponseConnectionAction(ResponseModel::Checkin)),
            this,SLOT(onResponseConnectionAction(ResponseModel::Checkin)));
    connect(&_dowork,SIGNAL(ResponseGetApiverAction(ResponseModel::GetApiVer)),
            this,SLOT(onResponseGetApiverAction(ResponseModel::GetApiVer)));
    connect(&_dowork,SIGNAL(ResponseGetFeatureRequestAction(ResponseModel::GetFeature)),
            this,SLOT(onResponseGetFeatureRequestAction(ResponseModel::GetFeature)));
    connect(&_dowork,SIGNAL(ResponseGetCurrentWeatherRequestAction(ResponseModel::GetCurrentWeather)),
            this,SLOT(onResponseGetCurrentWeatherRequestAction(ResponseModel::GetCurrentWeather)));
    connect(&_dowork,SIGNAL(ResponseGetCurrentWeatherIconRequestAction(ResponseModel::GetCurrentWeatherIcon)),
            this,SLOT(onResponseGetCurrentWeatherIconRequestAction(ResponseModel::GetCurrentWeatherIcon)));
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


ViewModel::Page MainPresenter::GetActivePage(ViewModel::State state)
{
    if(state.callsState==ViewModel::State::ConnectionState::created)
        return ViewModel::Page::calls;
    //ha amúgy van és a változott az nem megszűnés
    if(state.callsState==ViewModel::State::ConnectionState::changed)
        return ViewModel::Page::calls;
//    if(state.callsState==ViewModel::State::ConnectionState::deleted)
//        return ViewModel::Page::noChange;
    //ha bejön vagy van egy hívás de elnavigálok - és nincs változás, ne menjen vissza, de ha valami változik, igen
    if(state.callsState==ViewModel::State::ConnectionState::unchanged)
        return ViewModel::Page::noChange;
    if(state.mediaState==ViewModel::State::ConnectionState::created)
        return ViewModel::Page::media;
    //ha amúgy van és a változott az nem megszűnés
    // és nem paused, akkor
    if(state.mediaState==ViewModel::State::ConnectionState::changed){
        //    return ViewModel::Page::media;
        auto s = _dowork.media().status;
        if(s==Model::Media::Status::playing) return ViewModel::Page::media;
        return ViewModel::Page::noChange;
    }


    if(state.mediaState==ViewModel::State::ConnectionState::unchanged)
        return ViewModel::Page::noChange;
//ha sokáig áll, vagy stopped, vagy deleted, akkor conn
//    if(state.mediaState==DoWork::State::ConnectionState::deleted)
//        return ViewModel::Page::connection;
    return ViewModel::Page::noChange;
}

void MainPresenter::onResponseConnectionAction(ResponseModel::Checkin m)
{

    if(_senders.contains(m.guid)){
        auto state = _dowork.GetState(m.device, m.media, m.calls);

        _dowork.setData(m.device);
        _dowork.setData(m.media);

        auto page = GetActivePage(state);

        ViewModel::ConnectionR rm {.page = page, .state = state};
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
    if(_senders.contains(m.guid)){
        //_data.apiVer = m.apiVer;
        _dowork.setData(m.apiVer);
        ViewModel::Apiver rm = {m.apiVer};
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
    if(_senders.contains(m.guid)){
        //_data.features = m.features;
         _dowork.setData(m.features);

        ViewModel::Features rm = {m.features};
        _senders[m.guid]->set_FeatureRequestView(rm);
        _senders.remove(m.guid);
    }
}

void MainPresenter::processMediaRefreshAction(IMainView *sender)
{
    Model::Media m = _dowork.media();
    ViewModel::Media rm{
        .media = m
    };
    sender->set_MediaView(rm);
}

void MainPresenter::processDeviceRefreshAction(IMainView *sender)
{
    Model::Device m = _dowork.device();
    ViewModel::Device rm{
        .device = m
    };
    sender->set_DeviceView(rm);
}

void MainPresenter::processCallsRefreshAction(IMainView *sender)
{
    Model::Calls m = _dowork.calls();
    ViewModel::Calls rm{
        .calls = m
    };
    sender->set_CallsView(rm);
}

/*weather*/

void MainPresenter::processGetCurrentWeatherAction(IMainView *sender){
    auto guid = _dowork.GetCurrentWeather();
    _senders.insert(guid,sender);
}

void MainPresenter::onResponseGetCurrentWeatherRequestAction(ResponseModel::GetCurrentWeather m)
{
    if(_senders.contains(m.guid)){
        auto sender = _senders[m.guid];

        //_data.apiVer = m.apiVer;
        _dowork.setData(m.currentWeather);
        ViewModel::CurrentWeather rm = {m.currentWeather};

        sender->set_CurrentWeatherView(rm);
        _senders.remove(m.guid);

        auto guid_icon = _dowork.GetCurrentWeatherIcon();
        _senders.insert(guid_icon,sender);
    }
}

void MainPresenter::onResponseGetCurrentWeatherIconRequestAction(ResponseModel::GetCurrentWeatherIcon m)
{
    if(_senders.contains(m.guid)){
        //_data.apiVer = m.apiVer;
        //_dowork.setData(m.currentWeather);
        ViewModel::CurrentWeatherIcon rm = {m.pixmap};
        _senders[m.guid]->set_CurrentWeatherIconView(rm);
        _senders.remove(m.guid);
    }
}
