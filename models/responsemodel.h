#ifndef RESPONSEMODEL_H
#define RESPONSEMODEL_H

#include <QString>
#include <QUuid>

namespace ResponseModel{

    struct Base{

        Base(const QUuid _guid){
            guid = _guid;
        }
        QString msg;
        QUuid guid;
    };

    struct Checkin : public Base{
        Checkin(const QUuid _guid):Base(_guid){};
    };

    struct GetApiVer: public Base{
        GetApiVer(const QUuid _guid):Base(_guid){};
    };

    struct GetFeature: public Base{
        GetFeature(const QUuid _guid):Base(_guid){};
    };
};
#endif // RESPONSEMODEL_H
