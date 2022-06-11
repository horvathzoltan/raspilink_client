#ifndef APIVER_H
#define APIVER_H

#include <QJsonObject>
#include <QString>

struct Apiver{
    int major=0;
    int minor = 0;

    static Apiver JsonParse(const QJsonObject& j)
    {
        Apiver m;
        m.major = j.value("major").toInt(0);
        m.minor = j.value("minor").toInt(0);
        return m;
    }

    QString toString(){
        return QString::number(major)+'.'+QString::number(minor);
    }
};

#endif // APIVER_H
