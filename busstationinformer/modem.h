#ifndef MODEM_H
#define MODEM_H

#include <QtSerialPort/QSerialPort>
#include <QQueue>
#include <QTimer>
#include <pthread.h>
#include <unistd.h>
#include "mainwindow.h"

class BGS2_E : public QObject
{
    Q_OBJECT
public:

    explicit BGS2_E(MainWindow *w=nullptr);
    ~BGS2_E();
    QSerialPort serial;
    bool timeExpiredFlag;

private:
    MainWindow *mainW;
    int timerdelay;
    bool foundNewLine;

    ERRORS errors;

    QQueue<QString> urc;
    QQueue<QString> gsm_str;
    QString inputMsg;

    int AT_CSQ(void);

    void gsmTimerStart(int tout);
    void gsmTimerStop(void);
    int WaitForString(const char *s,QString  &, int timeout);


public slots:
    void run();
    void RecieveBytes();

};



#endif // MODEM_H
