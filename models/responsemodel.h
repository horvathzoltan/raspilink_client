#ifndef RESPONSEMODEL_H
#define RESPONSEMODEL_H

#include "model.h"

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
        Model::Device device;
        Model::Media media;
        //Model::Calls calls;
    };

    struct GetApiVer: public Base{
        GetApiVer(const QUuid _guid):Base(_guid){};
        Model::ApiVer apiVer;
    };

    struct GetFeature: public Base{
        GetFeature(const QUuid _guid):Base(_guid){};
        Model::Features features;
    };
};
#endif // RESPONSEMODEL_H
