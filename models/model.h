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

        QString toString() const {
            QString msg = title+": "+temperature+", "+value+ " - "+ shortDesc;
            return msg;
        };
    };

    //        Warning ts; //zivatar
    //        Warning wind;//széllökés
    //        Warning rainstorm; //felhőszakadás
    //        Warning fzra;//ónos eső,
    //        Warning snowdrift;//hófúvás
    //Havazásra és nagy mennyiségű esőre
    //hőség, extrém hideg, továbbá a tartós sűrű köd

    struct Warning{
        QString title;
        QString icon;
        QString value_icon;
        int value=0;

        //src="/images/warningb/w0.gif"
        static int ParseValue(const QString& txt){
            static const QString rtxt = R"(\/[^\d]*(\d+)\.)";
            static const QRegularExpression rx(rtxt);

            auto m = rx.match(txt);
            if(!m.hasMatch()) return -1;
            bool ok;
            int i = m.captured(1).toInt(&ok);
            if(!ok) return -1;
            return i;
        }
    };

    struct CurrentWarning{
        QList<Warning> warnings;
        QString map;
    };

    struct Warnings{
        QList<Warning> warnings;
    };

    struct Data{
        ApiVer apiVer;
        Features features;
        Media media;
        Device device;
        Calls calls;
        CurrentWeather currentWeather;
        CurrentWarning currentWarning;
    };       
};

#endif // MODELS_H
