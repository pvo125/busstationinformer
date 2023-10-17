#include "videoplayer.h"
#include <QDir>

videoplayer::videoplayer(MainWindow *w)
{
    setParent(w);
    connect(&player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(PlayerStateChanged(QMediaPlayer::State)));
    connect(&playlist,SIGNAL(currentIndexChanged(int)), this, SLOT(PlaylistIndexChanged(int)));
    connect(&player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &videoplayer::displayErrorMessage);

    QDir dir("VIDEO");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList list=dir.entryInfoList();

    int maxListIdx=list.size();
    for(int i=0;i<maxListIdx;i++)
    {
        playlist.addMedia(QUrl(list.at(i).absoluteFilePath()));
    }

    player.setPlaylist(&playlist);
    //player.setMedia(QUrl::fromLocalFile("VIDEO/1.mp4"));
    videowidget.setParent(w);
    player.setVideoOutput(&videowidget);
    videowidget.setFullScreen(1);
    videowidget.hide();
   // player.play();
}

videoplayer::~videoplayer() {}
//
 void videoplayer::PlaylistIndexChanged(int position)
 {
   if(position/*playlist.currentIndex()*/<0)
   {
        playlist.setCurrentIndex(0);
   }
   //player.pause();
   player.stop();
   videowidget.hide();
 }
//
void videoplayer::PlayerStateChanged(QMediaPlayer::State state)
{
    if(state==QMediaPlayer::StoppedState)
    {
        //player.stop();
       // videowidget.hide();
    }
}
/*
 *
 */
void videoplayer::displayErrorMessage(void)
{
    QString strErr=player.errorString();
}
/*
 *
 */
void videoplayer::PlayVideo(void)
{
  videowidget.show();
  player.play();
}
/*
 *
 */
void videoplayer::StopVideo(void)
{
  player.stop();
  videowidget.hide();
}
//
//
//
//
//
