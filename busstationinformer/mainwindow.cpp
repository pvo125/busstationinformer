#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDate>
#include <QTime>
//#include<QNetworkAccessManager>
//#include <QtNetwork>
#include "http.h"
#include "wiring.h"

int InfoMsg::count=0;

InfoMsg::InfoMsg(MainWindow *parent, QString &str,enum TYPE_NOTIFY type)
{
    QFont font;
    QRect rect;

   MsgWindow.setParent(parent);
    rect=((MainWindow*)parent)->centralWidget()->geometry();
    QPoint centre=rect.center();
    MsgWindow.setGeometry(QRect(centre.x()-300,centre.y()-100+count*250,600,200));
    count++;

    if(type==ERR_MSG)
        MsgWindow.setStyleSheet("background-color: rgb(230, 150, 150)");
    else if(type==WARN_MSG)
        MsgWindow.setStyleSheet("background-color: rgb(220, 220, 100)");
    else if(type==NOTIFY_MSG)
        MsgWindow.setStyleSheet("background-color: rgb(170, 240, 170)");

    MsgWindow.show();

    labelInfoMsg.setParent(&MsgWindow);
    labelInfoMsg.setGeometry(10,2,550,45);
    font.setPointSize(32);
    labelInfoMsg.setFont(font);

    if(type==ERR_MSG)
        labelInfoMsg.setText("Ошибка!!!");
    else if(type==WARN_MSG)
        labelInfoMsg.setText("Внимание!");
    else if(type==NOTIFY_MSG)
        labelInfoMsg.setText("Уведомление");
    labelInfoMsg.setAlignment(Qt::AlignCenter);
    labelInfoMsg.show();

    inframe.setParent(&MsgWindow);
    inframe.setGeometry(5,50,590,145);
    inframe.setStyleSheet("background-color: rgb(205, 220, 205)");
    inframe.show();

    labelMsg.setParent(&inframe);
    labelMsg.setGeometry(5,10,580,125);
    font.setPointSize(28);
    labelMsg.setFont(font);
    labelMsg.setText(str);
    labelMsg.setAlignment(Qt::AlignCenter);
    labelMsg.setWordWrap(true);
    labelMsg.show();

}

InfoMsg::~InfoMsg()
{
    if(InfoMsg::count>0)
      InfoMsg::count--;
}


void MainWindow::customEvent(QEvent *event)
{
    if(event->type()==QEvent::User)
    {
      switch(((RedrawMainWindow*)event)->GetingMsg())
      {

        case RedrawMainWindow::UPDATE_ROUT_LIST:
        {
          int *pIdxBuff=(int*)((RedrawMainWindow*)event)->GetingData();
          if(pIdxBuff)
            buffIdx=*pIdxBuff;
        }
        break;
        case RedrawMainWindow::NO_ACTIVE_ROUTS:
        {
           ERRORS err;
           err.allbits=*(uint32_t*)((RedrawMainWindow*)event)->GetingData();
           if(err.noactiveRouts)
           {
               QString s="Нет активных маршрутов!";
               NoActiveRoutsNotify=new InfoMsg(this,s,InfoMsg::NOTIFY_MSG);
           }
           else
           {
               if(NoActiveRoutsNotify)
                 delete NoActiveRoutsNotify;
               NoActiveRoutsNotify=NULL;
           }
         }
        break;
        case RedrawMainWindow::UPDATE_TEMPR:
        {
          float *pTempr=(float*)((RedrawMainWindow*)event)->GetingData();
          if(pTempr)
            onewiretempr=*pTempr;
        }
        break;
        case RedrawMainWindow::SOUND_BUTTON_PRESS:
        {
          if(soundTrackCount==-1)
          {
            timetrackFlag=SND_TRACK;
            soundTrackCount=0;
            if(currRoutList->isEmpty()==false)
            {
#ifndef Q_OS_WIN
                QString numbertrack("/home/pi/app/TRACK/");
#else
                QString numbertrack("TRACK/");
#endif
                numbertrack.append(currRoutList->at(soundTrackCount).routNumber);
                numbertrack.append(".wav");

                if(buffer)  delete buffer;
                if(arr)     delete arr;
                QFile file(numbertrack);
                file.open(QIODevice::ReadOnly);
                arr = new QByteArray();
                arr->append(file.readAll());
                file.close();

                buffer = new QBuffer(arr);
                buffer->open(QIODevice::ReadWrite);
                player->setMedia(QMediaContent(), buffer);

                //player->setMedia(QUrl::fromLocalFile(numbertrack));
                player->setVolume(50);
                player->play();
            }
          }
        }
        break;
        case RedrawMainWindow::FILECONFIG_ERR_MESSAGE:
        {
            ERRORS  err;
            err.allbits=*(uint32_t*)((RedrawMainWindow*)event)->GetingData();
            if(err.fileconfigErr)
            {
                QString s="Файл конфигурации не найден или содержит не верные данные!";
                FileConfigError=new InfoMsg(this,s,InfoMsg::ERR_MSG);
            }
        }
        break;
        case RedrawMainWindow::CONNECT_ERR_MESSAGE:
        {
          ERRORS  err;
          err.allbits=*(uint32_t*)((RedrawMainWindow*)event)->GetingData();
          if(err.connectionErr)
          {
              QString s="Нет соединения с сервером!Данные по маршрутам не актуальны!";
              NoConnectWarning=new InfoMsg(this,s,InfoMsg::WARN_MSG);
          }
          else
          {
              if(NoConnectWarning)
                delete NoConnectWarning;
              NoConnectWarning=NULL;
          }
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

    NoConnectWarning=NULL;
    FileConfigError=NULL;
    NoActiveRoutsNotify=NULL;

    routStringEmptyFlag[0]=true;
    routStringEmptyFlag[1]=true;
    routStringEmptyFlag[2]=true;
    routStringEmptyFlag[3]=true;

    buffer=NULL;
    arr=NULL;
    w_pins=new WiringPins(this);

    soundTrackCount=-1;
    timetrackFlag=SND_TRACK;
    buffIdx=-1;
    currRoutList=NULL;
    http=new httpProcess(this);
    routlistFront=new QVector<ROUT_ITEM>();
    routlistBack=new QVector<ROUT_ITEM>();

    secTimer.setInterval(1000);
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
    if(FileConfigError)
        delete FileConfigError;
    if(NoActiveRoutsNotify)
        delete NoActiveRoutsNotify;

    delete w_pins;
    player->stop();
    delete player;

    if(buffer)  delete buffer;
    if(arr)     delete arr;

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
}
/*
 *
 */
void MainWindow::playerStateChanged(QMediaPlayer::State state)
{
    if(state==QMediaPlayer::StoppedState)
    {
loop:
        if(timetrackFlag==SND_TIME)
        {
            timetrackFlag=SND_TRACK;
            if(soundTrackCount < currRoutList->size())
            {
#ifndef Q_OS_WIN
                QString numbertrack("/home/pi/app/TRACK/");
#else
                QString numbertrack("TRACK/");
#endif
                numbertrack.append(currRoutList->at(soundTrackCount).routNumber);
                numbertrack.append(".wav");
                if(buffer)  delete buffer;
                if(arr)     delete arr;
                QFile file(numbertrack);
                file.open(QIODevice::ReadOnly);
                arr = new QByteArray();
                arr->append(file.readAll());
                file.close();

                buffer = new QBuffer(arr);
                buffer->open(QIODevice::ReadWrite);
                player->setMedia(QMediaContent(), buffer);
               // player->setMedia(QUrl::fromLocalFile(numbertrack));
                player->play();
            }
            else
            {
                if(buffer)  delete buffer;
                if(arr)     delete arr;
                buffer=NULL;
                arr=NULL;
                soundTrackCount=-1;
            }
        }
        else if(timetrackFlag==SND_TRACK)
        {
            timetrackFlag=SND_TIME;
            if(currRoutList->at(soundTrackCount).timeLeft.toInt() > 30)
            {
              soundTrackCount++;
              goto loop;
            }
#ifndef Q_OS_WIN
            QString numbertrack("/home/pi/app/TIME/");
#else
            QString numbertrack("TIME/");
#endif
            numbertrack.append(currRoutList->at(soundTrackCount).timeLeft);
            numbertrack.append("min.wav");
            if(buffer)  delete buffer;
            if(arr)     delete arr;
            QFile file(numbertrack);
            file.open(QIODevice::ReadOnly);
            arr = new QByteArray();
            arr->append(file.readAll());
            file.close();

            buffer = new QBuffer(arr);
            buffer->open(QIODevice::ReadWrite);
            player->setMedia(QMediaContent(), buffer);
            //player->setMedia(QUrl::fromLocalFile(numbertrack));
            player->play();
            soundTrackCount++;
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
    str=date.toString("dd.MM.yyyy");
    ui->labelDate->setText(str);

    if(w_pins->initCompletedFlag)
    {
       if(onewiretempr > -50 && onewiretempr <100)
       {
           QString strtemp=QString::number(onewiretempr,'f',0);
           strtemp.append("°C");
           ui->labelTempr->setText(strtemp);
       }
        else
          ui->labelTempr->setText("----");
    }
    else
        ui->labelTempr->setText("----");
    ui->labelTempr->setText(QString::number(buffIdx));
}
/*
 *
 */
 void MainWindow::ClearStringsRouts(int startStrIdx)
 {
     switch(startStrIdx){
        case 1:
         if(routStringEmptyFlag[startStrIdx-1]==false)
            {
                ui->string1_routnumber->clear();
                ui->string1_routname->clear();
                ui->string1_lefttime->clear();
                ui->string2_routnumber->clear();
                ui->string2_routname->clear();
                ui->string2_lefttime->clear();
                ui->string3_routnumber->clear();
                ui->string3_routname->clear();
                ui->string3_lefttime->clear();
                ui->string4_routnumber->clear();
                ui->string4_routname->clear();
                ui->string4_lefttime->clear();
                routStringEmptyFlag[startStrIdx-1]=true;
            }
        break;
        case 2:
            if(routStringEmptyFlag[startStrIdx-1]==false)
            {
                ui->string2_routnumber->clear();
                ui->string2_routname->clear();
                ui->string2_lefttime->clear();
                ui->string3_routnumber->clear();
                ui->string3_routname->clear();
                ui->string3_lefttime->clear();
                ui->string4_routnumber->clear();
                ui->string4_routname->clear();
                ui->string4_lefttime->clear();
                routStringEmptyFlag[startStrIdx-1]=true;
            }
        break;
        case 3:
            if(routStringEmptyFlag[startStrIdx-1]==false)
            {
                ui->string3_routnumber->clear();
                ui->string3_routname->clear();
                ui->string3_lefttime->clear();
                ui->string4_routnumber->clear();
                ui->string4_routname->clear();
                ui->string4_lefttime->clear();
                routStringEmptyFlag[startStrIdx-1]=true;
            }
        break;
        case 4:
            if(routStringEmptyFlag[startStrIdx-1]==false)
            {
                ui->string4_routnumber->clear();
                ui->string4_routname->clear();
                ui->string4_lefttime->clear();
                routStringEmptyFlag[startStrIdx-1]=true;
            }
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
        if(routcount ==currRoutList->size())
        {
            routcount=0;
            ClearStringsRouts(1);
            return;
        }
        ui->string1_routnumber->setText(currRoutList->at(routcount).routNumber);
        ui->string1_routname->setText(currRoutList->at(routcount).routName);
        ui->string1_lefttime->setText(currRoutList->at(routcount).timeLeft);
        routStringEmptyFlag[1-1]=false;

        if(++routcount ==currRoutList->size())
        {
            routcount=0;
            ClearStringsRouts(2);
            return;
        }
        ui->string2_routnumber->setText(currRoutList->at(routcount).routNumber);
        ui->string2_routname->setText(currRoutList->at(routcount).routName);
        ui->string2_lefttime->setText(currRoutList->at(routcount).timeLeft);
        routStringEmptyFlag[2-1]=false;

        if(++routcount ==currRoutList->size())
        {
            routcount=0;
            ClearStringsRouts(3);
            return;
        }
        ui->string3_routnumber->setText(currRoutList->at(routcount).routNumber);
        ui->string3_routname->setText(currRoutList->at(routcount).routName);
        ui->string3_lefttime->setText(currRoutList->at(routcount).timeLeft);
        routStringEmptyFlag[3-1]=false;

        if(++routcount ==currRoutList->size())
        {
            routcount=0;
            ClearStringsRouts(4);
            return;
        }
        ui->string4_routnumber->setText(currRoutList->at(routcount).routNumber);
        ui->string4_routname->setText(currRoutList->at(routcount).routName);
        ui->string4_lefttime->setText(currRoutList->at(routcount).timeLeft);
        routStringEmptyFlag[4-1]=false;

        if(++routcount ==currRoutList->size())
        {
            routcount=0;
            return;
        }
    }
}
/*
 *
 */

