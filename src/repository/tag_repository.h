#ifndef TAGREPOSITORY_H
#define TAGREPOSITORY_H

#include <QObject>

class TagRepository : public QObject
{
    Q_OBJECT
public:
    explicit TagRepository(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TAGREPOSITORY_H
