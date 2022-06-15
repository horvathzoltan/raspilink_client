#ifndef MODELS_H
#define MODELS_H

#include <QJsonObject>
#include <QString>

namespace Model
{    
    struct ApiVer{
        int major=0;
        int minor = 0;

        static Model::ApiVer JsonParse(const QJsonObject &j);
        QString toString();
    };

    struct Media{
        enum Status {unknown,playing,stopped,paused};
        static Status ParseStatus(const QString& s);
        static QString StatusToString(Status s);

        QString album;
        QString artist;
        Status status=Status::unknown;
        QString title;

        static Model::Media JsonParse(const QJsonObject &j);
        QString toString();
    };

    struct Device{
        QString address="";
        bool connected = false;
        QString name="";
        int signal_strength=-1;

        static Model::Device JsonParse(const QJsonObject &j);
        QString toString();
    };

    struct Features{
        bool audio = false;
        bool call_support = false;
        QString server="";
        QString version="";

        static Model::Features JsonParse(const QJsonObject &j);
        QString toString();
    };    

    struct Calls{
        QString msg;

        static Model::Calls JsonParse(const QJsonObject &j);
        QString toString();
    };

    struct Data{
        ApiVer apiVer;
        Features features;
        Media media;
        Device device;
        Calls calls;
    };
};

#endif // MODELS_H
