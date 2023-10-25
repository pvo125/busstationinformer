#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>

MainWindow::MainWindow(char *mediaPath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   player = new QMediaPlayer(this);
   vwidget = new QVideoWidget(this);
   //vwidget->setGeometry(0,0,1280,720);
   player->setVideoOutput(vwidget);
   setWindowFlags(Qt::FramelessWindowHint);
   setCentralWidget(vwidget);


   QString path=QApplication::applicationDirPath();
   path.append("/");
   path.append(mediaPath);
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
    delete ui;
}
/*
 *
 */
void MainWindow::PlayerStateChanged(QMediaPlayer::State state)
{
    if(state==QMediaPlayer::StoppedState)
    {
      this->close();
    }
}
/*
 *
 */
//
//
//
