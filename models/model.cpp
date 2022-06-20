#include "model.h"


Model::ApiVer Model::ApiVer::JsonParse(const QJsonObject& j)
{
    Model::ApiVer m;
    m.major = j.value("major").toInt(0);
    m.minor = j.value("minor").toInt(0);
    return m;
}

QString Model::ApiVer::toString() const
{
    return QString::number(major)+'.'+QString::number(minor);
}

Model::Media::Status Model::Media::ParseStatus(const QString &s)
{
    if(s=="playing") return Status::playing;
    if(s=="paused") return Status::paused;
    if(s=="stopped") return Status::stopped;
    return Status::unknown;
}

QString Model::Media::StatusToString(Model::Media::Status s)
{
    if(s==Status::playing) return "playing";
    if(s==Status::paused) return "paused";
    if(s==Status::stopped) return "stopped";
    return "unknown";
}

Model::Media Model::Media::JsonParse(const QJsonObject& j)
{
    Model::Media m;
    m.album = j.value("album").toString();
    m.artist = j.value("artist").toString();
    QString statustxt = j.value("status").toString().toLower();
    m.status = ParseStatus(statustxt);
    m.title = j.value("title").toString();
    return m;
}

QString Model::Media::toString(){
    QString msg = Media::StatusToString(status)+": " + artist+": "+title;
    if(!album.isEmpty()) msg+= +" ("+album+")";
    return msg;
}

bool Model::Media::operator==(Media m)
{
    if(this->album!=m.album) return false;
    if(this->artist!=m.artist) return false;
    if(this->status!=m.status) return false;
    if(this->title!=m.title) return false;
    return true;
}

Model::Device Model::Device::JsonParse(const QJsonObject& j)
{
    Model::Device d;
    d.address = j.value("address").toString();
    d.connected = j.value("connected").toBool();
    d.name = j.value("name").toString();
    d.signal_strength = j.value("signal_strength").toInt(-1);
    return d;
}

QString Model::Device::toString() const
{
    return name+" "+address+" conn:"+(connected?"yes":"no");
}

bool Model::Device::operator==(Device m)
{
    if(this->connected!=m.connected) return false;
    if(this->address!=m.address) return false;
    return true;
}

Model::Features Model::Features::JsonParse(const QJsonObject& j)
{
    Model::Features m;
    m.audio = j.value("audio").toBool(0);
    m.call_support = j.value("call-support").toBool();

    m.server = j.value("server").toString();
    m.version = j.value("version").toString();
    return m;
}

QString Model::Features::toServerString() const
{
    QString msg = server+' '+version;
    return msg;
}

QString Model::Features::toFeatureString() const
{
    QString msg;
    if(audio) msg+="audio";
    if(call_support){
        if(!msg.isEmpty()) msg+=' ';
        msg+="callsupport";
    }
    return msg;
}

QString Model::Features::toString() const
{
//    QString msg = server+' '+version;
//    if(audio) msg+=" audio";
//    if(call_support) msg+=" callsupport";
    QString msg = this->toServerString()+' '+this->toFeatureString();
    return msg;
}

Model::Calls Model::Calls::JsonParse(const QJsonObject &j)
{
    Model::Calls m;
    return m;
}

QString Model::Calls::toString()
{
    return this->msg;
}
