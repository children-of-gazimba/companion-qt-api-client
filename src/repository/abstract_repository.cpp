#include "abstract_repository.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QNetworkReply>
#include <QDebug>
#include <QUrlQuery>

AbstractRepository::AbstractRepository(QObject *parent)
    : QObject(parent)
    , network_access_(new QNetworkAccessManager(this))
    , ssl_conf_()
    , secrets_path_("../src/secret.json")
    , api_token_()
    , host_("0.0.0.0")
    , port_(5000)
    , scheme_("https")
{
    initNetworkAccess();
}

void AbstractRepository::setSslConfiguration(const QSslConfiguration &ssl_conf)
{
    ssl_conf_ = ssl_conf;
}

const QSslConfiguration &AbstractRepository::getSslConfiguration() const
{
    return ssl_conf_;
}

void AbstractRepository::setApiToken(const QString &api_token)
{
    api_token_ = api_token;
}

const QString &AbstractRepository::getApiToken() const
{
    return api_token_;
}

void AbstractRepository::setHost(const QString &host)
{
    host_ = host;
}

const QString &AbstractRepository::getHost() const
{
    return host_;
}

void AbstractRepository::setPort(int port)
{
    port_ = port;
}

int AbstractRepository::getPort() const
{
    return port_;
}

void AbstractRepository::setScheme(const QString &scheme)
{
    scheme_ = scheme;
}

const QString &AbstractRepository::getScheme() const
{
    return scheme_;
}

const QString AbstractRepository::toString(QNetworkAccessManager::Operation op)
{
    switch(op) {
        case QNetworkAccessManager::GetOperation: return "GET";
        case QNetworkAccessManager::PostOperation: return "POST";
        case QNetworkAccessManager::PutOperation: return "PUT";
        case QNetworkAccessManager::HeadOperation: return "HEAD";
        case QNetworkAccessManager::DeleteOperation: return "DELETE";
        case QNetworkAccessManager::CustomOperation: return "CUSTOM";
        case QNetworkAccessManager::UnknownOperation: return "UNKNOWN";
    }
}

void AbstractRepository::handleReply(QNetworkReply *reply)
{
    if (reply->error())
        onHandleError(reply);
    else
        onHandleReply(reply);
}

const QNetworkRequest AbstractRepository::buildRequest(const QUrl &url, bool apply_ssl_conf) const
{
    QNetworkRequest request;
    request.setUrl(url);
    if(apply_ssl_conf)
        request.setSslConfiguration(ssl_conf_);
    return request;
}

const QUrl AbstractRepository::buildUrl(const QString &path, bool requires_access_token) const
{
    return buildUrl(path, QMap<QString, QString>(), requires_access_token);
}

const QUrl AbstractRepository::buildUrl(const QString &path, const QMap<QString, QString> &queryItems, bool requires_access_token) const
{
    QUrl url;
    url.setHost(host_);
    url.setPort(port_);
    url.setScheme(scheme_);
    url.setPath(path);
    QUrlQuery query;
    if(requires_access_token)
        query.addQueryItem("access_token", api_token_);
    for(auto key: queryItems.keys())
        query.addQueryItem(key, queryItems.value(key));
    url.setQuery(query);
    return url;
}

void AbstractRepository::onHandleError(QNetworkReply *reply)
{
    qDebug() << reply->errorString() << "\n";
}

void AbstractRepository::initNetworkAccess()
{
    loadApiToken();

    connect(network_access_, &QNetworkAccessManager::finished,
            this, &AbstractRepository::handleReply);

    ssl_conf_ = QSslConfiguration::defaultConfiguration();
    ssl_conf_.setPeerVerifyMode(QSslSocket::VerifyNone);
}

void AbstractRepository::loadApiToken()
{
    QString val;
    QFile file;
    file.setFileName(secrets_path_);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    qDebug() << "content of 'secret.json': " << val;

    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject obj = d.object();
    if(obj.contains("access_token"))
        api_token_ = obj["access_token"].toString();
}
