#ifndef SOUND_TABLE_MODEL_H
#define SOUND_TABLE_MODEL_H

#include <QAbstractTableModel>

#include "repository/sound_repository.h"
#include "data_types.h"

class SoundTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SoundTableModel(QObject *parent = nullptr);
    ~SoundTableModel() = default;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::DisplayRole);

    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex());

    bool removeRows(const QModelIndexList& indexes);

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    /* update model data from repository */
    void update();

    void createSound(const QString& name, const QString& local_path, bool upload_data = true);

    const QUrl getStreamUrl(const QModelIndex& index) const;

    /* set data from external source */
    void setSounds(const QList<SoundData>& sounds);

    const SoundData& getSound(int row) const;

signals:

public slots:

private slots:
    void onRepoReceivedSounds(const QList<SoundData>& sounds);
    void onRepoCreatedSound(const SoundData& sound);

private:
    /* validates existance of given QModelIndex for this model **/
    bool indexIsValid(const QModelIndex&) const;

    void init();

    SoundRepository* repo_;
    QList<SoundData> sounds_;
};

#endif // SOUND_TABLE_MODEL_H
