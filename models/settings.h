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

    Settings();
    static Settings Load(const QString& dir);
    bool isValid();
};

#endif // SETTINGS_H
