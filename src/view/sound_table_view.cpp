#include "sound_table_view.h"

SoundTableView::SoundTableView(QWidget *parent)
    : QTableView(parent)
    , model_(new SoundTableModel(this))
{
    setModel(model_);
}

void SoundTableView::update()
{
    model_->update();
}
