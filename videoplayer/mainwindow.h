#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QString>
#include <QMediaPlayer>
#include <QVideoWidget>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(char *mediafile, QWidget *parent = 0);
    ~MainWindow();

private:
    QMediaPlayer *player;
    QVideoWidget *vwidget;

private slots:
     void PlayerStateChanged(QMediaPlayer::State);
};

#endif // MAINWINDOW_H
