#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>



class Settings
{
public:
    QString host;
    int port=-1;

    Settings();
    static Settings Load(const QString& dir);
    bool isValid();
};

#endif // SETTINGS_H
