#include "dowork.h"
#include "logger.h"
#include <QCommandLineParser>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

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

    //com::helper::CommandLineParserHelper::addOption(&parser, OPTION_IN, QStringLiteral("geometry file as input"));
    //com::helper::CommandLineParserHelper::addOption(&parser, OPTION_OUT, QStringLiteral("g-code file as output"));
    //com::helper::CommandLineParserHelper::addOptionBool(&parser, OPTION_BACKUP, QStringLiteral("set if backup is needed"));
    //com::helper::CommandLineParserHelper::addOptionBool(&parser, OPTION_TEST, QStringLiteral("set to activate test mode"));

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
    QObject::connect(&_h, SIGNAL(ResponseOk(QByteArray)),
                     this, SLOT(ResponseOkAction(QByteArray)));

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


QString DoWork::GetCheckin()
{
    _h.SendGet("checkin");
    zInfo("request sent");
    return QString("request sent");
}

void DoWork::ResponseOkAction(QByteArray s){
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(s, &errorPtr);
    QJsonObject rootobj = doc.object();
    auto device = rootobj.value("device").toObject();
    auto address = device.value("address");
    QString translation;
    /*foreach(const QJsonValue & val, tr){
        translation=val.toObject().value("text").toString();
    }*/

    translation = "address: "+address.toString();
    zInfo("ResponseOkAction:" + translation);

    emit ResponseOkAction2(translation);
}
