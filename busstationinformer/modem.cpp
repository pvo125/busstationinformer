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
    if(foundNewLine)
    {
       int index=arr.indexOf("\r\n",0);
       if(index!=-1)    // нет "\r\n" в принятом сообщении
       { // приняли промежуточный пакет байт строки.
         //  добавляем в контейнер принятого сообщения и выходим из слота до принятия "\r\n"
         inputMsg.append(arr);
       }
       else // есть "\r\n"  в принятом сообщении
       {// записываем принятый хвост в контейнер принятого сообщения
           inputMsg.chop(index);            // delete \r\n в конце сообщения
           gsm_str.enqueue(inputMsg);   // записываем принятое сообщение в очередь

           // подготовим контейнер для принятия следующего сообщения
           foundNewLine=false;
           inputMsg.clear();
       }
    }
    else
    {
       int index=arr.indexOf("\r\n",0);
       if(index!=-1)
       {
          QByteArray temp=arr.right(index+2);
          index=temp.indexOf("\r\n",0);
          if(index!=-1)
          {
              inputMsg.chop(index);            // delete \r\n в конце сообщения
              gsm_str.enqueue(inputMsg);   // записываем принятое сообщение в очередь

              // подготовим контейнер для принятия следующего сообщения
              foundNewLine=false;
              inputMsg.clear();
          }
          else
          {
            //  добавляем в контейнер принятого сообщения и выходим из слота до принятия "\r\n"
            inputMsg.append(temp);
            foundNewLine=true;
          }

       }
    }
}
