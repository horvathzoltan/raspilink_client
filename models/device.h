#ifndef DEVICE_H
#define DEVICE_H

#include <QJsonObject>
#include <QString>

struct Device{
    QString address="";
    bool connected = false;
    QString name="";
    int signal_strength=-1;

    static Device JsonParse(const QJsonObject& j)
    {
        Device d;
        d.address = j.value("address").toString();
        d.connected = j.value("connected").toBool();
        d.name = j.value("name").toString();
        d.signal_strength = j.value("signal_strength").toInt(-1);
        return d;
    }

    QString toString(){
        return name+" "+address+" conn:"+(connected?"yes":"no");
    }
};

#endif // DEVICE_H
