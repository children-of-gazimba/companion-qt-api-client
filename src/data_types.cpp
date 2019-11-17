#include "data_types.h"

ResourceData::ResourceData()
    : UuidData()
    , name()
    , type()
{}

ResourceData::ResourceData(const QString &name, int type, const QString &uuid)
    : UuidData(uuid)
    , name(name)
    , type(type)
{}

ResourceData::ResourceData(const QJsonObject obj)
    : ResourceData()
{
    setFromJsonObject(obj);
}

void ResourceData::setFromJsonObject(const QJsonObject &obj)
{
    UuidData::setFromJsonObject(obj);
    if(obj.contains("name"))
        name = obj["name"].toString();
    if(obj.contains("type"))
        type = obj["type"].toInt();
}

const QJsonObject ResourceData::toJsonObject() const
{
    QJsonObject obj(UuidData::toJsonObject());
    obj["name"] = name;
    obj["type"] = type;
    return obj;
}

SoundData::SoundData()
    : UuidData()
    , resource()
    , local_path()
{}

SoundData::SoundData(const QString &local_path, const QString &uuid)
    : UuidData(uuid)
    , resource()
    , local_path(local_path)
{}

SoundData::SoundData(const QJsonObject obj)
    : SoundData()
{
    setFromJsonObject(obj);
}

void SoundData::setFromJsonObject(const QJsonObject &obj)
{
    UuidData::setFromJsonObject(obj);
    if(obj.contains("local_path"))
        local_path = obj["local_path"].toString();
    if(obj.contains("resource"))
        resource.setFromJsonObject(obj["resource"].toObject());
}

const QJsonObject SoundData::toJsonObject() const
{
    QJsonObject obj(UuidData::toJsonObject());
    obj["resource"] = resource.toJsonObject();
    obj["local_path"] = local_path;
    return obj;
}

UuidData::UuidData()
    : uuid()
{}

UuidData::UuidData(const QString &uuid)
    : uuid(uuid)
{}

UuidData::UuidData(const QJsonObject &obj)
    : uuid()
{
    setFromJsonObject(obj);
}

void UuidData::setFromJsonObject(const QJsonObject &obj)
{
    if(obj.contains("uuid") && obj["uuid"].isString())
        uuid = obj["uuid"].toString();
}

const QJsonObject UuidData::toJsonObject() const
{
    QJsonObject obj;
    obj["uuid"] = uuid;
    return obj;
}
