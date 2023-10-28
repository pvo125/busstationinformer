#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>

MainWindow::MainWindow(const char *param_routs, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
#ifdef Q_OS_WIN
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
#else
     QTextCodec *codec = QTextCodec::codecForName("UTF-8");
#endif
    ui->setupUi(this);

    currRoutList=new QVector<ROUT_ITEM>();
    QByteArray tempArr;

    for(int i=0;;i++)
    {
       tempArr.append(param_routs[i]);
       if(param_routs[i]==';')
           break;
    }
    int startIdx=0;
    int endIdx;
    while(1)
    {
        QByteArray temp;
        ROUT_ITEM item;
        endIdx=tempArr.indexOf('-',startIdx);
        temp=tempArr.mid(startIdx,endIdx-startIdx);
        item.routNumber=codec->toUnicode(temp);

        startIdx=endIdx+1;
        endIdx=tempArr.indexOf(',',startIdx);
        if(endIdx > 0)
        {
            temp.clear();
            temp=tempArr.mid(startIdx,endIdx-startIdx);
            item.timeLeft=codec->toUnicode(temp);
        }
        else
        {
            endIdx=tempArr.indexOf(';',startIdx);
            if(endIdx > 0)
            {
                temp.clear();
                temp=tempArr.mid(startIdx,endIdx-startIdx);
                item.timeLeft=codec->toUnicode(temp);
                currRoutList->append(item);
                break;
            }
        }
        startIdx=endIdx+1;
        currRoutList->append(item);
    }
    soundTrackCount=0;
    timetrackFlag=SND_TRACK;
    player = new QMediaPlayer;
    QString path=QApplication::applicationDirPath();
    path.append("/TRACK/");
    path.append(currRoutList->at(soundTrackCount).routNumber);
    path.append(".wav");
    player->setMedia(QUrl::fromLocalFile(path));
    player->setVolume(50);
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(PlayerStateChanged(QMediaPlayer::State)));
    player->play();
}

MainWindow::~MainWindow()
{
    delete currRoutList;
    delete player;
    delete ui;
}


void MainWindow::PlayerStateChanged(QMediaPlayer::State state)
{
    if(state==QMediaPlayer::StoppedState)
    {
loop:
        if(timetrackFlag==SND_TIME)
        {
            timetrackFlag=SND_TRACK;
            if(soundTrackCount < currRoutList->size())
            {
                QString path=QApplication::applicationDirPath();
                path.append("/TRACK/");
                path.append(currRoutList->at(soundTrackCount).routNumber);
                path.append(".wav");
                player->setMedia(QUrl::fromLocalFile(path));
                player->play();
            }
            else
            {
               close();
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
            QString path=QApplication::applicationDirPath();
            path.append("/TIME/");
            path.append(currRoutList->at(soundTrackCount).timeLeft);
            path.append("min.wav");
            player->setMedia(QUrl::fromLocalFile(path));
            player->play();
            soundTrackCount++;
        }
    }
}
