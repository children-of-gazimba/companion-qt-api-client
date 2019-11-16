#include "abstract_api_table_model.h"

AbstractApiTableModel::AbstractApiTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

void AbstractApiTableModel::setSslConfiguration(const QSslConfiguration &ssl_conf)
{
    repo()->setSslConfiguration(ssl_conf);
}

const QSslConfiguration &AbstractApiTableModel::getSslConfiguration() const
{
    return repo()->getSslConfiguration();
}

void AbstractApiTableModel::setApiToken(const QString &api_token)
{
    repo()->setApiToken(api_token);
}

const QString &AbstractApiTableModel::getApiToken() const
{
    return repo()->getApiToken();
}

void AbstractApiTableModel::loadApiTokenFromJsonFile(const QString &path)
{
    repo()->loadApiTokenFromJsonFile(path);
}

void AbstractApiTableModel::setHost(const QString &host)
{
    repo()->setHost(host);
}

const QString &AbstractApiTableModel::getHost() const
{
    return repo()->getHost();
}

void AbstractApiTableModel::setPort(int port)
{
    repo()->setPort(port);
}

int AbstractApiTableModel::getPort() const
{
    return repo()->getPort();
}

void AbstractApiTableModel::setScheme(const QString &scheme)
{
    repo()->setScheme(scheme);
}

const QString &AbstractApiTableModel::getScheme() const
{
    return repo()->getScheme();
}

void AbstractApiTableModel::setServerUrl(const QUrl &url)
{
    repo()->setServerUrl(url);
}

const QUrl AbstractApiTableModel::getServerUrl() const
{
    return repo()->getServerUrl();
}
