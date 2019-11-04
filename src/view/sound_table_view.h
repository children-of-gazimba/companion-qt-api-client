#ifndef SOUNDTABLEVIEW_H
#define SOUNDTABLEVIEW_H

#include <QTableView>
#include <QMenu>

#include "model/sound_table_model.h"

class SoundTableView : public QTableView
{
    Q_OBJECT
public:
    explicit SoundTableView(QWidget *parent = nullptr);

signals:

public slots:
    void update();

protected slots:
    void onContextMenuRequested(const QPoint& p);

private:

    SoundTableModel* model_;
    QMenu* context_menu_;
};

#endif // SOUNDTABLEVIEW_H
