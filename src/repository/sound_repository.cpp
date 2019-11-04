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
#include <QNetworkRequest>
#include <QHttpPart>
#include <QMimeDatabase>
#include <QFileInfo>

SoundRepository::SoundRepository(QObject *parent)
    : AbstractRepository(parent)
    , upload_handle_(nullptr)
{}

void SoundRepository::onHandleReply(QNetworkReply *reply)
{
    QString path = reply->request().url().path();

    if(path.compare("/api/sound") == 0) {
        if(reply->operation() == QNetworkAccessManager::GetOperation)
            return onGetSoundsReply(reply);
        else if(reply->operation() == QNetworkAccessManager::PostOperation)
            return onCreateSoundReply(reply);
        else if(reply->operation() == QNetworkAccessManager::DeleteOperation)
            return onDeleteSoundsReply(reply);
    }
    else if(path.contains("/api/sound/data")) {
        if(reply->operation() == QNetworkAccessManager::PutOperation)
            return onUploadSoundDataReply(reply);
    }

    qDebug() << "no handling for Operation"
             << toString(reply->operation())
             << " defined on " << path << "\n";
}

void SoundRepository::parseSoundsReceived(const QByteArray &body)
{
    auto doc = QJsonDocument::fromJson(body);

    QList<SoundData> sounds;
    if(doc.isArray()) {
        for(auto val: doc.array()) {
            if(val.isObject())
                sounds.append(SoundData::fromJsonObject(val.toObject()));
        }
    }

    emit soundsReceived(sounds);
}

void SoundRepository::parseSoundCreated(const QByteArray &body)
{
    auto doc = QJsonDocument::fromJson(body);
    if(doc.isObject())
        emit soundCreated(SoundData::fromJsonObject(doc.object()));
}

void SoundRepository::getSounds()
{
    network_access_->get(buildRequest(buildUrl("/api/sound")));
}

void SoundRepository::createSound(const QString &name, const QString &local_path, bool upload_data)
{
    QJsonObject obj;
    obj["name"] = name;
    obj["local_path"] = local_path;
    auto body = QJsonDocument(obj).toJson();
    auto req = buildJsonRequest(body, buildUrl("/api/sound"));

    if(upload_data) {
        auto temp_access = new QNetworkAccessManager(this);
        connect(temp_access, &QNetworkAccessManager::finished,
                this, [=](QNetworkReply* reply){
            auto doc = QJsonDocument::fromJson(reply->readAll());
            if(doc.isObject()) {
                auto sound = SoundData::fromJsonObject(doc.object());
                emit soundCreated(sound);
                uploadSoundData(sound);
            }
            temp_access->deleteLater();
        });
        temp_access->post(req, body);
    }
    else {
        network_access_->post(req, body);
    }
}

bool SoundRepository::uploadSoundData(const SoundData &sound)
{
    auto file_payload = buildFilePayload(sound.local_path);
    if(file_payload == nullptr)
        return false;

    network_access_->put(buildRequest(buildUrl("/api/sound/data/"+sound.uuid)), file_payload);

    return true;
}

bool SoundRepository::deleteSounds(const QList<SoundData> &sounds)
{
    auto url = buildUrl("/api/sound");
    QUrlQuery query(url);
    QString uuids = "";
    for(auto sound: sounds) {
        if(uuids.size() > 0)
            uuids += "%";
        uuids += sound.uuid;
    }
    query.addQueryItem("uuids", uuids);
    url.setQuery(query);

    network_access_->deleteResource(buildRequest(url));

    return true;
}

void SoundRepository::onGetSoundsReply(QNetworkReply *reply)
{
    parseSoundsReceived(reply->readAll());
}

void SoundRepository::onCreateSoundReply(QNetworkReply *reply)
{
    parseSoundCreated(reply->readAll());
}

void SoundRepository::onUploadSoundDataReply(QNetworkReply *reply)
{
    qDebug() << "Upload " << reply->request().url().path() << " finished.";
}

void SoundRepository::onDeleteSoundsReply(QNetworkReply *reply)
{
    parseSoundsReceived(reply->readAll());
}
