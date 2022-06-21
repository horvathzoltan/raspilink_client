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
        QString toString() const;
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
        bool operator==(Media m);
    };

    struct Device{
        QString address="";
        bool connected = false;
        QString name="";
        int signal_strength=-1;

        static Model::Device JsonParse(const QJsonObject &j);
        QString toString() const;
        bool operator==(Device m);
    };

    struct Features{
        bool audio = false;
        bool call_support = false;
        QString server="";
        QString version="";

        static Model::Features JsonParse(const QJsonObject &j);
        QString toString() const;
        QString toServerString() const;
        QString toFeatureString() const;
    };    

    struct Calls{
        QString msg;

        static Model::Calls JsonParse(const QJsonObject &j);
        QString toString();
    };

    struct Sun{
        QString title;
        QString icon;
        QTime time;
    };

    struct CurrentWeather{
        QString city;
        QString shortDesc;
        QString title;
        QString value;
        QString icon;
        QString temperature;
        Sun sunrise;
        Sun sunset;
    };

    struct Warning{
        QString title;
        QString icon;
        int value;
    };

    struct CurrentWarning{
        Warning ts;
        Warning wind;
        Warning rainstorm;
        Warning fzra;
        Warning snowdrift;
        QString map;
    };

    struct Data{
        ApiVer apiVer;
        Features features;
        Media media;
        Device device;
        Calls calls;
        CurrentWeather currentWeather;
    };       
};

#endif // MODELS_H
