#ifndef MEDIA_H
#define MEDIA_H

#include <QJsonObject>
#include <QString>

struct Media{
    QString album;
    QString artist;
    QString status;
    QString title;

    static Media JsonParse(const QJsonObject& j)
    {
        Media m;
        m.album = j.value("album").toString();
        m.artist = j.value("artist").toString();
        m.status = j.value("status").toString();
        m.title = j.value("title").toString();
        return m;
    }

    QString toString(){
        QString msg = status+": " + artist+": "+title;
        if(!album.isEmpty()) msg+= +" ("+album+")";
        return msg;
    }
};
#endif // MEDIA_H
