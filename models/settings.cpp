#include "settings.h"
#include "logger.h"

#include <QFileInfo>
#include <QDir>
#include <QSettings>

#include <helpers/textfilehelper.h>
#include <helpers/filenamehelper.h>
#include <QCoreApplication>

Settings::Settings()
{

}

Settings Settings::Load(const QString& dir)
{
    const QString SETTINGS_FN = "settings.ini";

    QFileInfo fi(dir);
    if(fi.isFile()){
        zInfo("not a directory:"+dir);
        return{};
    }

    QString p(dir);
    if(dir.isEmpty()) p = qApp->applicationDirPath();
    QString absoluteFilePath = FileNameHelper::GetAbsolutFn(p+ "/" + SETTINGS_FN);


    QFileInfo fi2(absoluteFilePath);
    if(!fi2.exists()){
        zInfo("settings file not exisits:"+absoluteFilePath);
        return{};
    }

    QSettings::Format f;
    auto suffix = fi2.suffix();
    if(suffix=="ini")
        f = QSettings::IniFormat;
    else
        f = QSettings::InvalidFormat;

    if(f==QSettings::InvalidFormat) return {};

    QSettings q(absoluteFilePath, f);

    Settings s;

    s.host=q.value("host").toString();
    s.port=q.value("port").toInt();

    s.host_idokep=q.value("host_idokep").toString();
    s.host_met=q.value("host_met").toString();    

    s.currentWeather.div=q.value("current-weather-div").toString();
    s.currentWeather.shortDesc=q.value("current-weather-short-desc").toString();
    s.currentWeather.title=q.value("current-weather-title").toString();
    s.currentWeather.value=q.value("current-weather").toString();
    s.currentWeather.icon=q.value("current-weather-icon").toString();
    s.currentWeather.temperature=q.value("current-temperature").toString();
    s.currentWeather.sunrise=q.value("sunrise").toString();
    s.currentWeather.sunset=q.value("sunset").toString();

    s.currentWarning.div=q.value("warning-div").toString();
    s.currentWarning.tags = q.value("warning-tag").toStringList();
    //s.currentWarning.tag=q.value("warning-tag").toString();
    s.currentWarning.title=q.value("warning-title").toString();
    //s.currentWarning.icon=q.value("warning-icon").toString();
    //s.currentWarning.level_icon=q.value("warning-level-icon").toString();
    s.currentWarning.map=q.value("warning-map").toString();
    return s;
}



bool Settings::isValid()
{
    if(host.isEmpty()) return false;
    if(port<0||port>65535) return false;
    return true;
}
