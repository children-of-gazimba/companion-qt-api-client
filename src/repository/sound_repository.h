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

signals:
    void soundsReceived(const QList<SoundData>& sounds);

protected:
    void onHandleReply(QNetworkReply *reply) override;

    void onGetSoundsReply(QNetworkReply *reply);
};

#endif // SOUND_REPOSITORY_H
