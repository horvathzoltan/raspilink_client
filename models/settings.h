#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>



class Settings
{
public:
    QString host;
    int port=-1;
    QString host_idokep;
    QString host_met;

    struct CurrentWeather{
        QString div;
        QString shortDesc;
        QString title;
        QString value;
        QString icon;
        QString temperature;
        QString sunrise;
        QString sunset;
    };

    CurrentWeather currentWeather;

    Settings();
    static Settings Load(const QString& dir);
    bool isValid();
};

#endif // SETTINGS_H
