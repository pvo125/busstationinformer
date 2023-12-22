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
   int rssi;
   int netReg;

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
    bool threadExitRequest;
    bool callRequest;
    int callState;
    bool hangUp;

private:
    MainWindow *mainW;
    int timerdelay;
    bool flagMsgComplete;

    ERRORS errors;
    GSM_PARAM gsmParam;
    QQueue<QString> urc;
    QQueue<QString> gsm_str;
    QString inputMsg;



    void SendGsmParam(GSM_PARAM *);
    void SendGsmErrors(ERRORS *);
    int PortInit(QSerialPort *);

    int ProcessURC(void);
    int WaitForString(const char *s, QString *, int timeout);
    int AT(void);
    int ATE(void);
    int AT_CREG(void);
    int AT_CSQ(void);
    int AT_SM20(int mode);
    int ATD(QString &);
    int ATH(void);
    int AT_CLCC(int *);

signals:
    void finishedPort();

public slots:
    void gsmProcess();
    void RecieveBytes(void);
    void gsmTimerExpired(void);
    void errorHandle(QSerialPort::SerialPortError error);


};



#endif // MODEM_H
