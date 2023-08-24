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
    soundTrackCount=-1;
    timetrackFlag=SND_TRACK;
    buffIdx=-1;
    currRoutList=NULL;
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
      if(soundTrackCount==-1)
      {
        timetrackFlag=SND_TRACK;
        soundTrackCount=0;
        if(currRoutList->isEmpty()==false)
        {
            QString numbertrack("TRACK/");
            numbertrack.append(currRoutList->at(soundTrackCount).routNumber);
            numbertrack.append(".wav");
            player->setMedia(QUrl::fromLocalFile(numbertrack));
            player->setVolume(30);
            player->play();
        }
      }
    }
}
/*
 *
 */
void MainWindow::playerStateChanged(QMediaPlayer::State state)
{
    if(state==QMediaPlayer::StoppedState)
    {
        if(timetrackFlag==SND_TRACK)
        {
            timetrackFlag=SND_TIME;
            QString numbertrack("TIME/");
            numbertrack.append(currRoutList->at(soundTrackCount).timeLeft);
            numbertrack.append("min.wav");
            player->setMedia(QUrl::fromLocalFile(numbertrack));
            player->setVolume(10);
            player->play();
        }
        else if(timetrackFlag==SND_TIME)
        {
             timetrackFlag=SND_TRACK;
            if(soundTrackCount < currRoutList->size())
            {
                QString numbertrack("TRACK/");
                numbertrack.append(currRoutList->at(soundTrackCount).routNumber);
                numbertrack.append(".wav");
                player->setMedia(QUrl::fromLocalFile(numbertrack));
                player->setVolume(30);
                player->play();
                soundTrackCount++;
            }
            else
              soundTrackCount=-1;
        }
    }

}
/*
 *
 */
void MainWindow::secTimerExpired(void)
{
    QTime time=QTime::currentTime();
    QDate date=QDate::currentDate();
    QString str=time.toString("hh:mm");;
    ui->labelTime->setText(str);
    str=date.toString("dd MMMM yyyy");
    ui->labelDate->setText(str);
    ui->frame_9->width();
    ui->label_7->setText(QString::number(ui->frame_9->width()));
}
/*
 *
 */
 void MainWindow::ClearStringsRouts(int startStrIdx)
 {
     switch(startStrIdx){
        case 2:
            ui->string2_routnumber->clear();
            ui->string2_routname->clear();
            ui->string2_lefttime->clear();
            ui->string3_routnumber->clear();
            ui->string3_routname->clear();
            ui->string3_lefttime->clear();
            ui->string4_routnumber->clear();
            ui->string4_routname->clear();
            ui->string4_lefttime->clear();
        break;
        case 3:
            ui->string3_routnumber->clear();
            ui->string3_routname->clear();
            ui->string3_lefttime->clear();
            ui->string4_routnumber->clear();
            ui->string4_routname->clear();
            ui->string4_lefttime->clear();
        break;
        case 4:
            ui->string4_routnumber->clear();
            ui->string4_routname->clear();
            ui->string4_lefttime->clear();
        break;
     }
 }
/*
 *
 */
void MainWindow::routViewTimerExpired(void)
{
   static int routcount=0;

   if(buffIdx==0 && routcount==0)       currRoutList=routlistFront;
   else if(buffIdx==1 && routcount==0)  currRoutList=routlistBack;

    if(currRoutList)
    {
        ui->string1_routnumber->setText(currRoutList->at(routcount).routNumber);
        ui->string1_routname->setText(currRoutList->at(routcount).routName);
        ui->string1_lefttime->setText(currRoutList->at(routcount).timeLeft);
        if(++routcount >=currRoutList->size())
        {
            routcount=0;
            ClearStringsRouts(2);
            return;
        }
        ui->string2_routnumber->setText(currRoutList->at(routcount).routNumber);
        ui->string2_routname->setText(currRoutList->at(routcount).routName);
        ui->string2_lefttime->setText(currRoutList->at(routcount).timeLeft);
        if(++routcount >=currRoutList->size())
        {
            routcount=0;
            ClearStringsRouts(3);
            return;
        }
        ui->string3_routnumber->setText(currRoutList->at(routcount).routNumber);
        ui->string3_routname->setText(currRoutList->at(routcount).routName);
        ui->string3_lefttime->setText(currRoutList->at(routcount).timeLeft);
        if(++routcount >=currRoutList->size())
        {
            routcount=0;
            ClearStringsRouts(4);
            return;
        }
        ui->string4_routnumber->setText(currRoutList->at(routcount).routNumber);
        ui->string4_routname->setText(currRoutList->at(routcount).routName);
        ui->string4_lefttime->setText(currRoutList->at(routcount).timeLeft);
        if(++routcount >=currRoutList->size())
        {
            routcount=0;
            return;
        }
    }
}
/*
 *
 */

