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
        QString album;
        QString artist;
        QString status;
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

    struct Data{
        ApiVer apiVer;
        Features features;
        Media media;
        Device device;
    } data;

};

#endif // MODELS_H
