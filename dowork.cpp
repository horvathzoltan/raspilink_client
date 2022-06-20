
#include "dowork.h"
#include "logger.h"
#include "helpers/commandlineparserhelper.h"
#include "models/responsemodel.h"
#include <QCommandLineParser>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <models/model.h>
#include <models/featurerequest.h>

const QString DoWork::CHECKIN = QStringLiteral("checkin");
const QString DoWork::APIVER = QStringLiteral("apiver");
const QString DoWork::FEATURE_REQUEST = QStringLiteral("feature_request");

const QString DoWork::CALLS_ANSWER = QStringLiteral("/calls/answer");
const QString DoWork::CALLS_HANGUP = QStringLiteral("/calls/hangup");
const QString DoWork::CALLS_HANGUPALL = QStringLiteral("/calls/hangupall");
const QString DoWork::CALLS_DIAL = QStringLiteral("/calls/dial");

const QString DoWork::MEDIA_PLAY = QStringLiteral("/media/play");
const QString DoWork::MEDIA_PAUSE = QStringLiteral("/media/pause");
const QString DoWork::MEDIA_SKIP = QStringLiteral("/media/skip");
const QString DoWork::MEDIA_BACK = QStringLiteral("/media/back");

const QString DoWork::CURRENTWEATHER = QStringLiteral("apiver/k");


DoWork::DoWork(QObject *parent) :QObject(parent)
{

}

auto DoWork::init(const QString& host, int port) -> bool
{
    _isInited = false;

    if(!_httpHelper.init(host, port)) return _isInited;
    //params = p;

    //_result = { Result::State::NotCalculated, -1};
    QObject::connect(&_httpHelper, SIGNAL(ResponseOk(const QUuid&, const QString&, QByteArray)),
                     this, SLOT(ResponseOkAction(const QUuid&, const QString&, QByteArray)));

    _isInited = true;
    return true;
}

QUuid DoWork::GetCheckin()
{
    return _httpHelper.SendGet(CHECKIN);
}

QUuid DoWork::GetApiver()
{
    return _httpHelper.SendGet(APIVER);
}

QUuid DoWork::GetFeatureRequest()
{
    return _httpHelper.SendGet(FEATURE_REQUEST);
}

QUuid DoWork::GetCallsAnswer()
{
    return _httpHelper.SendGet(CALLS_ANSWER);
}

QUuid DoWork::GetCallsHangup()
{
    return _httpHelper.SendGet(CALLS_HANGUP);
}

QUuid DoWork::GetCallsHangupAll()
{
    return _httpHelper.SendGet(CALLS_HANGUPALL);
}

QUuid DoWork::GetCallsDial()
{
    return _httpHelper.SendGet(CALLS_DIAL);
}

QUuid DoWork::GetMediaPlay()
{
    return _httpHelper.SendGet(MEDIA_PLAY);
}

QUuid DoWork::GetMediaPause()
{
    return _httpHelper.SendGet(MEDIA_PAUSE);
}

QUuid DoWork::GetMediaSkip()
{
    return _httpHelper.SendGet(MEDIA_SKIP);
}

QUuid DoWork::GetMediaBack()
{
    return _httpHelper.SendGet(MEDIA_BACK);
}

ViewModel::State DoWork::GetState(const Model::Device &device, const Model::Media &media, const Model::Calls &calls)
{
    ViewModel::State rm;
//    if(_data.calls.connected){
//        if(device.connected){
//            rm.deviceMsg="változott?";
//        } else{
//            rm.deviceMsg="lecsatlakozott";
//        }
//    }else{
//        if(device.connected){
//            rm.deviceMsg="felcsatlakozott";
//        }
//    }
    rm.callsMsg="";rm.callsState=ViewModel::State::ConnectionState::unknown;
       if(_data.device.connected){
           if(device.connected){
               if(_data.device==device){
                   rm.deviceMsg="változatlan";
                   rm.deviceState = ViewModel::State::ConnectionState::unchanged;
               } else {
                   rm.deviceMsg="változott";
                   rm.deviceState = ViewModel::State::ConnectionState::changed;
               }
           } else{
               rm.deviceMsg="lecsatlakozott";
               rm.deviceState = ViewModel::State::ConnectionState::deleted;
           }
       }else{
           if(device.connected){
               rm.deviceMsg="felcsatlakozott";
               rm.deviceState = ViewModel::State::ConnectionState::created;
           } else{
               rm.deviceMsg="nincs device";
               rm.deviceState = ViewModel::State::ConnectionState::unknown;
           }
       }
       if(_data.media.status!=Model::Media::Status::unknown){
           if(media.status!=Model::Media::Status::unknown){
               if(_data.media==media){
                   rm.mediaMsg="változatlan";
                   rm.mediaState = ViewModel::State::ConnectionState::unchanged;
               } else{
                   rm.mediaMsg="változott";
                   rm.mediaState = ViewModel::State::ConnectionState::changed;
               }
           } else{
               rm.mediaMsg="nincs média";
               rm.mediaState = ViewModel::State::ConnectionState::deleted;
           }
       } else{
           if(media.status!=Model::Media::Status::unknown &&
                   (media.title.toLower()!="not provided")){
               rm.mediaMsg="új média";
               rm.mediaState = ViewModel::State::ConnectionState::created;
           } else {
               rm.mediaMsg="nincs média";
               rm.mediaState = ViewModel::State::ConnectionState::unknown;
           }
       }
       return rm;
}

void DoWork::ResponseOkAction(const QUuid& guid, const QString& action,  QByteArray s){
    if(action==CHECKIN) GetCheckinResponse(guid,s);
    else if(action==APIVER) GetApiverResponse(guid,s);
    else if(action==FEATURE_REQUEST) GetFeatureRequestResponse(guid,s);
    else if(action==CURRENTWEATHER)
        GetCurrentWeatherResponse(guid,s);
    else zInfo("unknown action: "+action);
}

void DoWork::GetCheckinResponse(const QUuid& guid, QByteArray s){
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(s, &errorPtr);
    QJsonObject rootobj = doc.object();
    ResponseModel::Checkin r(guid);

    if(rootobj.isEmpty()){
        r.msg = "no response";
    }else{
        auto deviceJ = rootobj.value("device").toObject();
        if(deviceJ.isEmpty()){
            r.msg = "no device";
        }else{
            if(!deviceJ.isEmpty()) r.device = Model::Device::JsonParse(deviceJ);
            if(!r.msg.isEmpty()) r.msg+='\n';
            r.msg += "device: "+r.device.toString();
        }
        auto mediaJ = rootobj.value("media").toObject();
        if(mediaJ.isEmpty()){
            r.msg = "no media";
        }else{
            if(!mediaJ.isEmpty()) r.media=Model::Media::JsonParse(mediaJ);
            if(!r.msg.isEmpty()) r.msg+='\n';
            r.msg += "media: "+r.media.toString();
        }

        auto callsJ = rootobj.value("calls").toObject();
        if(callsJ.isEmpty()){
            r.msg = "no calls";
        }else{

            if(!callsJ.isEmpty()) r.calls=Model::Calls::JsonParse(callsJ);
            if(!r.msg.isEmpty()) r.msg+='\n';
            r.msg += "calls: "+r.media.toString();
        }
    }

    emit ResponseConnectionAction(r);
}

void DoWork::GetApiverResponse(const QUuid& guid, QByteArray s){
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(s, &errorPtr);
    QJsonObject rootobj = doc.object();
    ResponseModel::GetApiVer r(guid);

    //Model::ApiVer m;
    if(rootobj.isEmpty()){
        r.msg = "no response";
    }else{
        r.apiVer = Model::ApiVer::JsonParse(rootobj);
        r.msg = "apiver: "+r.apiVer.toString();
    }

    emit ResponseGetApiverAction(r);
}

void DoWork::GetFeatureRequestResponse(const QUuid& guid, QByteArray s){
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(s, &errorPtr);
    QJsonObject rootobj = doc.object();
    ResponseModel::GetFeature r(guid);

    //Model::Features m;
    if(rootobj.isEmpty()){
        r.msg = "no response";
    }else{
        r.features = Model::Features::JsonParse(rootobj);
        r.msg = "features: "+r.features.toString();
    }

    emit ResponseGetFeatureRequestAction(r);
}

/*weather*/

QUuid DoWork::GetCurrentWeather()
{
    return _httpHelper.SendGet(CURRENTWEATHER);
}

void DoWork::GetCurrentWeatherResponse(const QUuid& guid, QByteArray s){
    //QJsonParseError errorPtr;
    //QJsonDocument doc = QJsonDocument::fromJson(s, &errorPtr);
    //QJsonObject rootobj = doc.object();
    ResponseModel::GetCurrentWeather r(guid);

    //Model::ApiVer m;
    //if(rootobj.isEmpty()){
    //    r.msg = "no response";
    //}else{
        //r.currentWeather = Model::CurrentWeather::JsonParse(rootobj);
        //r.msg = "currentWeather: "+r.currentWeather.toString();
        r.currentWeather = {"hutty"};
        r.msg = "hutty2";
    //}

    emit ResponseGetCurrentWeatherRequestAction(r);
}
