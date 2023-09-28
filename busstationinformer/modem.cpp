#include "modem.h"
#include "mainwindow.h"

BGS2_E::BGS2_E(void)
{
    foundNewLine=false;
    serial.setPortName("COM3");  // устанавливаеми имя порта с виджета выбора доступных COM в системе
    if(serial.open(QIODevice::ReadWrite))          // открываем данный порт с проверкой
    {// если открыли успешно продолжаем насройки порта
        serial.setBaudRate(QSerialPort::Baud9600);  /*speed=9600;*/
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::EvenParity);
        serial.setFlowControl(QSerialPort::NoFlowControl);
        serial.setStopBits(QSerialPort::OneStop);
    }
    //else        // если порт открылся с ошибкой значит занят или не рабочий.
    //   ErrorMsg("COM ПОРТ ЗАНЯТ!");      // Выводим Message

    /*сигнал readyRead привязываем к объекту QSerialPort serial, слот RecieveBytes() к данному классу окна MainWindow*/
    QObject::connect(&serial,SIGNAL(readyRead()),this,SLOT(RecieveBytes()));


}

BGS2_E::~BGS2_E()
{
  serial.clear();
  serial.close();
}
/*
 *
 */
void BGS2_E::RecieveBytes()
{
    QByteArray arr=serial.readAll();
    if(!foundNewLine)
    {
       if(arr.contains("\r\n")==true)
       {
           arr.indexOf("\r\n");
           foundNewLine=true;
       }
    }
    else if(arr.contains("\r\n"))
    {

    }





}
