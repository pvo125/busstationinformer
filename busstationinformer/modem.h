#ifndef MODEM_H
#define MODEM_H

#include <QtSerialPort/QSerialPort>
#include <QQueue>
#include <QTimer>
#include <pthread.h>
#include <unistd.h>
#include "mainwindow.h"

class BGS2_E : public QWidget
{
    Q_OBJECT
public:
    explicit BGS2_E(MainWindow *w=nullptr);
    ~BGS2_E();
     void runThread(void);

    pthread_t gsmThread;
private:
    bool foundNewLine;
    bool timeExpiredFlag;
    ERRORS errors;


    QSerialPort serial;
    QQueue<QString> urc;
    QQueue<QString> gsm_str;
    QString inputMsg;
    QTimer delayTimer;

    int WaitForString(const char *s,QString  &, int timeout);
    int AT_CSQ(void);

private slots:
     void RecieveBytes();
     void delayTimerExpired();
};



#endif // MODEM_H
