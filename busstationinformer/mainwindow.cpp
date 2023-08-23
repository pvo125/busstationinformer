#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDate>
#include <QTime>
//#include<QNetworkAccessManager>
//#include <QtNetwork>
#include "http.h"

void MainWindow::customEvent(QEvent *event)
{
    if(event->type()==QEvent::User)
    {
      switch(((UpdateRoutList*)event)->GetMsg())
      {

        case UpdateRoutList::UPDATE_ROUT_LIST:
        {
          buffIdx=*(int*)((UpdateRoutList*)event)->GetData();
        }
        break;
        default:
          break;
      }
    }
    QWidget::customEvent(event);
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    buffIdx=-1;
    http=new httpProcess(this);
    routlistFront=new QVector<ROUT_ITEM>();
    routlistBack=new QVector<ROUT_ITEM>();

    secTimer.setInterval(500);
    connect(&secTimer,SIGNAL(timeout()),SLOT(secTimerExpired()));
    secTimer.start();

    routViewTimer.setInterval(3000);
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
    ui->label_7->setText(QString::number(buffIdx));


}
void MainWindow::routViewTimerExpired(void)
{
   static int routcount=0;
   static bool startroutFlag=false;

    if(buffIdx==0 || startroutFlag==false)
    {
        ui->string1_routnumber->setText(routlistFront->at(routcount).routNumber);
        ui->string1_routname->setText(routlistFront->at(routcount).routName);
        ui->string1_lefttime->setText(routlistFront->at(routcount).timeLeft);
        if(++routcount >=routlistFront->size())
        {
            startroutFlag=true;
            routcount=0;
            ui->string2_routnumber->clear();
            ui->string2_routname->clear();
            ui->string2_lefttime->clear();
            ui->string3_routnumber->clear();
            ui->string3_routname->clear();
            ui->string3_lefttime->clear();
            ui->string4_routnumber->clear();
            ui->string4_routname->clear();
            ui->string4_lefttime->clear();
            return;
        }
        ui->string2_routnumber->setText(routlistFront->at(routcount).routNumber);
        ui->string2_routname->setText(routlistFront->at(routcount).routName);
        ui->string2_lefttime->setText(routlistFront->at(routcount).timeLeft);
        if(++routcount >=routlistFront->size())
        {
            startroutFlag=true;
            routcount=0;
            ui->string3_routnumber->clear();
            ui->string3_routname->clear();
            ui->string3_lefttime->clear();
            ui->string4_routnumber->clear();
            ui->string4_routname->clear();
            ui->string4_lefttime->clear();
            return;
        }
        ui->string3_routnumber->setText(routlistFront->at(routcount).routNumber);
        ui->string3_routname->setText(routlistFront->at(routcount).routName);
        ui->string3_lefttime->setText(routlistFront->at(routcount).timeLeft);
        if(++routcount >=routlistFront->size())
        {
            startroutFlag=true;
            routcount=0;
            ui->string4_routnumber->clear();
            ui->string4_routname->clear();
            ui->string4_lefttime->clear();
            return;
        }
        ui->string4_routnumber->setText(routlistFront->at(routcount).routNumber);
        ui->string4_routname->setText(routlistFront->at(routcount).routName);
        ui->string4_lefttime->setText(routlistFront->at(routcount).timeLeft);
        if(++routcount >=routlistFront->size())
        {
            startroutFlag=true;
            routcount=0;
            return;
        }

    }
    else if(buffIdx==1 || startroutFlag==true)
    {
        ui->string1_routnumber->setText(routlistBack->at(routcount).routNumber);
        ui->string1_routname->setText(routlistBack->at(routcount).routName);
        ui->string1_lefttime->setText(routlistBack->at(routcount).timeLeft);
        if(++routcount >=routlistBack->size())
        {
            startroutFlag=false;
            routcount=0;
             ui->string2_routnumber->clear();
             ui->string2_routname->clear();
             ui->string2_lefttime->clear();
             ui->string3_routnumber->clear();
             ui->string3_routname->clear();
             ui->string3_lefttime->clear();
             ui->string4_routnumber->clear();
             ui->string4_routname->clear();
             ui->string4_lefttime->clear();
             return;
        }
        ui->string2_routnumber->setText(routlistBack->at(routcount).routNumber);
        ui->string2_routname->setText(routlistBack->at(routcount).routName);
        ui->string2_lefttime->setText(routlistBack->at(routcount).timeLeft);
        if(++routcount >=routlistBack->size())
        {
            startroutFlag=false;
            routcount=0;
            ui->string3_routnumber->clear();
            ui->string3_routname->clear();
            ui->string3_lefttime->clear();
            ui->string4_routnumber->clear();
            ui->string4_routname->clear();
            ui->string4_lefttime->clear();
            return;
        }
        ui->string3_routnumber->setText(routlistBack->at(routcount).routNumber);
        ui->string3_routname->setText(routlistBack->at(routcount).routName);
        ui->string3_lefttime->setText(routlistBack->at(routcount).timeLeft);
        if(++routcount >=routlistBack->size())
        {
            startroutFlag=false;
            routcount=0;
            ui->string4_routnumber->clear();
            ui->string4_routname->clear();
            ui->string4_lefttime->clear();
            return;
        }
        ui->string4_routnumber->setText(routlistBack->at(routcount).routNumber);
        ui->string4_routname->setText(routlistBack->at(routcount).routName);
        ui->string4_lefttime->setText(routlistBack->at(routcount).timeLeft);
        if(++routcount >=routlistBack->size())
        {
            startroutFlag=false;
            routcount=0;
            return;
        }
    }
}
/*
 *
 */

