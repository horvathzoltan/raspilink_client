#ifndef MODELS_H
#define MODELS_H

#include <QJsonObject>
#include <QString>

namespace Model
{
//public:

    struct Apiver{
        int major=0;
        int minor = 0;

        static Model::Apiver JsonParse(const QJsonObject &j);
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

    struct FeatureRequest{
        bool audio = false;
        bool call_support = false;
        QString server="";
        QString version="";

        static Model::FeatureRequest JsonParse(const QJsonObject &j);
        QString toString();
    };

};

#endif // MODELS_H
