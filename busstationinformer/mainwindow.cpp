#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDate>
#include <QTime>
#include <QProcess>
#include <QThread>
//#include<QNetworkAccessManager>
//#include <QtNetwork>
#include <QDir>
#include "http.h"
#include "wiring.h"
#include "modem.h"
#include "unistd.h"

int InfoMsg::count=0;

InfoMsg::InfoMsg(MainWindow *parent, QString &str,enum TYPE_NOTIFY type)
{
    QFont font;
    QRect rect;

   MsgWindow.setParent(parent);
    rect=parent->centralWidget()->geometry();
    QPoint centre=rect.center();
    MsgWindow.setGeometry(QRect(centre.x()-300,centre.y()-100+count*250,600,200));
    count++;

    if(type==ERR_MSG)
        MsgWindow.setStyleSheet("background-color: rgb(230, 170, 170)");
    else if(type==WARN_MSG)
        MsgWindow.setStyleSheet("background-color: rgb(220, 220, 120)");
    else if(type==NOTIFY_MSG)
        MsgWindow.setStyleSheet("background-color: rgb(180, 240, 180)");

    MsgWindow.show();

    labelInfoMsg.setParent(&MsgWindow);
    labelInfoMsg.setGeometry(10,2,550,45);
    font.setPointSize(24);
    labelInfoMsg.setFont(font);

    if(type==ERR_MSG)
        labelInfoMsg.setText("Ошибка!!!");
    else if(type==WARN_MSG)
        labelInfoMsg.setText("Внимание!");
    //else if(type==NOTIFY_MSG)
    //    labelInfoMsg.setText("Уведомление");
    labelInfoMsg.setAlignment(Qt::AlignCenter);
    labelInfoMsg.show();

    inframe.setParent(&MsgWindow);
    inframe.setGeometry(5,50,590,145);
    inframe.setStyleSheet("background-color: rgb(205, 220, 205)");
    inframe.show();

    labelMsg.setParent(&inframe);
    labelMsg.setGeometry(5,10,580,125);
    font.setPointSize(20);
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
/*
 *
 */
int MainWindow::CalcGsmSignalPower(int rssi)
{
    if(rssi >=26)   return 5;
    else if(rssi <=25 && rssi >=20) return 4;
    else if(rssi <=19 && rssi >=14) return 3;
    else if(rssi <=13 && rssi >=8)  return 2;
    else
      return 1;
}
/*
 *
 */
void MainWindow::extSoundPlayerFillBuffer(QString &str)
{
   int size=currRoutList->size();
   for(int i=0;i<size;i++)
   {
      str.append(currRoutList->at(i).routNumber+'-');
      if(i==(size-1))
        str.append(currRoutList->at(i).timeLeft+';');
      else
        str.append(currRoutList->at(i).timeLeft+',');
   }
}
/*
 *
 */
void MainWindow::extSoundProcessFinished(int, QProcess::ExitStatus)
{
   //extSoundPlayer->kill();
   extSoundPlayer->deleteLater();
   //w_pins->w1_mutex.unlock();
   extSoundPlayerActive=false;
   videotimer->start(VIDEOPLAYER_TOUT);
}
/*
 *
 */
void MainWindow::soundTimerExpired(void)
{
    //if(w_pins->flag==1)
    //{
        QString str;
        soundtimer->stop();
       // w_pins->w1_mutex.lock();
        extSoundPlayerActive=true;
        extSoundPlayerFillBuffer(str);
        extSoundPlayer=new QProcess(this);
        connect(extSoundPlayer , SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(extSoundProcessFinished(int, QProcess::ExitStatus)));
        QStringList arg(str);
#ifdef Q_OS_WIN
         QString s=(QApplication::applicationDirPath()+"/soundplayer.exe");
#else
         QString s=(QApplication::applicationDirPath()+"/soundplayer");
#endif
        extSoundPlayer->start(s,arg);
   // }
   // else
    //  soundtimer->start(200);

}
/*
 *
 */
int MainWindow::StartSoundPlayer(void)
{
    //if(w_pins->flag==1)
    //{
        QString str;
        //w_pins->w1_mutex.lock();
        extSoundPlayerActive=true;
        extSoundPlayerFillBuffer(str);
        extSoundPlayer=new QProcess(this);
        connect(extSoundPlayer , SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(extSoundProcessFinished(int, QProcess::ExitStatus)));
        QStringList arg(str);
#ifdef Q_OS_WIN
         QString s=(QApplication::applicationDirPath()+"/soundplayer.exe");
#else
         QString s=(QApplication::applicationDirPath()+"/soundplayer");
#endif
        extSoundPlayer->start(s,arg);
    //}
    //else
     // soundtimer->start(200);
    return 0;
}
/*
 *
 */
int MainWindow::StopSoundPlayer(void)
{
    if(extSoundPlayerActive && extSoundPlayer)
    {
        extSoundPlayer->kill();
        delete extSoundPlayer; //extVideoPlayer->deleteLater();
       //w_pins->w1_mutex.unlock();
       extSoundPlayerActive=false;
    }
    return 0;
}
/*
 *
 */
//
void MainWindow::customEvent(QEvent *event)
{
    if(event->type()==QEvent::User)
    {
      switch(reinterpret_cast<RedrawMainWindow*>(event)->GetingMsg())
      {
        case RedrawMainWindow::UPDATE_ROUT_LIST:
        {
          int *pIdxBuff=reinterpret_cast<int*>(reinterpret_cast<RedrawMainWindow*>(event)->GetingData());
          if(pIdxBuff)
            buffIdx=*pIdxBuff;
        }
        break;
        case RedrawMainWindow::NO_ACTIVE_ROUTS:
        {
           ERRORS err;
           err.allbits=*reinterpret_cast<uint32_t*>(reinterpret_cast<RedrawMainWindow*>(event)->GetingData());
           if(err.noactiveRouts)
           {
               QString s="Нет активных маршрутов!";
               NoActiveRoutsNotify=new InfoMsg(this,s,InfoMsg::NOTIFY_MSG);
           }
           else
           {
               if(NoActiveRoutsNotify)
                 delete NoActiveRoutsNotify;
               NoActiveRoutsNotify=nullptr;
           }
         }
        break;
        case RedrawMainWindow::UPDATE_W1_TEMPR:
        {
          float *pTempr=reinterpret_cast<float*>(reinterpret_cast<RedrawMainWindow*>(event)->GetingData());
          if(pTempr)
            onewiretempr=*pTempr;
        }
        break;
        case RedrawMainWindow::UPDATE_WEATHER_TEMPR:
        {
          int *pweatherTempr=reinterpret_cast<int*>(reinterpret_cast<RedrawMainWindow*>(event)->GetingData());
          if(pweatherTempr)
            weatherTempr=*pweatherTempr;
        }
        break;
        case RedrawMainWindow::SOUND_BUTTON_PRESS:
        {
          StopVideoPlayer();
          videotimer->stop();
          if(!extSoundPlayerActive)
          {
            if(currRoutList && (currRoutList->isEmpty()==false))
                StartSoundPlayer();
          }
        }
        break;
        case RedrawMainWindow::CALL112_BUTTON_PRESS:
        {
          if(!gsmmodule->callState/* (Call112Notify==NULL)*/ && gsmmodule)
          {
             gsmmodule->callRequest=true;
             gsmmodule->hangUp=false;
             StopVideoPlayer();
             videotimer->stop();
             QString s="Вызов службы спасения 112!";
             Call112Notify=new InfoMsg(this,s,InfoMsg::NOTIFY_MSG);
          }
          else  if(gsmmodule->callState/*Call112Notify*/ && gsmmodule)
          {
              gsmmodule->callRequest=false;
              gsmmodule->hangUp=true;
              Call112Notify->deleteLater();
              videotimer->start(VIDEOPLAYER_TOUT);
             //Call112Notify=NULL;
          }
          /*
          bool callstate=*(bool*)((RedrawMainWindow*)event)->GetingData();
          if(callstate==true)
          {
              if((Call112Notify==NULL) && gsmmodule)
              {
                QString s="Вызов службы спасения 112!";
                Call112Notify=new InfoMsg(this,s,InfoMsg::NOTIFY_MSG);
                gsmmodule->callRequest=true;
                gsmmodule->hangUp=false;
              }
          }
          else
          {
              if(Call112Notify && gsmmodule)
              {
                delete Call112Notify;
                Call112Notify=NULL;
                gsmmodule->callRequest=false;
                gsmmodule->hangUp=true;
              }
          }
          */
        }
        break;
        case RedrawMainWindow::FILECONFIG_ERR_MESSAGE:
        {
            ERRORS  err;
            err.allbits=*reinterpret_cast<uint32_t*>(reinterpret_cast<RedrawMainWindow*>(event)->GetingData());
            if(err.fileconfigErr)
            {
                QString s="Файл конфигурации не найден или содержит неверные данные!";
                FileConfigError=new InfoMsg(this,s,InfoMsg::ERR_MSG);
            }
        }
        break;
        case RedrawMainWindow::CONNECT_ERR_MESSAGE:
        {
          ERRORS  err;
          err.allbits=*reinterpret_cast<uint32_t*>(reinterpret_cast<RedrawMainWindow*>(event)->GetingData());
          if(err.connectionErr)
          {
              QString s="Нет соединения с сервером!Данные по маршрутам не актуальны!";
              NoConnectWarning=new InfoMsg(this,s,InfoMsg::WARN_MSG);
          }
          else
          {
              if(NoConnectWarning)
                delete NoConnectWarning;
              NoConnectWarning=nullptr;
          }
        }
        break;
        case RedrawMainWindow::COMPORT_ERR_MESSAGE:
        {
            ERRORS  err;
            err.allbits=*reinterpret_cast<uint32_t*>(reinterpret_cast<RedrawMainWindow*>(event)->GetingData());
            if(err.comportOpenErr)
            {
                if(!COMPortOpenError)
                {
                    QString s="Ошибка открытия COM порта!Порт занят или не настроен!\nВызов 112 недоступен!";
                    COMPortOpenError=new InfoMsg(this,s,InfoMsg::ERR_MSG);
                }
                break;
            }
            if(err.comportConnErr)
            {
                QString s="Обрыв линии связи c GSM модемом!";
                COMPortConnWarning=new InfoMsg(this,s,InfoMsg::WARN_MSG);
            }
            else
            {
                if(COMPortConnWarning)
                  delete COMPortConnWarning;
                COMPortConnWarning=nullptr;
            }
        }
        break;
        case RedrawMainWindow::GSM_PARAM:
        {
            GSM_PARAM  *param;
            param=reinterpret_cast<GSM_PARAM*>(reinterpret_cast<RedrawMainWindow*>(event)->GetingData());
            if(param->netReg==-1 || param->rssi==99)
            {
               ui->gsmSignalgraphicsView->setStyleSheet(QString::fromUtf8("border-image: url(:/gsm_bitmaps/gsm_noreg.png);"));
            }
            else
            {
               int temp=CalcGsmSignalPower(param->rssi);
               switch(temp)
               {
                  case 1:
                      ui->gsmSignalgraphicsView->setStyleSheet(QString::fromUtf8("border-image: url(:/gsm_bitmaps/gsm_1.png);"));
                  break;
                  case 2:
                      ui->gsmSignalgraphicsView->setStyleSheet(QString::fromUtf8("border-image: url(:/gsm_bitmaps/gsm_2.png);"));
                  break;
                  case 3:
                      ui->gsmSignalgraphicsView->setStyleSheet(QString::fromUtf8("border-image: url(:/gsm_bitmaps/gsm_3.png);"));
                  break;
                  case 4:
                      ui->gsmSignalgraphicsView->setStyleSheet(QString::fromUtf8("border-image: url(:/gsm_bitmaps/gsm_4.png);"));
                  break;
                  case 5:
                      ui->gsmSignalgraphicsView->setStyleSheet(QString::fromUtf8("border-image: url(:/gsm_bitmaps/gsm_5.png);"));
                  break;
                }
            }
        }
        break;
        case RedrawMainWindow::W1_MUTEX_UNLOCK:
        {
          w_pins->w1_mutex.unlock();
        }
        break;
        default:
        break;
      }
    }
    QWidget::customEvent(event);
}
/*
 *
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->WeathergraphicsView->setAlignment(Qt::AlignHCenter);
    ui->WeathergraphicsView->hide();
    weatherTempr=-200;
    ui->labelTempr->setAlignment(Qt::AlignHCenter);

    extVideoPlayerActive=false;
    extSoundPlayerActive=false;
    extVideoPlayer=nullptr;
    extSoundPlayer=nullptr;

    gsmThread=new QThread();
    gsmmodule=new BGS2_E(this);
    connect(gsmThread, &QThread::started, gsmmodule, &BGS2_E::gsmProcess);
    // По завершению выходим из потока, и удаляем рабочий класс
    connect(gsmmodule, SIGNAL(finishedPort()), gsmThread, SLOT(quit()));
    connect(gsmThread, SIGNAL(finished()), gsmmodule, SLOT(deleteLater()));

    // Удаляем поток, после выполнения операции
    connect(gsmmodule, SIGNAL(finishedPort()), gsmThread, SLOT(deleteLater()));
    gsmmodule->moveToThread(gsmThread);
    gsmThread->start();

    FileConfigError=nullptr;
    COMPortOpenError=nullptr;

    NoConnectWarning=nullptr;
    COMPortConnWarning=nullptr;

    NoActiveRoutsNotify=nullptr;
    Call112Notify=nullptr;

    routStringEmptyFlag[0]=true;
    routStringEmptyFlag[1]=true;
    routStringEmptyFlag[2]=true;
    routStringEmptyFlag[3]=true;
    w_pins=new WiringPins(this);
    onewiretempr=-1000;

    buffIdx=-1;
    currRoutList=nullptr;
    http=new httpProcess(this);
    routlistFront=new QVector<ROUT_ITEM>();
    routlistBack=new QVector<ROUT_ITEM>();

    secTimer.setInterval(1000);
    connect(&secTimer,SIGNAL(timeout()),SLOT(secTimerExpired()));
    secTimer.start();

    routViewTimer.setInterval(5000);
    connect(&routViewTimer,SIGNAL(timeout()),SLOT(routViewTimerExpired()));
    routViewTimer.start();

    QDir dir(QApplication::applicationDirPath()+"/VIDEO");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    list=new QFileInfoList();
    list->clear();
    list->append(dir.entryInfoList());
    maxvideoListIdx=list->size();
    videolistIdx=0;
    videotimer=new QTimer();
    //videotimer->setInterval(VIDEOPLAYER_PERIOD);
    connect(videotimer,SIGNAL(timeout()),SLOT(videoTimerExpired()));
    videotimer->start(VIDEOPLAYER_TOUT);

    soundtimer=new QTimer();
    connect(soundtimer,SIGNAL(timeout()),SLOT(soundTimerExpired()));

}
/*
 *
 */
MainWindow::~MainWindow()
{
    if(FileConfigError)
        delete FileConfigError;
    if(COMPortOpenError)
        delete COMPortOpenError;

    if(NoConnectWarning)
        delete NoConnectWarning;
    if(COMPortConnWarning)
        delete COMPortConnWarning;

    if(NoActiveRoutsNotify)
        delete NoActiveRoutsNotify;

    if(gsmmodule)
    {
        if(gsmmodule->callState)
        {
            Call112Notify->close();
            //delete  Call112Notify;
        }
        emit gsmmodule->finishedPort(); //delete gsmmodule;
    }

    delete w_pins;
    if(extSoundPlayerActive)
         StopSoundPlayer();
    if(extVideoPlayerActive)
        StopVideoPlayer();

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
#ifdef Q_OS_WIN
    else if(event->key()==Qt::Key_D)
    {
        if(!gsmmodule->callState/* (Call112Notify==NULL)*/ && gsmmodule)
        {
           gsmmodule->callRequest=true;
           gsmmodule->hangUp=false;
           StopVideoPlayer();
           videotimer->start(30000);
           QString s="Вызов службы спасения 112!";
           Call112Notify=new InfoMsg(this,s,InfoMsg::NOTIFY_MSG);
        }
        else  if(gsmmodule->callState/*Call112Notify*/ && gsmmodule)
        {
            gsmmodule->callRequest=false;
            gsmmodule->hangUp=true;
            Call112Notify->deleteLater();
           //Call112Notify=NULL;
        }
    }
    else if(event->key()==Qt::Key_H)
    {
        StopVideoPlayer();
        videotimer->start(30000);
        if(!extSoundPlayerActive)
        {
          if(currRoutList && (currRoutList->isEmpty()==false))
              StartSoundPlayer();
        }
    }
#endif
}
/*
 *
 */
#if 0
void MainWindow::soundPlayerStateChanged(QMediaPlayer::State state)
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
                soundPlayer->setMedia(QMediaContent(), buffer);
               // player->setMedia(QUrl::fromLocalFile(numbertrack));
                soundPlayer->play();
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
            soundPlayer->setMedia(QMediaContent(), buffer);
            //player->setMedia(QUrl::fromLocalFile(numbertrack));
            soundPlayer->play();
            soundTrackCount++;
        }
    }

}
#endif
/*
 *
 */
void MainWindow::secTimerExpired(void)
{
    static QTime t;
    static int temprPrev=-200;

    QTime time=QTime::currentTime();
    if((time.hour()==6) &&  (time.minute()==2) && (time.second()==15 || time.second()==16 || time.second()==17))
    {
        system("sudo reboot");
        close();
        return;
    }
    if(extVideoPlayerActive)
        return;
    if(t.minute()!=time.minute())
    {
        QDate date=QDate::currentDate();
        QString str=time.toString("hh:mm");;
        ui->labelTime->setText(str);
        str=date.toString("dd.MM.yyyy");
        ui->labelDate->setText(str);
        t=time;
    }

    if(temprPrev!=weatherTempr)
    {
        temprPrev=weatherTempr;
        if(weatherTempr > -50 && weatherTempr <100)
        {
            ui->WeathergraphicsView->show();
            //QString strtemp=QString::number(onewiretempr,'f',1);
            QString strtemp=QString::number(weatherTempr);
            strtemp.append(" °C");
            ui->labelTempr->setText(strtemp);
        }
        else
        {
            ui->WeathergraphicsView->hide();
            ui->labelTempr->setText("----");
        }

    }
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

   if(extVideoPlayerActive)
       return;
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
int MainWindow::StartVideoPlayer(void)
{
    extVideoPlayerActive=true;
    extVideoPlayer=new QProcess(this);
    connect(extVideoPlayer , SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(extVideoProcessFinished(int, QProcess::ExitStatus)));
    QString path("VIDEO/"+list->at(videolistIdx).fileName());
    QStringList arg(path);
#ifdef Q_OS_WIN
    extVideoPlayer->start(QApplication::applicationDirPath()+"/videoplayer.exe",arg);
#else
    extVideoPlayer->start(QApplication::applicationDirPath()+"/videoplayer",arg);
#endif
    return 0;
}
/*
 *
 */
void MainWindow::extVideoProcessFinished(int, QProcess::ExitStatus)
{
   if(extVideoPlayerActive)
   {
      // extVideoPlayer->kill();
       extVideoPlayer->deleteLater();
       //w_pins->w1_mutex.unlock();
       extVideoPlayerActive=false;
       videotimer->start(VIDEOPLAYER_TOUT);
   }
   if(++videolistIdx >=maxvideoListIdx)   videolistIdx=0;
}
/*
 *
 */
int MainWindow::StopVideoPlayer(void)
{
    if(extVideoPlayerActive && extVideoPlayer)
    {
        //extVideoPlayer->kill();
        extVideoPlayer->terminate();
       /*delete extVideoPlayer;*/ extVideoPlayer->deleteLater();
       // w_pins->w1_mutex.unlock();
        extVideoPlayerActive=false;
    }
    return 0;
}
/*
 *
 */
void MainWindow::videoTimerExpired(void)
{
    if(!extSoundPlayerActive && gsmmodule && !gsmmodule->callState/*!Call112Notify*/)
    {
       // if(w_pins->flag==1/* || w_pins->flag==2*/)
       // {
         //  w_pins->w1_mutex.lock();
           StartVideoPlayer();
           //videotimer->start(VIDEOPLAYER_TOUT);
       // }
       // else
       // {
       //     videotimer->start(200);
       // }
    }
}
//
//
//
