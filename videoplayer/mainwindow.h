#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMediaPlayer>
#include <QVideoWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(char *mediafile, QWidget *parent = 0);
    ~MainWindow();

private:
    QMediaPlayer *player;
    QVideoWidget *vwidget;

    Ui::MainWindow *ui;
private slots:
     void PlayerStateChanged(QMediaPlayer::State);
};

#endif // MAINWINDOW_H
