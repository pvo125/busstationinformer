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
    enum GSM_CMD_IDX
    {
      CSQ_CMD,

      TOTAL_CMD
    };

    explicit BGS2_E(MainWindow *w=nullptr);
    ~BGS2_E();
     QSerialPort serial;

     int AT_CSQ(void);
private:
    int indexCmdChain;
    bool foundNewLine;
    bool timeExpiredFlag;
    ERRORS errors;

    QQueue<QString> urc;
    QQueue<QString> gsm_str;
    QString inputMsg;
    QTimer gsmDelayTimer;


    void gsmCommandChain(int cmdIdx);

    int WaitForString(const char *s,QString  &, int timeout);


private slots:
     void RecieveBytes();
     void delayTimerExpired();
};



#endif // MODEM_H
