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
#include <QThread>

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
        uint32_t comportOpenErr  :1;   // ошибка открытия COMx порта
        uint32_t comportConnErr  :1;   // ошибка соединения с gsm модулем.
    };
    uint32_t allbits;
}ERRORS;

class httpProcess;
class WiringPins;
class MainWindow;
class BGS2_E;
class videoplayer;


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
      CALL112_BUTTON_PRESS,
      FILECONFIG_ERR_MESSAGE,
      CONNECT_ERR_MESSAGE,

      COMPORT_ERR_MESSAGE,
      GSM_PARAM,
      GSM_TIMER_START,
      GSM_TIMER_STOP
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
    QMediaPlayer *soundPlayer;
    videoplayer *vplayer;
    QTimer *videotimer;
    QVector<ROUT_ITEM> *routlistFront;
    QVector<ROUT_ITEM> *routlistBack;
    QVector<ROUT_ITEM>  *currRoutList;
    WiringPins *w_pins;
    float onewiretempr;
    QBuffer *buffer;
    QByteArray *arr;

    BGS2_E *gsmmodule;
    QThread *gsmThread;

    InfoMsg *FileConfigError;       // Ошибка открытия или чтение данных файла конфига
    InfoMsg *COMPortOpenError;      // Ошибка открытия COM порта при старте программы

    InfoMsg *NoConnectWarning;      // Предупреждение
    InfoMsg *COMPortConnWarning;     // предупреждение об обрыве связи с gsm модулем

    InfoMsg *NoActiveRoutsNotify;   // Уведомление
    InfoMsg *Call112Notify;   // Уведомление
    //ERRORS  errors;

    bool routStringEmptyFlag[4];
private:
    Ui::MainWindow *ui;
    int CalcGsmSignalPower(int rssi);
    int StartVideoPlayer(void);
    int StopVideoPlayer(void);

protected:
    virtual void keyPressEvent(QKeyEvent *ev);
    void customEvent(QEvent *event);

private slots:
    void videoTimerExpired(void);
    void secTimerExpired(void);
    void routViewTimerExpired(void);
    void soundPlayerStateChanged(QMediaPlayer::State);
    //void videoPlayerStateChanged(QMediaPlayer::State);
    //void displayErrorMessage(void);
};
#endif // MAINWINDOW_H
