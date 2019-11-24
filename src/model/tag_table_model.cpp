#include "tag_table_model.h"

TagTableModel::TagTableModel(QObject *parent)
    : AbstractApiTableModel(parent)
    , repo_(new TagRepository(this))
    , tags_()
{
  init();
}

AbstractRepository *TagTableModel::repo() const
{
    return repo_;
}

int TagTableModel::columnCount(const QModelIndex &parent) const
{
    return 1; // (uuid)name
}

int TagTableModel::rowCount(const QModelIndex &) const
{
    return tags_.size();
}

QVariant TagTableModel::data(const QModelIndex &index, int role) const
{
    if(!indexIsValid(index))
        return QVariant();

    if(role == Qt::DisplayRole) {
        if(index.column() == 0)
            return QVariant(tags_[index.row()].name);
//        else if(index.column() == 1)
//            return QVariant(tags_[index.row()].name);
    }
    else if(role == Qt::EditRole) {
        if(index.column() == 0)
            return QVariant(tags_[index.row()].uuid);
        else if(index.column() == 1)
            return QVariant(tags_[index.row()].name);
    }

    return QVariant();
}

QVariant TagTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // horizontal header (attribute names)
    if(orientation == Qt::Horizontal) {
        if(section < 0 || section >= columnCount())
            return QVariant();

        if(role == Qt::DisplayRole) {
            if(section == 0)
                return QVariant("Name");
//            else if(section == 1)
//                return QVariant("Local Path");
        }

        else if(role == Qt::EditRole) {
            if(section == 0)
                return QVariant("UUID");
            else if(section == 1)
                return QVariant("Name");
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

Qt::ItemFlags TagTableModel::flags(const QModelIndex &index) const
{
    if(!indexIsValid(index))
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index);
}

bool TagTableModel::setData(const QModelIndex &, const QVariant &, int)
{
    // TODO support edit
    return false;
}

bool TagTableModel::setHeaderData(int, Qt::Orientation, const QVariant &, int)
{
    return false;
}

bool TagTableModel::removeColumns(int, int, const QModelIndex&)
{
    return false;
}

bool TagTableModel::removeRows(const QModelIndexList &indexes)
{
    QList<TagData> tags;
    for(auto index: indexes) {
        if(indexIsValid(index))
            tags.append(tags_[index.row()]);
    }

    return repo_->deleteTags(tags);
}

bool TagTableModel::removeRows(int row, int count, const QModelIndex &parent)
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

void TagTableModel::update()
{
    repo_->getTags();
}

void TagTableModel::createTag(const QString &name, const QString &description)
{
    repo_->createTag(name, description);
}

void TagTableModel::setTags(const QList<TagData> &tags)
{
    onRepoReceivedTags(tags);
}

const TagData &TagTableModel::getTag(int row) const
{
    return tags_[row];
}

const QList<TagData> &TagTableModel::getTags() const
{
    return tags_;
}

void TagTableModel::onRepoReceivedTags(const QList<TagData> &tags)
{
    tags_ = tags;

    emit layoutAboutToBeChanged();
    emit layoutChanged();
    emit dataChanged(index(0,0), index(rowCount(), columnCount()));
}

void TagTableModel::onRepoCreatedTag(const TagData &tag)
{
    tags_.append(tag);

    emit layoutAboutToBeChanged();
    emit layoutChanged();
    emit dataChanged(index(rowCount()-1, 0), index(rowCount(), columnCount()));
}


bool TagTableModel::indexIsValid(const QModelIndex &index) const
{
    return index.isValid() && index.row() < rowCount() && index.column() < columnCount();
}

void TagTableModel::init()
{
    connect(repo_, &TagRepository::tagsReceived,
            this, &TagTableModel::onRepoReceivedTags);

    connect(repo_, &TagRepository::tagCreated,
            this, &TagTableModel::onRepoCreatedTag);
}
