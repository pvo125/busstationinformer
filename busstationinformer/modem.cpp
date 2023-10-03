#include "modem.h"
#include "mainwindow.h"
#include <QApplication>
#include <QThread>


void BGS2_E::run(void)
{

  while(1)
  {
     AT_CSQ();
     sleep(1);
     AT_CSQ();
  }

}
/*
 *
 */
//void * GSMThreadFunc(void *arg)
//{
//  int unused;
//  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &unused);
//  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,&unused);

//  MainWindow *w=(MainWindow*)arg;
//  w->gsmmodule=new BGS2_E(w);
//  while(1)
//   {
//       w->gsmmodule->AT_CSQ();
//       sleep(1);

//   }
//}

BGS2_E::BGS2_E(MainWindow *w)
{
    //setParent(w,Qt::Window);
    mainW=w;
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
    else        // если порт открылся с ошибкой значит занят или не рабочий.
    {
        RedrawMainWindow *ev=new RedrawMainWindow((QEvent::Type)(QEvent::User));
        ev->SendingMsg(RedrawMainWindow::COMPORT_ERR_MESSAGE);
        errors.comportConnErr=1;
        ev->SendingData((uint32_t*)&errors);
        QApplication::postEvent(mainW,ev);
        return;
        //ErrorMsg("COM ПОРТ ЗАНЯТ!");      // Выводим Message
    }
   /*сигнал readyRead привязываем к объекту QSerialPort serial, слот RecieveBytes() к данному классу окна MainWindow*/
   connect(&serial,SIGNAL(readyRead()),this,SLOT(RecieveBytes()));
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
/*
 *
 */
void BGS2_E::gsmTimerStart(int tout)
{
    RedrawMainWindow *ev=new RedrawMainWindow((QEvent::Type)(QEvent::User));
    ev->SendingMsg(RedrawMainWindow::GSM_TIMER_START);
    timerdelay=tout;
    ev->SendingData(&timerdelay);
    QApplication::postEvent(mainW,ev);
}
/*
 *
 */
void BGS2_E::gsmTimerStop(void)
{
    RedrawMainWindow *ev=new RedrawMainWindow((QEvent::Type)(QEvent::User));
    ev->SendingMsg(RedrawMainWindow::GSM_TIMER_STOP);
    ev->SendingData(NULL);
    QApplication::postEvent(mainW,ev);
}
/*
 *
 */
int BGS2_E::WaitForString(const char *s,QString &waitedStr,int timeout)
{
    QString str;
    timeExpiredFlag=0;
    gsmTimerStart(timeout);
    do
    {
       if(!gsm_str.isEmpty())
       {
           str=gsm_str.dequeue();
           if(str.compare(s)==0)
           {
               gsmTimerStop();
               break;
           }
           else // если вытащили из контейнера очереди не то что ожидали значит это URC проскочил
           {    // перекладываем это сообщение в контейнер URC
              urc.enqueue(str);
           }
       }
    }
    while(!timeExpiredFlag);
    if(timeExpiredFlag)
        return -1;
    waitedStr=str;
    return 0;
}
/*
 *
 */
int BGS2_E::AT_CSQ(void)
{
    bool ok;
    QString response;
    QString s;
    int result;
    int index;

    result=serial.write("AT+CSQ\r");
    if(WaitForString("+CSQ:",response,3000)<0)
        return -1;
    if(WaitForString("OK",response,3000)<0)
        return -1;
    index=response.indexOf(":");
    s=response.left(index);
    result=s.toUInt(&ok);
    if(ok==true)
      return result;
    else
      return -1;
}

/*
 *
 *
 */
//
//
//
