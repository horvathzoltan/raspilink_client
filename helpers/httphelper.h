#ifndef HTTPHELPER_H
#define HTTPHELPER_H

#include <QObject>
#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QMap>
#include <QUuid>

class HttpHelper : public QObject
{
    Q_OBJECT

private:
    static const QString KEY;
public:
    explicit HttpHelper(QObject *parent = nullptr);
    bool init(const QString& host, int port);

    void SendPost(const QString& source_lang, const QString& dest_lang, const QString& msg);

    QUuid SendGet(const QString& action);
    QUuid Download(const QString& action, const QString &url);
public slots:
    void onFinish(QNetworkReply *rep);
signals:
    void ResponseErr(const QUuid& guid, const QString& key);
    void ResponseOk(const QUuid& guid, const QString& key, QByteArray s);

private:
    bool _inited = false;
    QUrl _url;
    struct Action{
        QString action;
        QNetworkAccessManager *mgr;
    };

    QMap<QUuid, Action> _actions;
};

#endif // HTTPHELPER_H
