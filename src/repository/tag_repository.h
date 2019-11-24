#ifndef TAG_REPOSITORY_H
#define TAG_REPOSITORY_H

#include "repository/abstract_repository.h"

#include "data_types.h"


class TagRepository : public AbstractRepository
{
    Q_OBJECT
public:
    explicit TagRepository(QObject *parent = nullptr);
    ~TagRepository() override = default;

    void getTags();
    void createTag(const QString& name, const QString& description);
    bool deleteTags(const QList<TagData>& tags);


signals:
    void tagsReceived(const QList<TagData>& tags);
    void tagCreated(const TagData& tag);

protected:
    void onHandleReply(QNetworkReply *reply) override;

    void parseTagsReceived(const QByteArray& body);
    void parseTagCreated(const QByteArray& body);

    void onGetTagsReply(QNetworkReply *reply);
    void onCreateTagReply(QNetworkReply *reply);
    void onUploadTagDataReply(QNetworkReply *reply);
    void onDeleteTagsReply(QNetworkReply *reply);
};

#endif // TAG_REPOSITORY_H
