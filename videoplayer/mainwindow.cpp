#include "mainwindow.h"
#include <QApplication>

MainWindow::MainWindow(char *mediafile, QWidget *parent) :
    QWidget(parent)
{
   player = new QMediaPlayer(this);
   vwidget = new QVideoWidget(this);
   vwidget->setGeometry(0,0,1920,1080);
   player->setVideoOutput(vwidget);
   setWindowFlags(Qt::FramelessWindowHint);
   QString path=QApplication::applicationDirPath();
   path.append("/");
   path.append(mediafile);
   player->setMedia(QUrl::fromLocalFile(path));
   connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(PlayerStateChanged(QMediaPlayer::State)));
   player->play();
}
/*
 *
 */
MainWindow::~MainWindow()
{
    delete vwidget;
    delete player;
}
/*
 *
 */
void MainWindow::PlayerStateChanged(QMediaPlayer::State state)
{
    if(state==QMediaPlayer::StoppedState)
    {
      delete this;//this->close();
    }
}
/*
 *
 */
//
//
//
