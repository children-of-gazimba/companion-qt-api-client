#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <QString>
#include <QJsonObject>

struct Serializable {
    virtual ~Serializable() = default;
    virtual void setFromJsonObject(const QJsonObject& obj) = 0;
    virtual const QJsonObject toJsonObject() const = 0;
};

struct UuidData : Serializable {
    UuidData();
    UuidData(const QString& uuid);
    UuidData(const QJsonObject& obj);

    virtual ~UuidData() override = default;

    virtual void setFromJsonObject(const QJsonObject &obj) override;
    virtual const QJsonObject toJsonObject() const override;

    QString uuid;
};

struct ResourceData : UuidData {
    ResourceData();
    ResourceData(const QString& name, int type, const QString& uuid);
    ResourceData(const QJsonObject obj);

    virtual void setFromJsonObject(const QJsonObject &obj) override;
    virtual const QJsonObject toJsonObject() const override;

    QString name;
    int type;
};

struct SoundData : UuidData {
    SoundData();
    SoundData(const QString& local_path, const QString& uuid);
    SoundData(const QJsonObject obj);

    virtual void setFromJsonObject(const QJsonObject &obj) override;
    virtual const QJsonObject toJsonObject() const override;

    ResourceData resource;
    QString local_path;
};

#endif // DATA_TYPES_H
