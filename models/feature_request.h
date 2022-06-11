#ifndef FEATURE_REQUEST_H
#define FEATURE_REQUEST_H

#include <QJsonObject>
#include <QString>

struct FeatureRequest{
    bool audio = false;
    bool call_support = false;
    QString server="";
    QString version="";

    static FeatureRequest JsonParse(const QJsonObject& j)
    {
        FeatureRequest m;
        m.audio = j.value("audio").toBool(0);
        m.call_support = j.value("call-support").toBool();

        m.server = j.value("server").toString();
        m.version = j.value("version").toString();
        return m;
    }

    QString toString(){
        QString msg = server+' '+version;
        if(audio) msg+=" audio";
        if(call_support) msg+=" callsupport";
        return msg;
    }
};

#endif // FEATURE_REQUEST_H
