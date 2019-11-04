#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTableView>

#include "model/sound_table_model.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initWidgets();
    void initLayout();

    QPushButton* button_refresh_;
    QPushButton* button_upload_;
    QTableView* sound_view_;
    SoundTableModel* sound_model_;
};
#endif // MAINWINDOW_H
