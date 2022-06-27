
#include "dowork.h"
#include "logger.h"
#include "helpers/commandlineparserhelper.h"
#include "models/responsemodel.h"
#include <QCommandLineParser>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <models/model.h>
#include <models/featurerequest.h>
#include "helpers/htmlhelper.h"


const QString DoWork::CHECKIN = QStringLiteral("checkin");
const QString DoWork::APIVER = QStringLiteral("apiver");
const QString DoWork::FEATURE_REQUEST = QStringLiteral("feature_request");

const QString DoWork::CALLS_ANSWER = QStringLiteral("/calls/answer");
const QString DoWork::CALLS_HANGUP = QStringLiteral("/calls/hangup");
const QString DoWork::CALLS_HANGUPALL = QStringLiteral("/calls/hangupall");
const QString DoWork::CALLS_DIAL = QStringLiteral("/calls/dial");

const QString DoWork::MEDIA_PLAY = QStringLiteral("/media/play");
const QString DoWork::MEDIA_PAUSE = QStringLiteral("/media/pause");
const QString DoWork::MEDIA_SKIP = QStringLiteral("/media/skip");
const QString DoWork::MEDIA_BACK = QStringLiteral("/media/back");

const QString DoWork::CURRENTWEATHER = QStringLiteral("#CURRENTWEATHER");
const QString DoWork::CURRENTWEATHERICON = QStringLiteral("#CURRENTWEATHERICON");
const QString DoWork::CURRENTWARNING = QStringLiteral("#CURRENTWARNING");
const QString DoWork::CURRENTWARNINGMAP = QStringLiteral("#CURRENTWARNINGMAP");

const QMap<int, QString> DoWork::warningLevelDescriptions{
    {1,"Első szint (sárga), Az ebbe a kategóriába sorolt időjárási események nem szokatlanok, de potenciális veszélyt jelenthetnek, ezért tanácsos elővigyázatosnak, óvatosnak lenni, főként az időjárási hatásoknak jobban kitett tevékenységek során. Különösen a bizonytalanabb kimenetelű, gyorsan változó időjárási helyzetekben célszerű a szokásosnál gyakrabban és részletesebben tájékozódni a várható időjárás felől."},
    {2,"Második szint (narancs) Veszélyt hordozó időjárási jelenség, amely káreseményekhez vezethet, vagy akár személyi sérülést, balesetet is okozhat. Érvényben lévő veszélyjelzés esetén legyünk nagyon körültekintőek, vigyázzunk saját biztonságunkra és értékeinkre. Részletesen tájékozódjunk az időjárás alakulásáról. Kövessük a megbízható média által közvetített tanácsokat, illetve a hatóságok utasításait."},
    {3,"Harmadik szint (piros) Veszélyes, komoly károkat okozó, sok esetben emberi életet is fenyegető időjárási jelenségek, amelyek rendszerint kiterjedt területeket érintenek. Érvényben lévő veszélyjelzés esetén legyünk különös figyelemmel értékeinkre és saját biztonságunkra. Folyamatosan kísérjük figyelemmel a legfrissebb hivatalos meteorológiai információkat. Minden körülmények között kövessük a hatóságok utasításait. Tartózkodjunk biztonságos helyen. A veszélyjelzés e legmagasabb (piros) szintjére már csak a meglehetősen ritkán előforduló események kerülnek."}
};

QMap<Model::WarningKeys, Model::WarningMeta> warningMeta
{
    {Model::WarningKeys::ts,{ .title="Zivatar",.iconMask="ts%1.gif",.description= {
                {1,"Figyelem! Zivatar alakulhat ki. Elsődleges veszélyforrást a villámlás jelent, emellett esetenként szélerősödés, jégeső előfordulhat!"},
                {2,"Veszély! Hevesebb zivatarok kialakulására lehet számítani. A villámlások mellett kockázatot jelent a zivatarokat kísérő szél, jégeső is!"},
                {3,"Fokozott veszély! Heves zivatarok várhatók! A zivatarokat kísérő szél, jégeső is jelentős kockázatot jelent!"}}}},
    {Model::WarningKeys::rainstorm, { .title="Felhőszakadás",.iconMask="ts%1.gif",.description= {
                {1,"Intenzív záporból, zivatarból rövid idő alatt 25-30 mm-t meghaladó csapadék hullhat."},
                {2,"Intenzív záporból, zivatarból rövid idő alatt 50 mm-t meghaladó csapadék hullhat."}}}},
    {Model::WarningKeys::wind, { .title="Széllökés",.iconMask="wind%1.gif",.description={
                {1,"A várt legerősebb széllökések meghaladhatják a 70 km/h-t."},
                {2,"A várt legerősebb széllökések meghaladhatják a 90 km/h-t."},
                {3,"A várt legerősebb széllökések meghaladhatják a 110 km/h-t."}}}},
    {Model::WarningKeys::fzra, { .title="Ónos eső",.iconMask="fzra%1.gif",.description= {
                {1,"Gyenge ónos eső. A várt csapadékmennyiség általában néhány tized (> 0,1) mm."},
                {2,"Tartós (több órás) ónos eső. A várt csapadékmennyiség meghaladhatja az 1 mm-t."},
                {3,"Tartós (több órás) ónos eső. A várt csapadékmennyiség meghaladhatja az 5 mm-t."}}}},
    {Model::WarningKeys::snowdrift, { .title="Hófúvás",.iconMask="snowdrift%1.gif",.description={
                {1,"Gyenge hófúvás. A friss hóval fedett területeken a szél alacsony hótorlaszokat emelhet."},
                {2,"Hófúvás. A friss hóval fedett területeken a viharos szél magas hótorlaszokat emelhet."},
                {3,"Erős hófúvás. A friss hóval fedett területeken a viharos szél több helyen jelentős hóakadályokat emel."}}}},
    {Model::WarningKeys::rain, { .title="Eső",.iconMask="rain%1.gif",.description= {
                {1,"24 óra alatt több mint 20 mm csapadék hullhat."},
                {2,"24 óra alatt több mint 30 mm csapadék hullhat."},
                {3,"24 óra alatt több mint 50 mm csapadék hullhat."}}}},
    {Model::WarningKeys::snow, { .title="Havazás",.iconMask="snow%1.gif",.description={
                {1,"12 óra alatt 5 cm-t meghaladó friss hó hullhat."},
                {2,"24 óra alatt 20 cm-t meghaladó friss hó hullhat."},
                {3,"24 óra alatt 30 cm-t meghaladó friss hó hullhat."}}}},
    {Model::WarningKeys::coldx, { .title="Extrém hideg",.iconMask="coldx%1.gif",.description={
                {1,"A hőmérséklet - 15 °C alá csökkenhet."},
                {2,"A hőmérséklet - 20 °C alá csökkenhet."},
                {3,"A hőmérséklet - 25 °C alá csökkenhet."}}}},
    {Model::WarningKeys::hotx, { .title="Hőség",.iconMask="hotx%1.gif",.description={
                {1,"A napi középhőmérséklet várhatóan eléri vagy meghaladja a 25 °C-ot."},
                {2,"A napi középhőmérséklet várhatóan eléri vagy meghaladja a 27 °C-ot."},
                {3,"A napi középhőmérséklet 29 °C felett alakulhat."}}}},
    {Model::WarningKeys::fog, { .title="Tartós, sűrű köd",.iconMask="fog%1.gif",.description={
                {1,"Tartós (> 6 óra) sűrű köd (látástávolság pár száz méter) várható."}}}}
};

DoWork::DoWork(QObject *parent) :QObject(parent)
{

}

auto DoWork::init(const DoWorkInit& m) -> bool
{
    _isInited = false;

    _currentWeatherKeys = m.settings.currentWeather;
    _currentWarningKeys = m.settings.currentWarning;

    if(!_httpHelper.init(m.settings.host, m.settings.port)) return _isInited;
    if(!_httpHelper_idokep.init(m.settings.host_idokep, -1)) return _isInited;
    if(!_httpHelper_met.init(m.settings.host_met, -1)) return _isInited;

    QObject::connect(&_httpHelper, SIGNAL(ResponseOk(QUuid,QString,QByteArray)),
                     this, SLOT(ResponseOkAction(QUuid,QString,QByteArray)));
    QObject::connect(&_httpHelper_idokep, SIGNAL(ResponseOk(QUuid,QString,QByteArray)),
                     this, SLOT(ResponseOkAction(QUuid,QString,QByteArray)));
    QObject::connect(&_httpHelper_met, SIGNAL(ResponseOk(QUuid,QString,QByteArray)),
                     this, SLOT(ResponseOkAction(QUuid,QString,QByteArray)));


    _isInited = true;
    return true;
}

QUuid DoWork::GetCheckin()
{
    return _httpHelper.GetAction(CHECKIN);
}

QUuid DoWork::GetApiver()
{
    return _httpHelper.GetAction(APIVER);
}

QUuid DoWork::GetFeatureRequest()
{
    return _httpHelper.GetAction(FEATURE_REQUEST);
}

QUuid DoWork::GetCallsAnswer()
{
    return _httpHelper.GetAction(CALLS_ANSWER);
}

QUuid DoWork::GetCallsHangup()
{
    return _httpHelper.GetAction(CALLS_HANGUP);
}

QUuid DoWork::GetCallsHangupAll()
{
    return _httpHelper.GetAction(CALLS_HANGUPALL);
}

QUuid DoWork::GetCallsDial()
{
    return _httpHelper.GetAction(CALLS_DIAL);
}

QUuid DoWork::GetMediaPlay()
{
    return _httpHelper.GetAction(MEDIA_PLAY);
}

QUuid DoWork::GetMediaPause()
{
    return _httpHelper.GetAction(MEDIA_PAUSE);
}

QUuid DoWork::GetMediaSkip()
{
    return _httpHelper.GetAction(MEDIA_SKIP);
}

QUuid DoWork::GetMediaBack()
{
    return _httpHelper.GetAction(MEDIA_BACK);
}

ViewModel::State DoWork::GetState(const Model::Device &device, const Model::Media &media, const Model::Calls &calls)
{
    ViewModel::State rm;
//    if(_data.calls.connected){
//        if(device.connected){
//            rm.deviceMsg="változott?";
//        } else{
//            rm.deviceMsg="lecsatlakozott";
//        }
//    }else{
//        if(device.connected){
//            rm.deviceMsg="felcsatlakozott";
//        }
//    }
    rm.callsMsg="";rm.callsState=ViewModel::State::ConnectionState::unknown;
       if(_data.device.connected){
           if(device.connected){
               if(_data.device==device){
                   rm.deviceMsg="változatlan";
                   rm.deviceState = ViewModel::State::ConnectionState::unchanged;
               } else {
                   rm.deviceMsg="változott";
                   rm.deviceState = ViewModel::State::ConnectionState::changed;
               }
           } else{
               rm.deviceMsg="lecsatlakozott";
               rm.deviceState = ViewModel::State::ConnectionState::deleted;
           }
       }else{
           if(device.connected){
               rm.deviceMsg="felcsatlakozott";
               rm.deviceState = ViewModel::State::ConnectionState::created;
           } else{
               rm.deviceMsg="nincs device";
               rm.deviceState = ViewModel::State::ConnectionState::unknown;
           }
       }
       if(_data.media.status!=Model::Media::Status::unknown){
           if(media.status!=Model::Media::Status::unknown){
               if(_data.media==media){
                   rm.mediaMsg="változatlan";
                   rm.mediaState = ViewModel::State::ConnectionState::unchanged;
               } else{
                   rm.mediaMsg="változott";
                   rm.mediaState = ViewModel::State::ConnectionState::changed;
               }
           } else{
               rm.mediaMsg="nincs média";
               rm.mediaState = ViewModel::State::ConnectionState::deleted;
           }
       } else{
           if(media.status!=Model::Media::Status::unknown &&
                   (media.title.toLower()!="not provided")){
               rm.mediaMsg="új média";
               rm.mediaState = ViewModel::State::ConnectionState::created;
           } else {
               rm.mediaMsg="nincs média";
               rm.mediaState = ViewModel::State::ConnectionState::unknown;
           }
       }
       return rm;
}

void DoWork::ResponseOkAction(const QUuid& guid, const QString& action,  QByteArray s){
    if(action==CHECKIN) GetCheckinResponse(guid,s);
    else if(action==APIVER) GetApiverResponse(guid,s);
    else if(action==FEATURE_REQUEST) GetFeatureRequestResponse(guid,s);
    else if(action==CURRENTWEATHER) GetCurrentWeatherResponse(guid,s);
    else if(action==CURRENTWEATHERICON) GetCurrentWeatherIconResponse(guid,s);    
    else if(action==CURRENTWARNING) GetCurrentWarningResponse(guid,s);
    else if(action==CURRENTWARNINGMAP) GetCurrentWarningMapResponse(guid,s);
    else zInfo("unknown action: "+action);
}

void DoWork::GetCheckinResponse(const QUuid& guid, QByteArray s){
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(s, &errorPtr);
    QJsonObject rootobj = doc.object();
    ResponseModel::Checkin r(guid);

    if(rootobj.isEmpty()){
        r.msg = "no response";
    }else{
        auto deviceJ = rootobj.value("device").toObject();
        if(deviceJ.isEmpty()){
            r.msg = "no device";
        }else{
            if(!deviceJ.isEmpty()) r.device = Model::Device::JsonParse(deviceJ);
            if(!r.msg.isEmpty()) r.msg+='\n';
            r.msg += "device: "+r.device.toString();
        }
        auto mediaJ = rootobj.value("media").toObject();
        if(mediaJ.isEmpty()){
            r.msg = "no media";
        }else{
            if(!mediaJ.isEmpty()) r.media=Model::Media::JsonParse(mediaJ);
            if(!r.msg.isEmpty()) r.msg+='\n';
            r.msg += "media: "+r.media.toString();
        }

        auto callsJ = rootobj.value("calls").toObject();
        if(callsJ.isEmpty()){
            r.msg = "no calls";
        }else{

            if(!callsJ.isEmpty()) r.calls=Model::Calls::JsonParse(callsJ);
            if(!r.msg.isEmpty()) r.msg+='\n';
            r.msg += "calls: "+r.media.toString();
        }
    }

    emit ResponseConnectionAction(r);
}

void DoWork::GetApiverResponse(const QUuid& guid, QByteArray s){
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(s, &errorPtr);
    QJsonObject rootobj = doc.object();
    ResponseModel::GetApiVer r(guid);

    //Model::ApiVer m;
    if(rootobj.isEmpty()){
        r.msg = "no response";
    }else{
        r.apiVer = Model::ApiVer::JsonParse(rootobj);
        r.msg = "apiver: "+r.apiVer.toString();
    }

    emit ResponseGetApiverAction(r);
}

void DoWork::GetFeatureRequestResponse(const QUuid& guid, QByteArray s){
    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(s, &errorPtr);
    QJsonObject rootobj = doc.object();
    ResponseModel::GetFeature r(guid);

    //Model::Features m;
    if(rootobj.isEmpty()){
        r.msg = "no response";
    }else{
        r.features = Model::Features::JsonParse(rootobj);
        r.msg = "features: "+r.features.toString();
    }

    emit ResponseGetFeatureRequestAction(r);
}

/*weather*/

QUuid DoWork::GetCurrentWeather()
{
    return _httpHelper_idokep.GetAction(CURRENTWEATHER);
}

void DoWork::GetCurrentWeatherResponse(const QUuid& guid, QByteArray s){
    QString txt(s);
    ResponseModel::GetCurrentWeather r(guid);

    if(txt.isEmpty()){
        r.msg = "no response";
    } else{
        auto divs = HTMLHelper::GetNestedTagContent(txt, "div", _currentWeatherKeys.div);
        if(!divs.isEmpty())
        {
            auto cw=divs.first();
            r.currentWeather.shortDesc = HTMLHelper::GetDivContent(cw, _currentWeatherKeys.shortDesc).trimmed();
            r.currentWeather.title = HTMLHelper::GetDivContent(cw, _currentWeatherKeys.title).trimmed();
            r.currentWeather.value = HTMLHelper::GetDivContent(cw, _currentWeatherKeys.value).trimmed();
            r.currentWeather.temperature = HTMLHelper::GetDivContent(cw, _currentWeatherKeys.temperature).trimmed();
            r.currentWeather.sunrise.title = HTMLHelper::GetDivContent(cw, _currentWeatherKeys.sunrise).trimmed();
            r.currentWeather.sunrise.time = HTMLHelper::GetTime(r.currentWeather.sunrise.title);

            r.currentWeather.sunset.title = HTMLHelper::GetDivContent(cw, _currentWeatherKeys.sunset).trimmed();
            r.currentWeather.sunset.time = HTMLHelper::GetTime(r.currentWeather.sunset.title);

            auto divs2 = HTMLHelper::GetNestedTagContent(cw, "div", _currentWeatherKeys.icon);
            if(!divs2.isEmpty()){
               auto cw2=divs2.first();
               r.currentWeather.icon = HTMLHelper::GetImgSrc(cw2).trimmed();
            }
            r.msg = "hutty2";

        }
    }

    emit ResponseGetCurrentWeatherRequestAction(r);
}

QUuid DoWork::GetCurrentWeatherIcon(const QString& iconpath)
{
    return _httpHelper_idokep.DownloadFromHost(CURRENTWEATHERICON, iconpath);
}

void DoWork::GetCurrentWeatherIconResponse(const QUuid& guid, QByteArray s){

    ResponseModel::GetCurrentWeatherIcon r(guid);
    QPixmap pm;
    pm.loadFromData(s);
    r.pixmap = pm;
    emit ResponseGetCurrentWeatherIconRequestAction(r);
}

/*warning*/

QUuid DoWork::GetCurrentWarning()
{
    return _httpHelper_met.GetAction(CURRENTWARNING);
}

Model::WarningKeys DoWork::ParseWarningKeys(const QString& txt){
    if(txt.isEmpty()) return Model::unknown;
    else if(txt=="ts") return Model::ts;
    else if(txt=="rainstorm") return Model::rainstorm;
    else if(txt=="wind") return Model::wind;
    else if(txt=="fzra") return Model::fzra;
    else if(txt=="snowdrift") return Model::snowdrift;
    else if(txt=="rain") return Model::rain;
    else if(txt=="snow") return Model::snow;
    else if(txt=="coldx") return Model::coldx;
    else if(txt=="hotx") return Model::hotx;
    else if(txt=="fog") return Model::fog;
    return Model::unknown;
}

void DoWork::GetCurrentWarningResponse(const QUuid& guid, QByteArray s){
    QString txt(s);
    ResponseModel::GetCurrentWarning r(guid);

    if(txt.isEmpty()){
        r.msg = "no response";
    } else{
        auto divs = HTMLHelper::GetNestedTagContent(txt, "div", _currentWarningKeys.div);
        auto map_divs = HTMLHelper::GetNestedTagContent(txt, "div", _currentWarningKeys.map_div);
        auto uv_divs = HTMLHelper::GetNestedTagContent(txt, "div", _currentWarningKeys.uvB_div);
        if(!divs.isEmpty())
        {
            //auto tag_txt_list = _currentWarningKeys.tags.split(',');
            QList<HTMLHelper::Tag> tags;
            for(auto&t:_currentWarningKeys.tags){tags.append({.tag=t,.desc=""});}
            auto div = divs.first();
            auto lis = HTMLHelper::GetNestedTagContent(div, tags);
            int i=0;
            for(auto&li:lis){
                Model::Warning w;
                w.index=i++;
                w.title = HTMLHelper::GetDivContent(li, _currentWarningKeys.title).trimmed();
                auto imgs = HTMLHelper::GetNestedTagContent(li, "img", "");

                if(!imgs.isEmpty()&&imgs.count()>=2){
                     w.icon = HTMLHelper::GetImgSrc(imgs[0]).trimmed();
                     w.value_icon = HTMLHelper::GetImgSrc(imgs[1]).trimmed();
                     if(!w.value_icon.isEmpty())
                     {
                         w.value = Model::Warning::ParseValue(w.value_icon);
                         auto k1 = Model::Warning::ParseKey(w.icon);
                         w.key = ParseWarningKeys(k1);
                     }
                }
                r.currentWarning.warnings.insert(w.key, w);
            }         

            zInfo("CurrentWarning: "+QString::number(r.currentWarning.warnings.count()));
            r.msg = "hutty3";

        }
        if(!map_divs.isEmpty()){
            auto cw2=map_divs.first();
            r.currentWarning.map = HTMLHelper::GetImgSrc(cw2).trimmed();
        }
        if(!uv_divs.isEmpty()){
            auto cw2=uv_divs.first();
            auto uvBicon = HTMLHelper::GetImgSrc(cw2).trimmed();
            if(!uvBicon.isEmpty())
            {
                r.currentWarning.uvBlevel = Model::Warning::ParseValue(uvBicon);
            }
        }

    }

    emit ResponseGetCurrentWarningRequestAction(r);
}

//GetCurrentWarningMap
QUuid DoWork::GetCurrentWarningMap(const QString& iconpath)
{
    return _httpHelper_met.DownloadFromHost(CURRENTWARNINGMAP, iconpath);
}

void DoWork::GetCurrentWarningMapResponse(const QUuid& guid, QByteArray s){

    ResponseModel::GetCurrentWarningMap r(guid);
    QPixmap pm;
    pm.loadFromData(s);
    r.pixmap = pm;
    emit ResponseGetCurrentWarningMapRequestAction(r);
}
