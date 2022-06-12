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

    QUuid SendGet(const QString &msg);
public slots:
    void onFinish(QNetworkReply *rep);
signals:
    void ResponseOk(const QUuid& guid, const QString& key, QByteArray s);

private:
    bool _inited = false;
    QUrl _url;
    QMap<QUuid, QString> _actions;
};

#endif // HTTPHELPER_H
