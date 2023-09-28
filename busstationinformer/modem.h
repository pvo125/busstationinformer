#ifndef MODEM_H
#define MODEM_H

#include <QtSerialPort/QSerialPort>
#include <QQueue>

class BGS2_E : public QObject
{
    Q_OBJECT
public:
    explicit BGS2_E(void);
    ~BGS2_E();

private:
    bool foundNewLine;
    QSerialPort serial;
    QQueue<QString> urc;
    QQueue<QString> gsm_str;

private slots:
     void RecieveBytes();
};



#endif // MODEM_H
