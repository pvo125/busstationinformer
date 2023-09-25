#ifndef MODEM_H
#define MODEM_H

#include <QtSerialPort/QSerialPort>


class BGS2_E : public QObject
{
    Q_OBJECT
public:
    explicit BGS2_E(void);
    ~BGS2_E();

private:
     QSerialPort *serial;

private slots:
     void RecieveBytes();

};



#endif // MODEM_H
