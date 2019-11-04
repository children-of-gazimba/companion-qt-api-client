#include "sound_table_view.h"

SoundTableView::SoundTableView(QWidget *parent)
    : QTableView(parent)
    , model_(new SoundTableModel(this))
    , context_menu_(new QMenu(this))
{
    setModel(model_);

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &SoundTableView::customContextMenuRequested,
            this, &SoundTableView::onContextMenuRequested);

}

void SoundTableView::update()
{
    model_->update();
}

void SoundTableView::onContextMenuRequested(const QPoint &p)
{
    qDebug() << "context menu requested";
    QMenu *menu = new QMenu;
    menu->addAction(tr("delete"), this, [=]() {
        model_->removeRows(selectionModel()->selectedRows());
    });
    menu->exec(mapToGlobal(p));
}
