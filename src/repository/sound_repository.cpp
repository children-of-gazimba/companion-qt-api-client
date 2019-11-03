#include "sound_repository.h"
#include "sound_repository.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QNetworkReply>
#include <QDebug>
#include <QUrlQuery>

SoundRepository::SoundRepository(QObject *parent)
    : AbstractRepository(parent)
{}

void SoundRepository::onHandleReply(QNetworkReply *reply)
{
    QString path = reply->request().url().path();

    if(path.compare("/api/sound") == 0) {
        if(reply->operation() == QNetworkAccessManager::GetOperation)
            return onGetSoundsReply(reply);
    }

    qDebug() << "no handling for Operation"
             << toString(reply->operation())
             << " defined on " << path << "\n";
}

void SoundRepository::getSounds()
{
    network_access_->get(buildRequest(buildUrl("/api/sound")));
}

void SoundRepository::onGetSoundsReply(QNetworkReply *reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());

    QList<SoundData> sounds;
    if(doc.isArray()) {
        for(auto val: doc.array()) {
            if(val.isObject())
                sounds.append(SoundData::fromJsonObject(val.toObject()));
        }
    }

    if(sounds.size() > 0)
        emit soundsReceived(sounds);
}
