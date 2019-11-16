#include "sound_table_model.h"

SoundTableModel::SoundTableModel(QObject *parent)
    : AbstractApiTableModel(parent)
    , repo_(new SoundRepository(this))
    , sounds_()
{
    init();
}

AbstractRepository *SoundTableModel::repo() const
{
    return repo_;
}

int SoundTableModel::columnCount(const QModelIndex &) const
{
    return 2; // (uuid/)name, local_path
}

int SoundTableModel::rowCount(const QModelIndex &) const
{
    return sounds_.size();
}

QVariant SoundTableModel::data(const QModelIndex &index, int role) const
{
    if(!indexIsValid(index))
        return QVariant();

    if(role == Qt::DisplayRole) {
        if(index.column() == 0)
            return QVariant(sounds_[index.row()].resource.name);
        else if(index.column() == 1)
            return QVariant(sounds_[index.row()].local_path);
    }
    else if(role == Qt::EditRole) {
        if(index.column() == 0)
            return QVariant(sounds_[index.row()].uuid);
        else if(index.column() == 1)
            return QVariant(sounds_[index.row()].local_path);
    }

    return QVariant();
}

QVariant SoundTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // horizontal header (attribute names)
    if(orientation == Qt::Horizontal) {
        if(section < 0 || section >= columnCount())
            return QVariant();

        if(role == Qt::DisplayRole) {
            if(section == 0)
                return QVariant("Name");
            else if(section == 1)
                return QVariant("Local Path");
        }

        else if(role == Qt::EditRole) {
            if(section == 0)
                return QVariant("UUID");
            else if(section == 1)
                return QVariant("Path");
        }
    }

    // vertical header (number of record)
    else {
        if(section < 0 || section >= rowCount())
            return QVariant();
        if(role == Qt::DisplayRole || role == Qt::EditRole)
            return QVariant(section);
    }

    return QVariant();
}

Qt::ItemFlags SoundTableModel::flags(const QModelIndex &index) const
{
    if(!indexIsValid(index))
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index);
}

bool SoundTableModel::setData(const QModelIndex &, const QVariant &, int)
{
    // TODO support edit
    return false;
}

bool SoundTableModel::setHeaderData(int, Qt::Orientation, const QVariant &, int)
{
    return false;
}

bool SoundTableModel::removeColumns(int, int, const QModelIndex&)
{
    return false;
}

bool SoundTableModel::removeRows(const QModelIndexList &indexes)
{
    QList<SoundData> sounds;
    for(auto index: indexes) {
        if(indexIsValid(index))
            sounds.append(sounds_[index.row()]);
    }

    return repo_->deleteSounds(sounds);
}

bool SoundTableModel::removeRows(int row, int count, const QModelIndex&)
{
    QModelIndexList indexes;
    for(int i = row; i < row+count; ++i) {
        auto idx = index(i, 0);
        if(!indexIsValid(idx))
            break;
        indexes.append(idx);
    }
    return removeRows(indexes);
}

void SoundTableModel::update()
{
    repo_->getSounds();
}

void SoundTableModel::createSound(const QString &name, const QString &local_path, bool upload_data)
{
    repo_->createSound(name, local_path, upload_data);
}

const QUrl SoundTableModel::getStreamUrl(const QModelIndex &index) const
{
    if(!indexIsValid(index))
        return QUrl();

    return repo_->getStreamUrl(sounds_[index.row()]);
}

void SoundTableModel::setSounds(const QList<SoundData> &sounds)
{
    onRepoReceivedSounds(sounds);
}

const SoundData &SoundTableModel::getSound(int row) const
{
    return sounds_[row];
}

const QList<SoundData> &SoundTableModel::getSounds() const
{
    return sounds_;
}

void SoundTableModel::onRepoReceivedSounds(const QList<SoundData> &sounds)
{
    sounds_ = sounds;

    emit layoutAboutToBeChanged();
    emit layoutChanged();
    emit dataChanged(index(0,0), index(rowCount(), columnCount()));
}

void SoundTableModel::onRepoCreatedSound(const SoundData &sound)
{
    sounds_.append(sound);

    emit layoutAboutToBeChanged();
    emit layoutChanged();
    emit dataChanged(index(rowCount()-1, 0), index(rowCount(), columnCount()));
}

bool SoundTableModel::indexIsValid(const QModelIndex &index) const
{
    return index.isValid() && index.row() < rowCount() && index.column() < columnCount();
}

void SoundTableModel::init()
{
    connect(repo_, &SoundRepository::soundsReceived,
            this, &SoundTableModel::onRepoReceivedSounds);

    connect(repo_, &SoundRepository::soundCreated,
            this, &SoundTableModel::onRepoCreatedSound);
}
