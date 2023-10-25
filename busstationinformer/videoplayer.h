#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QMainWindow>
#include <QTimer>
#include "mainwindow.h"
#include <QMediaPlayer>
#include <QVideoWidget>
//#include <QMediaPlaylist>
#include <QGraphicsView>
#include <QGraphicsVideoItem>
#include <QDir>

class videoplayer : public QObject
{
    Q_OBJECT
public:
    explicit videoplayer(MainWindow *w=nullptr);
    ~videoplayer();
    void PlayVideo(void);
    void StopVideo(void);
    bool playerActiveFlag;

signals:
     // void currentIndexChanged(int);
private:

     MainWindow *pMainW;
     int listIdx;
     int maxListIdx;
     QFileInfoList *list;
     QMediaPlayer *player;
     QVideoWidget *videowidget;
     //QMediaPlaylist *playlist;
private slots:
     void PlayerStateChanged(QMediaPlayer::State);
     void PlaylistIndexChanged(int position);
     void displayErrorMessage(void);
     void SendUnlockW1(void);
};

#endif // VIDEOPLAYER_H
