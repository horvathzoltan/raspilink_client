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

bool HttpHelper::init(const QString& host, int port)
{
    _inited = false;

    if(host.startsWith("http")){
        _url = QUrl(host);
    }else{
        _url = QUrl();
        _url.setScheme("http");
        _url.setHost(host);
    }

    if(port>=0&&port<=65535) _url.setPort(port);
    if(!_url.isValid()) return _inited;
    _inited = true;
    return _inited;
}

QUuid HttpHelper::SendGet(const QString& action)
{
    if(!action.startsWith('#')) _url.setPath((action.startsWith('/')?action:'/'+action));
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);

    auto guid = QUuid::createUuid();
    _actions.insert(guid, action);
    auto guid2 = zShortGuid::Encode(guid);
    QUrlQuery q;
    q.addQueryItem(KEY, guid2);

    _url.setQuery(q.query());

    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinish(QNetworkReply*)));
    connect(mgr,SIGNAL(finished(QNetworkReply*)),mgr,SLOT(deleteLater()));

    QNetworkRequest request(_url);
    if(_url.scheme()=="https"){
        QSslConfiguration conf = request.sslConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        request.setSslConfiguration(conf);
    }
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
            if(b.isEmpty())
            {
                QString msg = "no reply";
                if(!err.isEmpty()) msg+=" err: " + err;
                msg+=" url: " + url.toDisplayString();
                zInfo(msg);
                //emit ResponseErr(guid, action, b);
            }
            else{
                zInfo("reply: "+(b.count()>20?b.mid(0,20)+"...":b));
                emit ResponseOk(guid, action, b);
            }
        }
    }
}
