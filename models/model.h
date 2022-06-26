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
        int index=0;

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
        QMap<QString, Warning> warnings;
        QString map;
        int uvBlevel;
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

    struct WarningMeta{
        QString title;
        QString iconMask;
        QMap<int, QString> description;
    };

    enum WarningKeys {ts,rainstorm,wind,fzra,snowdrift,rain,snow,coldx,hotx,fog};



    ///images/warning_header/ts1.gif

//    QMap<WarningKeys, WarningMeta> warningMeta
//    {
//        {ts,{ .title="Zivatar",.iconMask="ts%1.gif",.description= {
//                    {1,"Figyelem! Zivatar alakulhat ki. Elsődleges veszélyforrást a villámlás jelent, emellett esetenként szélerősödés, jégeső előfordulhat!"},
//                    {2,"Veszély! Hevesebb zivatarok kialakulására lehet számítani. A villámlások mellett kockázatot jelent a zivatarokat kísérő szél, jégeső is!"},
//                    {3,"Fokozott veszély! Heves zivatarok várhatók! A zivatarokat kísérő szél, jégeső is jelentős kockázatot jelent!"}}}},
//        {rainstorm, { .title="Felhőszakadás",.iconMask="ts%1.gif",.description= {
//                    {1,"Intenzív záporból, zivatarból rövid idő alatt 25-30 mm-t meghaladó csapadék hullhat."},
//                    {2,"Intenzív záporból, zivatarból rövid idő alatt 50 mm-t meghaladó csapadék hullhat."}}}},
//        {wind, { .title="Széllökés",.iconMask="wind%1.gif",.description={
//                    {1,"A várt legerősebb széllökések meghaladhatják a 70 km/h-t."},
//                    {2,"A várt legerősebb széllökések meghaladhatják a 90 km/h-t."},
//                    {3,"A várt legerősebb széllökések meghaladhatják a 110 km/h-t."}}}},
//        {fzra, { .title="Ónos eső",.iconMask="fzra%1.gif",.description= {
//                    {1,"Gyenge ónos eső. A várt csapadékmennyiség általában néhány tized (> 0,1) mm."},
//                    {2,"Tartós (több órás) ónos eső. A várt csapadékmennyiség meghaladhatja az 1 mm-t."},
//                    {3,"Tartós (több órás) ónos eső. A várt csapadékmennyiség meghaladhatja az 5 mm-t."}}}},
//        {snowdrift, { .title="Hófúvás",.iconMask="snowdrift%1.gif",.description={
//                    {1,"Gyenge hófúvás. A friss hóval fedett területeken a szél alacsony hótorlaszokat emelhet."},
//                    {2,"Hófúvás. A friss hóval fedett területeken a viharos szél magas hótorlaszokat emelhet."},
//                    {3,"Erős hófúvás. A friss hóval fedett területeken a viharos szél több helyen jelentős hóakadályokat emel."}}}},
//        {rain, { .title="Eső",.iconMask="rain%1.gif",.description= {
//                    {1,"24 óra alatt több mint 20 mm csapadék hullhat."},
//                    {2,"24 óra alatt több mint 30 mm csapadék hullhat."},
//                    {3,"24 óra alatt több mint 50 mm csapadék hullhat."}}}},
//        {snow, { .title="Havazás",.iconMask="snow%1.gif",.description={
//                    {1,"12 óra alatt 5 cm-t meghaladó friss hó hullhat."},
//                    {2,"24 óra alatt 20 cm-t meghaladó friss hó hullhat."},
//                    {3,"24 óra alatt 30 cm-t meghaladó friss hó hullhat."}}}},
//        {coldx, { .title="Extrém hideg",.iconMask="coldx%1.gif",.description={
//                    {1,"A hőmérséklet - 15 °C alá csökkenhet."},
//                    {2,"A hőmérséklet - 20 °C alá csökkenhet."},
//                    {3,"A hőmérséklet - 25 °C alá csökkenhet."}}}},
//        {hotx, { .title="Hőség",.iconMask="hotx%1.gif",.description={
//                    {1,"A napi középhőmérséklet várhatóan eléri vagy meghaladja a 25 °C-ot."},
//                    {2,"A napi középhőmérséklet várhatóan eléri vagy meghaladja a 27 °C-ot."},
//                    {3,"A napi középhőmérséklet 29 °C felett alakulhat."}}}},
//        {fog, { .title="Tartós, sűrű köd",.iconMask="fog%1.gif",.description={
//                    {1,"Tartós (> 6 óra) sűrű köd (látástávolság pár száz méter) várható."}}}}
//    };
};

#endif // MODELS_H
