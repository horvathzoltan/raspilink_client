#ifndef CHECKINRESPONSE_H
#define CHECKINRESPONSE_H

#include <QString>
#include <QUuid>


struct CheckinResponseModel{

    CheckinResponseModel(const QUuid _guid){
        guid = _guid;
    }
    QString msg;
    QUuid guid;
};

#endif // CHECKINRESPONSE_H
