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
#include <QNetworkConfigurationManager>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , url_edit_(new QLineEdit(this))
    , token_edit_(new QLineEdit(this))
    , button_refresh_(new QPushButton(tr("refresh"), this))
    , button_upload_(new QPushButton(tr("upload"), this))
    , sound_view_(new QTableView(this))
    , sound_model_(new SoundTableModel(this))
    , sound_label_(new QLineEdit(this))
    , button_play_(new QPushButton(tr("play"), this))
    , player_(new QMediaPlayer(this, QMediaPlayer::StreamPlayback))
{
    initWidgets();
    initLayout();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initWidgets()
{
    QNetworkConfiguration conf;

    sound_label_->setReadOnly(true);
    connect(sound_label_, &QLineEdit::textChanged,
            this, [=](const QString& text) {
        button_play_->setEnabled(text.size() > 0);
        if(player_->state() == QMediaPlayer::PlayingState)
            player_->stop();
        QNetworkRequest req;
        QSslConfiguration conf = QSslConfiguration::defaultConfiguration();
        conf.setPeerVerifyMode(QSslSocket::VerifyNone);
        req.setUrl(QUrl(text));
        req.setSslConfiguration(conf);
        player_->setMedia(QMediaContent(req));
    });

    connect(player_, &QMediaPlayer::stateChanged,
            this, [=](QMediaPlayer::State state){
        switch(state) {
            case QMediaPlayer::PlayingState: button_play_->setText(tr("stop")); break;
            default: button_play_->setText(tr("play"));
        }
    });

    button_play_->setEnabled(false);
    connect(button_play_, &QPushButton::clicked,
            this, [=](){
        switch(player_->state()) {
            case QMediaPlayer::PlayingState: player_->stop(); break;
            default: player_->play();
        }
    });

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
    sound_model_->loadApiTokenFromJsonFile("../src/secret.json");
    sound_model_->update();

    connect(sound_model_->repo(), &AbstractRepository::requestError,
            this, [=](QNetworkAccessManager::Operation op, const QString& path, const QString& errorString){
        QMessageBox box;
        box.setText(AbstractRepository::toString(op) +  " " + path + " error: " + errorString);
        box.exec();
    });

    connect(sound_view_, &QTableView::doubleClicked,
            this, [=](const QModelIndex& index) {
        sound_label_->setText(sound_model_->getStreamUrl(index).toString());
    });

    connect(button_refresh_, &QPushButton::clicked,
            this, [=](){
        sound_model_->setServerUrl(QUrl(url_edit_->text()));
        sound_model_->setApiToken(token_edit_->text());
        sound_model_->update();
    });

    connect(button_upload_, &QPushButton::clicked,
            this, [=](){
        QString file = QFileDialog::getOpenFileName(
            this, tr("Upload Sound"), "",
            tr("Sound Files (*.mp3 *.wav)")
        );
        if(file.size() > 0)
            sound_model_->createSound(QUrl(file).fileName(), file, true);
    });

    url_edit_->setText(sound_model_->getServerUrl().toString());
    token_edit_->setText(sound_model_->getApiToken());
}

void MainWindow::initLayout()
{

    QHBoxLayout* player_layout = new QHBoxLayout;
    player_layout->addWidget(sound_label_, 3);
    player_layout->addWidget(button_play_, 1);
    player_layout->setContentsMargins(0,0,0,0);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(url_edit_);
    layout->addWidget(token_edit_);
    layout->addWidget(sound_view_);
    layout->addWidget(button_refresh_);
    layout->addWidget(button_upload_);
    layout->addLayout(player_layout);

    QWidget* container = new QWidget(this);
    container->setLayout(layout);
    setCentralWidget(container);
}

