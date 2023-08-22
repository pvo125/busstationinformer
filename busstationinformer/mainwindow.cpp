#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDate>
#include <QTime>
//#include<QNetworkAccessManager>
//#include <QtNetwork>
#include "http.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    http=new httpProcess(this);
    routlistFront=new QVector<ROUT_ITEM>();
    routlistBack=new QVector<ROUT_ITEM>();

    redrawTimer.setInterval(500);
    connect(&redrawTimer,SIGNAL(timeout()),SLOT(redrawTimerExpired()));
    redrawTimer.start();
    player = new QMediaPlayer;
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(playerStateChanged(QMediaPlayer::State)));

}
/*
 *
 */
MainWindow::~MainWindow()
{
    player->stop();
    delete player;

    routlistFront->clear();
    delete routlistFront;

    routlistBack->clear();
    delete routlistBack;

    delete http;
    delete ui;
}
/*
 *
 */
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Escape)
    {
        this->close();
    }
    else if(event->key()==Qt::Key_Return)
    {
      QString numbertrack("TRACK/");
      numbertrack.append(numbertrack.number(count));
      numbertrack.append(".wav");
      player->setMedia(QUrl::fromLocalFile(numbertrack));
      player->setVolume(10);
      player->play();
    }
}



void MainWindow::playerStateChanged(QMediaPlayer::State state)
{
    if(state==QMediaPlayer::StoppedState)
    {
        QString numbertrack("TIME/");
        numbertrack.append(numbertrack.number(12));
        numbertrack.append("min.wav");
        player->setMedia(QUrl::fromLocalFile(numbertrack));
        player->setVolume(10);
        player->play();
    }

}

void MainWindow::redrawTimerExpired(void)
{
    QTime time=QTime::currentTime();
    QDate date=QDate::currentDate();

    QString str=time.toString("hh:mm:ss");
    ui->labelTime->setText(str);
    str=date.toString("dd MMMM yyyy");
    ui->labelDate->setText(str);


}

