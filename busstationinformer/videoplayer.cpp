#include "videoplayer.h"
#include <QDir>
#include <QApplication>

videoplayer::videoplayer(MainWindow *w)
{
    pMainW=w;
    setParent(w);
    //connect(&player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(PlayerStateChanged(QMediaPlayer::State)));
    //connect(&playlist,SIGNAL(currentIndexChanged(int)), this, SLOT(PlaylistIndexChanged(int)));
    //connect(&player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &videoplayer::displayErrorMessage);
#ifndef Q_OS_WIN
    QDir dir("/home/pi/app/VIDEO");
#else
    QDir dir("VIDEO");
#endif
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    list=new QFileInfoList();
    list->clear();
    list->append(dir.entryInfoList());
    maxListIdx=list->size();
    listIdx=0;
//    playlist=new QMediaPlaylist;
//    for(int i=0;i<maxListIdx;i++)
//    {
//        playlist->addMedia(QUrl::fromLocalFile(list->at(i).absoluteFilePath()));
//    }
//    playlist->setCurrentIndex(0);
    //player.setMedia(QUrl("gst-launch-1.0 filesrc location=/home/pi/app/VIDEO/4.mp4 ! decodebin ! autovideosink"));
    //player.setMedia(QUrl::fromLocalFile("VIDEO/1.mp4"));
}

videoplayer::~videoplayer()
{
    if(playerActiveFlag)
    {
        playerActiveFlag=false;
        player->stop();
        videowidget->deleteLater();//delete videowidget;
        player->deleteLater();
    }
}
//
 void videoplayer::PlaylistIndexChanged(int position)
 {
   if(position/*playlist.currentIndex()*/<0)
   {
        //playlist->setCurrentIndex(0);
   }
   if(playerActiveFlag)
   {
       playerActiveFlag=false;
       player->stop();
       delete videowidget;//videowidget->deleteLater();
       player->deleteLater();
   }
 }
//
void videoplayer::SendUnlockW1(void)
{
    RedrawMainWindow *ev=new RedrawMainWindow((QEvent::Type)(QEvent::User));
    ev->SendingMsg(RedrawMainWindow::W1_MUTEX_UNLOCK);
    ev->SendingData(NULL);
    QApplication::postEvent(parent(),ev);

}
//
void videoplayer::PlayerStateChanged(QMediaPlayer::State state)
{
    if(state==QMediaPlayer::StoppedState)
    {
       if(++listIdx >=maxListIdx)
           listIdx=0;
       if(playerActiveFlag)
       {
           //disconnect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(PlayerStateChanged(QMediaPlayer::State)));
           playerActiveFlag=false;
           player->stop();
           delete videowidget; //videowidget->deleteLater();
           player->deleteLater();
       }
       SendUnlockW1();
    }
}
/*
 *
 */
void videoplayer::displayErrorMessage(void)
{
    QString strErr=player->errorString();
}
/*
 *
 */
void videoplayer::PlayVideo(void)
{
   playerActiveFlag=true;
   player=new QMediaPlayer(pMainW);
   player->setMedia(QUrl::fromLocalFile(list->at(listIdx).absoluteFilePath()));
   //player->setPlaylist(playlist);
   connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(PlayerStateChanged(QMediaPlayer::State)));
   //connect(playlist,SIGNAL(currentIndexChanged(int)), this, SLOT(PlaylistIndexChanged(int)));
   //connect(player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &videoplayer::displayErrorMessage);


   videowidget=new QVideoWidget(pMainW);
   videowidget->setWindowFlag(Qt::FramelessWindowHint);
   videowidget->setGeometry(0,0,1920,1080);
   videowidget->setFullScreen(1);
   player->setVideoOutput(videowidget);
   videowidget->show();
   player->play();
}
/*
 *
 */
void videoplayer::StopVideo(void)
{
   if(playerActiveFlag)
   {
       disconnect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(PlayerStateChanged(QMediaPlayer::State)));
       playerActiveFlag=false;
       player->stop();
       delete videowidget;//videowidget->deleteLater();
       player->deleteLater();
   }
}
//
//
//
//
//
