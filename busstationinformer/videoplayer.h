#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QMainWindow>
#include <QTimer>
#include "mainwindow.h"
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMediaPlaylist>

class videoplayer : public QObject
{
    Q_OBJECT
public:
    explicit videoplayer(MainWindow *w=nullptr);
    ~videoplayer();
    void PlayVideo(void);
    void StopVideo(void);
signals:
     // void currentIndexChanged(int);
private:
     QMediaPlayer player;
     QVideoWidget videowidget;
     QMediaPlaylist playlist;
private slots:
     void PlayerStateChanged(QMediaPlayer::State);
     void PlaylistIndexChanged(int position);

     void displayErrorMessage(void);

};

#endif // VIDEOPLAYER_H
