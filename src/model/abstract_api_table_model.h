#ifndef ABSTRACTAPITABLEMODEL_H
#define ABSTRACTAPITABLEMODEL_H

#include <QAbstractTableModel>

#include "repository/abstract_repository.h"

class AbstractApiTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit AbstractApiTableModel(QObject *parent = nullptr);

    virtual AbstractRepository* repo() const = 0;

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

    void setServerUrl(const QUrl& url);
    const QUrl getServerUrl() const;

signals:
    void requestError(QNetworkAccessManager::Operation operation,
                      const QString& path, const QString& error_string);


public slots:
};

#endif // ABSTRACTAPITABLEMODEL_H
