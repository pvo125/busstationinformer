#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>
#include <QtMultimedia/QMediaPlayer>
#include <QVector>
#include <QBuffer>

#include <QFrame>
#include <QLabel>


typedef struct
{
   QString routNumber;
   QString routName;
   QString timeLeft;
}ROUT_ITEM;

typedef union
{
    struct
    {
        uint32_t connectionErr   :1;   // предупреждение
        uint32_t fileconfigErr   :1;   // ошибка
        uint32_t soundrtackErr   :1;
        uint32_t noactiveRouts   :1;   // уведомление
    };
    uint32_t allbits;
}ERRORS;

class httpProcess;
class WiringPins;
class MainWindow;


class InfoMsg : QWidget
{

    Q_OBJECT

public:
    enum TYPE_NOTIFY
    {
      ERR_MSG,
      WARN_MSG,
      NOTIFY_MSG,
    };

    InfoMsg(MainWindow *parent,QString &str,enum TYPE_NOTIFY type);
    ~InfoMsg();

    static int count;
    QFrame MsgWindow;
    QFrame inframe;
    QLabel labelInfoMsg;
    QLabel labelMsg;
};

class RedrawMainWindow : public QEvent
{
public:
   int msg;
   void* pData;
   enum REDRAW_MSG
   {
      UPDATE_ROUT_LIST=1,
      NO_ACTIVE_ROUTS,
      UPDATE_TEMPR,
      SOUND_BUTTON_PRESS,
      FILECONFIG_ERR_MESSAGE,
      CONNECT_ERR_MESSAGE
   };
   RedrawMainWindow(Type type) : QEvent(type) { }
   ~RedrawMainWindow() {}
   void SendingMsg(int p) { msg=p; }
   int GetingMsg(void) { return msg; }
   void SendingData(void *p) {pData=p;}
   void * GetingData(void) { return pData; }
};


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum SND_TIMETRACK
    {
       SND_TRACK,
       SND_TIME
    };
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void ClearStringsRouts(int startStrIdx);

    int soundTrackCount;
    enum SND_TIMETRACK timetrackFlag;
    int buffIdx;
    httpProcess *http;
    QTimer secTimer;
    QTimer routViewTimer;
    QMediaPlayer *player;
    QVector<ROUT_ITEM> *routlistFront;
    QVector<ROUT_ITEM> *routlistBack;
    QVector<ROUT_ITEM>  *currRoutList;
    WiringPins *w_pins;
    float onewiretempr;
    QBuffer *buffer;
    QByteArray *arr;

    InfoMsg *NoConnectWarning;      // Предупреждение
    InfoMsg *FileConfigError;       // Ошибка
    InfoMsg *NoActiveRoutsNotify;   // Уведомление
    //ERRORS  errors;

    bool routStringEmptyFlag[4];

protected:
    void customEvent(QEvent *event);

private:
    Ui::MainWindow *ui;


protected:
    virtual void keyPressEvent(QKeyEvent *ev);

private slots:
    void secTimerExpired(void);
    void routViewTimerExpired(void);
    void playerStateChanged(QMediaPlayer::State);
};
#endif // MAINWINDOW_H
