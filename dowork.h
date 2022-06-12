#ifndef DOWORK_H
#define DOWORK_H

#include <QCoreApplication>
#include <models/responsemodel.h>
#include "models/commandlineargs.h"
#include "models/viewmodel.h"
#include "helpers/httphelper.h"


class DoWork: public QObject
{
    Q_OBJECT
private:
    static const QString CHECKIN;
    static const QString APIVER;
    static const QString FEATURE_REQUEST;

    static const QString CALLS_ANSWER;
    static const QString CALLS_HANGUP;
    static const QString CALLS_HANGUPALL;
    static const QString CALLS_DIAL;

    static const QString MEDIA_PLAY;
    static const QString MEDIA_PAUSE;
    static const QString MEDIA_SKIP;
    static const QString MEDIA_BACK;
    //enum GetRequestType{checkin, apiver};
public:
    explicit DoWork(QObject *parent = nullptr);   

    bool init(CommadLineArgs args);
    QUuid GetCheckin();
    QUuid GetApiver();
    QUuid GetFeatureRequest();

    QUuid GetCallsAnswer();
    QUuid GetCallsHangup();
    QUuid GetCallsHangupAll();
    QUuid GetCallsDial();

    QUuid GetMediaPlay();
    QUuid GetMediaPause();
    QUuid GetMediaSkip();
    QUuid GetMediaBack();
private:
    bool _isInited = false;
    //CommadLineArgs params;
    HttpHelper _httpHelper;
    void GetCheckinResponse(const QUuid& guid, QByteArray s);
    void GetApiverResponse(const QUuid& guid, QByteArray s);
    void GetFeatureRequestResponse(const QUuid& guid, QByteArray s);

signals:
    void ResponseConnectionAction(ResponseModel::Checkin);
    void ResponseGetApiverAction(ResponseModel::GetApiVer);
    void ResponseGetFeatureRequestAction(ResponseModel::GetFeature);

private slots:
    void ResponseOkAction(const QUuid& guid, const QString& action, QByteArray s);
};

#endif // DOWORK_H
