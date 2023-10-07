#ifndef MODEM_H
#define MODEM_H

#include <QtSerialPort/QSerialPort>
#include <QQueue>
#include <QTimer>
#include <pthread.h>
#include <unistd.h>
#include "mainwindow.h"
#include <QEventLoop>

#define GSM_CMD_PAUSE   100
#define GSM_TOUT        300

typedef struct
{
   uint8_t rssi;
   int8_t netReg;

}GSM_PARAM;


class BGS2_E : public QObject
{
    Q_OBJECT
public:

    explicit BGS2_E(MainWindow *w=nullptr);
    ~BGS2_E();
    QSerialPort *serial;
    QTimer *gsmtimer;
    QEventLoop *loop;
private:
    MainWindow *mainW;
    int timerdelay;
    bool flagMsgComplete;
    bool threadExit;
    ERRORS errors;
    GSM_PARAM gsmParam;
    QQueue<QString> urc;
    QQueue<QString> gsm_str;
    QString inputMsg;



    void SendGsmParam(GSM_PARAM *);
    void SendGsmErrors(ERRORS *);
    int PortInit(QSerialPort *);
    int WaitForString(const char *s, QString *, int timeout);
    int AT(void);
    int ATE(void);
    int AT_CREG(void);
    int AT_CSQ(void);

signals:
    void finishedPort();

public slots:
    void gsmProcess();
    void RecieveBytes(void);
    void gsmTimerExpired(void);



};



#endif // MODEM_H
