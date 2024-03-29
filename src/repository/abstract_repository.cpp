#include "abstract_repository.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QNetworkReply>
#include <QDebug>
#include <QUrlQuery>
#include <QHttpPart>
#include <QMimeDatabase>
#include <QFileInfo>

ApiException AbstractRepository::NO_ACCESS_TOKEN = ApiException("Query requires Access Token, but none has been set (call setApiToken first).");

AbstractRepository::AbstractRepository(QObject *parent)
    : QObject(parent)
    , network_access_(new QNetworkAccessManager(this))
    , ssl_conf_()
    , api_token_()
    , host_("0.0.0.0")
    , port_(44100)
    , scheme_("http")
{
    initNetworkAccess();
}

AbstractRepository::AbstractRepository(const QString &host, int port, const QString &scheme, QObject *parent)
    : QObject(parent)
    , network_access_(new QNetworkAccessManager(this))
    , ssl_conf_()
    , api_token_()
    , host_(host)
    , port_(port)
    , scheme_(scheme)
{
    initNetworkAccess();
}

void AbstractRepository::loadApiTokenFromJsonFile(const QString &path)
{
    QString val;
    QFile file;
    file.setFileName(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject obj = d.object();
    if(obj.contains("access_token"))
        setApiToken(obj["access_token"].toString());
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
    return "";
}

void AbstractRepository::setServerUrl(const QUrl &url)
{
    setScheme(url.scheme());
    setPort(url.port());
    setHost(url.host());
}

const QUrl AbstractRepository::getServerUrl() const
{
    QUrl url;
    url.setHost(host_);
    url.setPort(port_);
    url.setScheme(scheme_);
    return url;
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

const QNetworkRequest AbstractRepository::buildJsonRequest(const QJsonDocument &document, const QUrl &url, bool apply_ssl_config) const
{
    QByteArray body = document.toJson();
    return buildJsonRequest(body, url, apply_ssl_config);
}

const QNetworkRequest AbstractRepository::buildJsonRequest(const QByteArray &data, const QUrl &url, bool apply_ssl_config) const
{
    QByteArray content_length = QByteArray::number(data.size());
    auto req = buildRequest(url, apply_ssl_config);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setHeader(QNetworkRequest::ContentLengthHeader, content_length);
    return req;
}

const QUrl AbstractRepository::buildUrl(const QString &path, bool requires_access_token) const
{
    return buildUrl(path, QMap<QString, QString>(), requires_access_token);
}

const QUrl AbstractRepository::buildUrl(const QString &path, const QMap<QString, QString> &queryItems, bool requires_access_token) const
{
    QUrl url(getServerUrl());
    url.setPath(path);
    QUrlQuery query;
    if(requires_access_token) {
        if(api_token_.size() == 0)
            throw NO_ACCESS_TOKEN;
        query.addQueryItem("access_token", api_token_);
    }
    for(auto key: queryItems.keys())
        query.addQueryItem(key, queryItems.value(key));
    url.setQuery(query);
    return url;
}

QHttpMultiPart *AbstractRepository::buildFilePayload(const QString &file_path, const QString &http_part_name)
{
    QHttpPart file_part;
    QMimeDatabase mime;
    QHttpMultiPart *file_data = new QHttpMultiPart(QHttpMultiPart::FormDataType, this);

    QFile *file = new QFile(file_path);
    file->open(QIODevice::ReadOnly);

    if (file->error() != QFile::NoError) {
      qDebug() << file->errorString().toLatin1();
      return nullptr;
    }

    QFileInfo fi(file->fileName());

    QString content_disposition = "form-data; name=\""+ http_part_name +"\"; ";
    content_disposition += "filename=\"" + fi.completeBaseName() + "." + fi.completeSuffix() + "\"";

    file_part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(content_disposition));
    file_part.setBodyDevice(file);
    file->setParent(file_data);
    file_data->append(file_part);

    return file_data;
}

void AbstractRepository::onHandleError(QNetworkReply *reply)
{
    emit requestError(reply->operation(), reply->request().url().path(),
                      reply->errorString());
}

void AbstractRepository::initNetworkAccess()
{
    connect(network_access_, &QNetworkAccessManager::finished,
            this, &AbstractRepository::handleReply);

    ssl_conf_ = QSslConfiguration::defaultConfiguration();
    ssl_conf_.setPeerVerifyMode(QSslSocket::VerifyNone);
}
