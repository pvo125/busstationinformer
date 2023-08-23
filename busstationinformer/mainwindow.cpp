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

    secTimer.setInterval(500);
    connect(&secTimer,SIGNAL(timeout()),SLOT(secTimerExpired()));
    secTimer.start();

    routViewTimer.setInterval(5000);
    connect(&routViewTimer,SIGNAL(timeout()),SLOT(routViewTimerExpired()));
    routViewTimer.start();

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
/*
 *
 */
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
/*
 *
 */
void MainWindow::secTimerExpired(void)
{
    QTime time=QTime::currentTime();
    QDate date=QDate::currentDate();

    QString str=time.toString("hh:mm:ss");
    ui->labelTime->setText(str);
    str=date.toString("dd MMMM yyyy");
    ui->labelDate->setText(str);


}
void MainWindow::routViewTimerExpired(void)
{
    ui->string1_routnumber->setText(routlistFront->at(0).routNumber);
    ui->string1_routname->setText(routlistFront->at(0).routName);
    ui->string1_lefttime->setText(routlistFront->at(0).timeLeft);

    ui->string2_routnumber->setText(routlistFront->at(1).routNumber);
    ui->string2_routname->setText(routlistFront->at(1).routName);
    ui->string2_lefttime->setText(routlistFront->at(1).timeLeft);

    ui->string3_routnumber->setText(routlistFront->at(2).routNumber);
    ui->string3_routname->setText(routlistFront->at(2).routName);
    ui->string3_lefttime->setText(routlistFront->at(2).timeLeft);

    ui->string4_routnumber->setText(routlistFront->at(3).routNumber);
    ui->string4_routname->setText(routlistFront->at(3).routName);
    ui->string4_lefttime->setText(routlistFront->at(3).timeLeft);

}
/*
 *
 */

