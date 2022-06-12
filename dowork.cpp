
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

void DoWork::ResponseOkAction(const QUuid& guid, const QString& action,  QByteArray s){
    if(action==CHECKIN) GetCheckinResponse(guid,s);
    else if(action==APIVER) GetApiverResponse(guid,s);
    else if(action==FEATURE_REQUEST) GetFeatureRequestResponse(guid,s);
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
            Model::Device device;
            if(!deviceJ.isEmpty()) device = Model::Device::JsonParse(deviceJ);
            if(!r.msg.isEmpty()) r.msg+='\n';
            r.msg += "device: "+device.toString();
        }
        auto mediaJ = rootobj.value("media").toObject();
        if(mediaJ.isEmpty()){
            r.msg = "no media";
        }else{
            Model::Media media;
            if(!mediaJ.isEmpty()) media=Model::Media::JsonParse(mediaJ);
            if(!r.msg.isEmpty()) r.msg+='\n';
            r.msg += "media: "+media.toString();
        }
    }

    emit ResponseConnectionAction(r);
}

void DoWork::GetApiverResponse(const QUuid& guid, QByteArray s){
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(s, &errorPtr);
    QJsonObject rootobj = doc.object();
    ResponseModel::GetApiVer r(guid);

    Model::Apiver m;
    if(rootobj.isEmpty()){
        r.msg = "no response";
    }else{
        m = Model::Apiver::JsonParse(rootobj);
        r.msg = "apiver: "+m.toString();
    }

    emit ResponseGetApiverAction(r);
}

void DoWork::GetFeatureRequestResponse(const QUuid& guid, QByteArray s){
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(s, &errorPtr);
    QJsonObject rootobj = doc.object();
    ResponseModel::GetFeature r(guid);

    Model::FeatureRequest m;
    if(rootobj.isEmpty()){
        r.msg = "no response";
    }else{
        m = Model::FeatureRequest::JsonParse(rootobj);
        r.msg = "features: "+m.toString();
    }

    emit ResponseGetFeatureRequestAction(r);
}
