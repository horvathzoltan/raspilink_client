
#include "dowork.h"
#include "logger.h"
#include "helpers/commandlineparserhelper.h"
#include "models/checkinresponse.h"
#include <QCommandLineParser>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <models/apiver.h>
#include <models/device.h>
#include <models/feature_request.h>
#include <models/media.h>

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

auto DoWork::Params::Parse(const QCoreApplication& app) -> DoWork::Params
{
    QCommandLineParser parser;

    parser.setApplicationDescription(QStringLiteral("command line test1 app."));
    parser.addHelpOption();
    parser.addVersionOption();

    const QString OPTION_IN = QStringLiteral("input");
    const QString OPTION_OUT = QStringLiteral("output");
    const QString OPTION_BACKUP = QStringLiteral("backup");
    const QString OPTION_TEST = QStringLiteral("test");

    CommandLineParserHelper::addOption(&parser, OPTION_IN, QStringLiteral("geometry file as input"));
    CommandLineParserHelper::addOption(&parser, OPTION_OUT, QStringLiteral("g-code file as output"));
    CommandLineParserHelper::addOptionBool(&parser, OPTION_BACKUP, QStringLiteral("set if backup is needed"));
    CommandLineParserHelper::addOptionBool(&parser, OPTION_TEST, QStringLiteral("set to activate test mode"));

    parser.process(app);

    return {
        parser.value(OPTION_IN),
        parser.value(OPTION_OUT),
        parser.isSet(OPTION_BACKUP),
        parser.isSet(OPTION_TEST)
    };
}

auto DoWork::Params::IsValid() -> bool
{
    QStringList err;
//    if(inFile.isEmpty())
//    {
//        err.append(QStringLiteral("inFile is empty"));
//    }
//    if(inFile.isEmpty())
//    {
//        err.append(QStringLiteral("outFile is empty"));
//    }
    if(!err.isEmpty()) qDebug()<<err;
            return err.isEmpty();
}


auto DoWork::init(DoWork::Params p) -> bool
{
    _isInited = false;
    if(!p.IsValid()) return false;

    params = p;

    //_result = { Result::State::NotCalculated, -1};
    QObject::connect(&_httpHelper, SIGNAL(ResponseOk(const QUuid&, const QString&, QByteArray)),
                     this, SLOT(ResponseOkAction(const QUuid&, const QString&, QByteArray)));

    _isInited = true;
    return true;
}

auto DoWork::Work1(MainViewModel::DoWorkModel m) -> MainViewModel::DoWorkRModel
{
    if(!_isInited) return {};

    MainViewModel::DoWorkRModel rm;
    rm.txt = QString::number(m.i+1);
    return rm;
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
    else if(action==APIVER) GetApiverResponse(s);
    else if(action==FEATURE_REQUEST) GetFeatureRequestResponse(s);
    else zInfo("unknown action: "+action);
}

void DoWork::GetCheckinResponse(const QUuid& guid, QByteArray s){
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(s, &errorPtr);
    QJsonObject rootobj = doc.object();
    CheckinResponseModel m(guid);

    if(rootobj.isEmpty()){
        m.msg = "no response";
    }else{
        auto deviceJ = rootobj.value("device").toObject();
        if(deviceJ.isEmpty()){
            m.msg = "no device";
        }else{
            Device device;
            if(!deviceJ.isEmpty()) device=Device::JsonParse(deviceJ);
            if(!m.msg.isEmpty()) m.msg+='\n';
            m.msg += "device: "+device.toString();
        }
        auto mediaJ = rootobj.value("media").toObject();
        if(mediaJ.isEmpty()){
            m.msg = "no media";
        }else{
            Media media;
            if(!mediaJ.isEmpty()) media=Media::JsonParse(mediaJ);
            if(!m.msg.isEmpty()) m.msg+='\n';
            m.msg += "media: "+media.toString();
        }
    }

    emit ResponseConnectionAction(m);
}

void DoWork::GetApiverResponse(QByteArray s){
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(s, &errorPtr);
    QJsonObject rootobj = doc.object();

    QString msg;
    Apiver m;
    if(rootobj.isEmpty()){
        msg = "no response";
    }else{
        m=Apiver::JsonParse(rootobj);
        msg = "apiver: "+m.toString();
    }

    emit ResponseGetApiverAction(msg);
}

void DoWork::GetFeatureRequestResponse(QByteArray s){
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(s, &errorPtr);
    QJsonObject rootobj = doc.object();

    QString msg;
    FeatureRequest m;
    if(rootobj.isEmpty()){
        msg = "no response";
    }else{
        m = FeatureRequest::JsonParse(rootobj);
        msg = "features: "+m.toString();
    }

    emit ResponseGetFeatureRequestAction(msg);
}
