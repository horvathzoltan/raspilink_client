#include "httphelper.h"
#include "logger.h"

#include <QNetworkReply>
#include <QUrlQuery>
#include "zshortguid.h"

const QString HttpHelper::KEY=QStringLiteral("key");

HttpHelper::HttpHelper(QObject *parent)
    : QObject{parent}
{

}

bool HttpHelper::init(const QString& host)
{
    _url = QUrl(host);
}

QUuid HttpHelper::SendGet(const QString& msg)
{
    QString urltxt = R"(http://10.10.10.107:9098/{COMMAND})";
    auto urltxt2 = urltxt.replace("{COMMAND}", msg);
    QUrl url = QUrl(urltxt2);
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);

    auto guid = QUuid::createUuid();
    _actions.insert(guid, msg);
    auto guid2 = zShortGuid::Encode(guid);
    QUrlQuery q;
    q.addQueryItem(KEY, guid2);

    url.setQuery(q.query());
//    QSslConfiguration conf = request.sslConfiguration();
//    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
//    request.setSslConfiguration(conf);

    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinish(QNetworkReply*)));
    connect(mgr,SIGNAL(finished(QNetworkReply*)),mgr,SLOT(deleteLater()));

    QNetworkRequest request(url);
    mgr->get(request);
    return guid;
}

void HttpHelper::SendPost(const QString& source_lang, const QString& dest_lang, const QString& msg)
{
    QString urltxt = R"(https://api-free.deepl.com/v2/translate?auth_key=1bfdbee3-6605-3752-dd94-3b5ae775f090:fx&text={TEXT}&target_lang={LANG})";
    auto urltxt2 = urltxt.replace("{TEXT}", msg).replace("{LANG}",dest_lang);
    QUrl url = QUrl(urltxt2);

    QByteArray postData;

    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);

    QNetworkRequest request(url);

    QUrlQuery params;
    params.addQueryItem("POST", R"(/v2/translate?auth_key=1bfdbee3-6605-3752-dd94-3b5ae775f090:fx> HTTP/1.0)");
    params.addQueryItem("Host:", R"(api-free.deepl.com)");
    params.addQueryItem("User-Agent:", R"(wcode1)");
    params.addQueryItem("Accept:", R"(*/*)");
    params.addQueryItem("Content-Length:", R"(4096)");
    params.addQueryItem("content-type", R"(application/x-www-form-urlencoded)");

    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);

    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinish(QNetworkReply*)));
    connect(mgr,SIGNAL(finished(QNetworkReply*)),mgr,SLOT(deleteLater()));

    //QHttpMultiPart http;

    //QHttpPart receiptPart;
    //receiptPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"data\""));
    //receiptPart.setBody(postData);

    //http.append(receiptPart);

    mgr->post(request, params.query().toUtf8());
}

void HttpHelper::onFinish(QNetworkReply *rep)
{
    QByteArray b = rep->readAll();
    QString err = rep->errorString();
    auto url = rep->request().url();

    if(url.hasQuery()){
        auto queryString = url.query();
        auto q = QUrlQuery(queryString);
        if(q.hasQueryItem(KEY)){
            QString keyString=q.queryItemValue(KEY);
            auto guid = zShortGuid::Decode(keyString);
            auto action = _actions[guid];
            _actions.remove(guid);
            zInfo("reply: "+b);
            emit ResponseOk(guid, action, b);
        }
    }
}
