#ifndef SOUNDTABLEVIEW_H
#define SOUNDTABLEVIEW_H

#include <QTableView>
#include "model/sound_table_model.h"

class SoundTableView : public QTableView
{
    Q_OBJECT
public:
    explicit SoundTableView(QWidget *parent = nullptr);

signals:

public slots:
    void update();

private:
    SoundTableModel* model_;
};

#endif // SOUNDTABLEVIEW_H
