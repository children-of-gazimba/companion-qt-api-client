#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <QString>
#include <QJsonObject>

struct ResourceData {
    ResourceData();
    ResourceData(const QString& name, int type, const QString& uuid);

    static const ResourceData fromJsonObject(const QJsonObject& obj);

    QString name;
    int type;
    QString uuid;
};

struct SoundData {
    SoundData(const QString& local_path, const QString& uuid);

    static const SoundData fromJsonObject(const QJsonObject& obj);

    ResourceData resource;
    QString local_path;
    QString uuid;
};

struct TagData {
    TagData(const QString& name, const QString& uuid);

    static const TagData fromJsonObject(const QJsonObject& obj);

    QString name;
    QString uuid;
};

#endif // DATA_TYPES_H
