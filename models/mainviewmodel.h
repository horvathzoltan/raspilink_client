#ifndef MAINVIEWMODEL_H
#define MAINVIEWMODEL_H

#include <QString>


class MainViewModel
{
public:

    struct PhoneViewModelR{
        QString msg;
    };

    struct ConnectionViewModelR{
        QString msg;
    };

    struct DoWorkModel{
        int i;
    };

    struct DoWorkRModel{
        QString txt;
        //QMap<QString, Wcode> wcodes;
    };
};

#endif // MAINVIEWMODEL_H
