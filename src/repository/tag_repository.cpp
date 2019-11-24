#include "tag_repository.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QUrlQuery>
#include <QNetworkReply>



TagRepository::TagRepository(QObject *parent)
    : AbstractRepository(parent)
{}

void TagRepository::onHandleReply(QNetworkReply *reply)
{
    QString path = reply->request().url().path();

    if(path.compare("/api/tag") == 0) {
        if(reply->operation() == QNetworkAccessManager::GetOperation)
            return onGetTagsReply(reply);
        else if(reply->operation() == QNetworkAccessManager::PostOperation)
            return onCreateTagReply(reply);
        else if(reply->operation() == QNetworkAccessManager::DeleteOperation)
            return onDeleteTagsReply(reply);
    }
    else if(path.contains("/api/tag/data")) {
        if(reply->operation() == QNetworkAccessManager::PutOperation)
            return onUploadTagDataReply(reply);
    }

    qDebug() << "no handling for Operation"
             << toString(reply->operation())
             << " defined on " << path << "\n";
}

void TagRepository::parseTagsReceived(const QByteArray &body)
{
    auto doc = QJsonDocument::fromJson(body);

    QList<TagData> tags;
    if (doc.isArray()){
        for(auto val: doc.array()) {
            if (val.isObject())
                tags.append(TagData::fromJsonObject(val.toObject()));
        }
    }

    emit tagsReceived(tags);
}

void TagRepository::parseTagCreated(const QByteArray &body)
{
    auto doc = QJsonDocument::fromJson(body);
    if(doc.isObject())
        emit tagCreated(TagData::fromJsonObject(doc.object()));
}

void TagRepository::getTags()
{
    network_access_->get(buildRequest(buildUrl("/api/tag")));
}

void TagRepository::createTag(const QString &name, const QString& description)
{
    QJsonObject obj;
    obj["name"] = name;
    obj["description"] = description;
    auto body = QJsonDocument(obj).toJson();
    auto req = buildJsonRequest(body, buildUrl("/api/tag"));

    network_access_->post(req, body);
}

bool TagRepository::deleteTags(const QList<TagData> &tags)
{
    auto url = buildUrl("/api/tag");
    QUrlQuery query(url);
    QString uuids = "";
    for(auto tag: tags) {
        if(uuids.size() > 0)
            uuids += "%";
        uuids += tag.uuid;
    }
    query.addQueryItem("uuids", uuids);
    url.setQuery(query);

    network_access_->deleteResource(buildRequest(url));

    return true;
}


void TagRepository::onGetTagsReply(QNetworkReply *reply)
{
    parseTagsReceived(reply->readAll());
}

void TagRepository::onCreateTagReply(QNetworkReply *reply)
{
    parseTagCreated(reply->readAll());
}

void TagRepository::onUploadTagDataReply(QNetworkReply *reply)
{
    qDebug() << "Upload " << reply->request().url().path() << " finished.";
}

void TagRepository::onDeleteTagsReply(QNetworkReply *reply)
{
    parseTagsReceived(reply->readAll());
}

