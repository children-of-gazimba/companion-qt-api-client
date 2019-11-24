#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTableView>
#include <QLineEdit>
#include <QMediaPlayer>

#include "model/sound_table_model.h"
#include "model/tag_table_model.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initWidgets();
    void initLayout();

    const QString getCurrentServerUrl() const;

    QLineEdit* url_edit_;
    QLineEdit* token_edit_;
    QPushButton* button_refresh_;
    QPushButton* button_upload_;
    QTableView* sound_view_;
    SoundTableModel* sound_model_;
    QLineEdit* sound_label_;
    QPushButton* button_play_;

    QLineEdit* tag_edit_;
    QTableView* tag_view_;
    TagTableModel* tag_model_;
    QPushButton* button_add_;

    QMediaPlayer* player_;
};
#endif // MAINWINDOW_H
