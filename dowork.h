#ifndef DOWORK_H
#define DOWORK_H

#include <QCoreApplication>
#include <models/checkinresponse.h>
#include "models/mainviewmodel.h"
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

    struct Params{
    public:
        QString inFile;
        QString outFile;
        bool isBackup;
        bool isTest = false;

        bool IsValid();

        static Params Parse(const QCoreApplication& app);
    };


    bool init(Params p);
    MainViewModel::DoWorkRModel Work1(MainViewModel::DoWorkModel m);
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
    //bool _isEventLoopNeeded = false;
    Params params;
    //QMap<QString, Wcode> _wcodes;
    HttpHelper _httpHelper;
    //Result doWork2();
    //QByteArray _prevCheckin;
    void GetCheckinResponse(const QUuid& guid, QByteArray s);
    void GetApiverResponse(QByteArray s);
    void GetFeatureRequestResponse(QByteArray s);

signals:
    void ResponseConnectionAction(CheckinResponseModel);
    void ResponseGetApiverAction(QString);
    void ResponseGetFeatureRequestAction(QString);

private slots:
    void ResponseOkAction(const QUuid& guid, const QString& action, QByteArray s);
};

#endif // DOWORK_H
