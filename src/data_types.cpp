#include "data_types.h"

ResourceData::ResourceData()
    : name()
    , type()
    , uuid()
{}

ResourceData::ResourceData(const QString &name, int type, const QString &uuid)
    : name(name)
    , type(type)
    , uuid(uuid)
{}

const ResourceData ResourceData::fromJsonObject(const QJsonObject &obj)
{
    QString name = "", uuid = "";
    int type = 0;
    if(obj.contains("name"))
        name = obj["name"].toString();
    if(obj.contains("uuid"))
        uuid = obj["uuid"].toString();
    if(obj.contains("type"))
        type = obj["type"].toInt();
    return ResourceData(name, type, uuid);
}

SoundData::SoundData(const QString &local_path, const QString &uuid)
    : resource()
    , local_path(local_path)
    , uuid(uuid)
{}

const SoundData SoundData::fromJsonObject(const QJsonObject &obj)
{
    QString local_path = "", uuid = "";
    if(obj.contains("local_path"))
        local_path = obj["local_path"].toString();
    if(obj.contains("uuid"))
        uuid = obj["uuid"].toString();
    SoundData s(local_path, uuid);
    if(obj.contains("resource"))
        s.resource = ResourceData::fromJsonObject(obj["resource"].toObject());
    return s;
}
