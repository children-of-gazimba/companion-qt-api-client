#ifndef ABSTRACT_REPOSITORY_H
#define ABSTRACT_REPOSITORY_H

#include <QObject>
#include <QNetworkAccessManager>

#include "client_exceptions.h"

class AbstractRepository : public QObject
{
    Q_OBJECT

public:
    explicit AbstractRepository(QObject *parent = nullptr);
    explicit AbstractRepository(const QString& host, int port, const QString& scheme, QObject *parent = nullptr);
    virtual ~AbstractRepository() = default;

    void setSslConfiguration(const QSslConfiguration& ssl_conf);
    const QSslConfiguration& getSslConfiguration() const;

    void setApiToken(const QString& api_token);
    const QString& getApiToken() const;
    void loadApiTokenFromJsonFile(const QString& path = "../src/secret.json");

    void setHost(const QString& host);
    const QString& getHost() const;

    void setPort(int port);
    int getPort() const;

    void setScheme(const QString& scheme);
    const QString& getScheme() const;

    static const QString toString(QNetworkAccessManager::Operation op);

    void setServerUrl(const QUrl& url);
    const QUrl getServerUrl() const;

signals:
    void requestError(QNetworkAccessManager::Operation operation,
                      const QString& path, const QString& error_string);

public slots:

protected slots:
    void handleReply(QNetworkReply* reply);

public:
    static ApiException NO_ACCESS_TOKEN;

protected:
    QNetworkRequest const buildRequest(const QUrl& url, bool apply_ssl_conf = true) const;

    QNetworkRequest const buildJsonRequest(const QJsonDocument& document, const QUrl& url,
                                           bool apply_ssl_config = true) const;
    QNetworkRequest const buildJsonRequest(const QByteArray& data, const QUrl& url,
                                           bool apply_ssl_config = true) const;

    QUrl const buildUrl(const QString &path, bool requires_access_token = true) const;
    QUrl const buildUrl(const QString &path, const QMap<QString, QString> &queryItems,
                        bool requires_access_token = true) const;

    QHttpMultiPart* buildFilePayload(const QString& file_path,
                                     const QString& http_part_name="data");

    virtual void onHandleError(QNetworkReply* reply);
    virtual void onHandleReply(QNetworkReply* reply) = 0;

    QNetworkAccessManager* network_access_;
    QSslConfiguration ssl_conf_;
    QString api_token_;

    QString host_;
    int port_;
    QString scheme_;

private:
    void initNetworkAccess();
};

#endif // ABSTRACT_REPOSITORY_H
