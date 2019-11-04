#ifndef SOUND_REPOSITORY_H
#define SOUND_REPOSITORY_H

#include "repository/abstract_repository.h"

#include <QNetworkAccessManager>
#include <functional>

#include "data_types.h"

class SoundRepository : public AbstractRepository
{
    Q_OBJECT
public:
    explicit SoundRepository(QObject *parent = nullptr);
    ~SoundRepository() override = default;

    void getSounds();
    void createSound(const QString& name, const QString& local_path,
                     bool upload_data = true);
    bool uploadSoundData(const SoundData& sound);
    bool deleteSounds(const QList<SoundData>& sounds);

    const QUrl getRemoteUrl(const SoundData& sound) const;

signals:
    void soundsReceived(const QList<SoundData>& sounds);
    void soundCreated(const SoundData& sound);

protected:
    void onHandleReply(QNetworkReply *reply) override;

    void parseSoundsReceived(const QByteArray& body);
    void parseSoundCreated(const QByteArray& body);

    void onGetSoundsReply(QNetworkReply *reply);
    void onCreateSoundReply(QNetworkReply *reply);
    void onUploadSoundDataReply(QNetworkReply *reply);
    void onDeleteSoundsReply(QNetworkReply* reply);

    QIODevice* upload_handle_;
};

#endif // SOUND_REPOSITORY_H
