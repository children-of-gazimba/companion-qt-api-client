#include "main_window.h"

#include <QDebug>
#include <QTimer>
#include <QTextStream>
#include <QVBoxLayout>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QHeaderView>
#include <QFileDialog>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , button_refresh_(new QPushButton(tr("refresh"), this))
    , button_upload_(new QPushButton(tr("upload"), this))
    , sound_view_(new QTableView(this))
    , sound_model_(new SoundTableModel(this))
{
    initWidgets();
    initLayout();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initWidgets()
{
    sound_view_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    sound_view_->setModel(sound_model_);

    sound_view_->setSelectionBehavior(QAbstractItemView::SelectRows);
    sound_view_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(sound_view_, &QTableView::customContextMenuRequested,
            this, [=](const QPoint& p){
        QMenu *menu = new QMenu;
        menu->addAction(tr("delete"), this, [=]() {
            sound_model_->removeRows(sound_view_->selectionModel()->selectedRows());
        });
        menu->exec(mapToGlobal(p));
    });

    connect(button_refresh_, &QPushButton::clicked,
            sound_model_, &SoundTableModel::update);
    connect(button_upload_, &QPushButton::clicked,
            this, [=](){
        QString file = QFileDialog::getOpenFileName(
            this, tr("Upload Sound"), "",
            tr("Sound Files (*.mp3 *.wav)")
        );
        if(file.size() > 0)
            sound_model_->createSound(QUrl(file).fileName(), file, true);
    });
}

void MainWindow::initLayout()
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(sound_view_);
    layout->addWidget(button_refresh_);
    layout->addWidget(button_upload_);
    QWidget* container = new QWidget(this);
    container->setLayout(layout);
    setCentralWidget(container);
}

