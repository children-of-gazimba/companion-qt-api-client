#ifndef TAG_TABLE_MODEL_H
#define TAG_TABLE_MODEL_H

#include "abstract_api_table_model.h"

#include "repository/tag_repository.h"
#include "data_types.h"

class TagTableModel : public AbstractApiTableModel
{
    Q_OBJECT
public:
    explicit TagTableModel(QObject *parent = nullptr);
    ~TagTableModel() = default;

    AbstractRepository *repo() const;

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

    void createTag(const QString& name, const QString& description);

    /* set data from external source */
    void setTags(const QList<TagData>& tags);

    const TagData& getTag(int row) const;

    const QList<TagData>& getTags() const;

signals:

public slots:

private slots:
    void onRepoReceivedTags(const QList<TagData>& tags);
    void onRepoCreatedTag(const TagData& tag);

private:
    /* validates existance of given QModelIndex for this model **/
    bool indexIsValid(const QModelIndex&) const;

    void init();

    TagRepository* repo_;
    QList<TagData> tags_;
};

#endif // TAG_TABLE_MODEL_H
