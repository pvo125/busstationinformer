#include "modem.h"
#include "mainwindow.h"
#include <QApplication>
#include <QThread>

void BGS2_E::SendGsmParam(GSM_PARAM *pgsm)
{
    RedrawMainWindow *ev=new RedrawMainWindow(QEvent::User);
    ev->SendingMsg(RedrawMainWindow::GSM_PARAM);
    ev->SendingData(reinterpret_cast<uint32_t*>(pgsm));
    QApplication::postEvent(mainW,ev);
}
void BGS2_E::SendGsmErrors(ERRORS *err)
{
    RedrawMainWindow *ev=new RedrawMainWindow(QEvent::User);
    ev->SendingMsg(RedrawMainWindow::COMPORT_ERR_MESSAGE);
    ev->SendingData(reinterpret_cast<uint32_t*>(err));
    QApplication::postEvent(mainW,ev);
}

void BGS2_E::gsmProcess(void)
{
  serial=new QSerialPort(this);
  loop=new QEventLoop();
  thread()->msleep(5000);
  if(PortInit(serial)!=-1)
  {
    ATE();
    AT_SM20(0);
    while(!threadExitRequest)
    {
      thread()->msleep(1000);
      gsmParam.netReg=AT_CREG();
      if(gsmParam.netReg==0)
          continue;

      if(callRequest)
      {
          callRequest=false;
          callState=1;
          QFile file(QApplication::applicationDirPath()+"/appconfig.txt");
          if(file.open(QIODevice::ReadOnly)==true)
          {
            int startIdx,endIdx;
            QByteArray array=file.readAll();
            file.close();
            startIdx=array.indexOf("phone=");
            if(startIdx >=0)
            {
              startIdx+=6;
              endIdx=array.indexOf(';',startIdx);
              QString phone=array.mid(startIdx,endIdx-startIdx);
              ATD(phone);
            }
          }
      }
      else if(hangUp)
      {
         hangUp=false;
         callState=0;
         ATH();

      }
      gsmParam.rssi=AT_CSQ();
      ProcessURC();
      if(!threadExitRequest)
        SendGsmParam(&gsmParam);
      gsm_str.clear();
      urc.clear();
      flagMsgComplete=true;

    }
    if(callState)
       ATH();
  }
 emit finishedPort();
}
/*
 *
 */
BGS2_E::BGS2_E(MainWindow *w)
{
    mainW=w;
    callState=0;
    callRequest=false;
    hangUp=false;

    threadExitRequest=0;
    errors.allbits=0;
    flagMsgComplete=true;
    gsmParam.netReg=-1;
    gsmParam.rssi=99;

    serial=nullptr;
    gsmtimer=nullptr;
    loop=nullptr;

}
/*
 *
 */
BGS2_E::~BGS2_E()
{
  if(serial->isOpen())
  {
      serial->clear();
      serial->close();
  }
  if(serial)
  {
    delete serial;
    serial=nullptr;
  }
  if(loop)
  {
      delete loop;
      loop=nullptr;
  }
  if(gsmtimer)
  {
      delete gsmtimer;
      gsmtimer=nullptr;
  }
  mainW->gsmmodule=nullptr;
}
/*
 *
 */
int BGS2_E::PortInit(QSerialPort *ser)
{
#ifdef Q_OS_WIN
    ser->setPortName("COM4");  // устанавливаеми имя порта с виджета выбора доступных COM в системе
#else
    ser->setPortName("/dev/ttyUSB0");  // устанавливаеми имя порта с виджета выбора доступных COM в системе
#endif
    ser->setBaudRate(QSerialPort::Baud9600);  /*speed=9600;*/
    ser->setDataBits(QSerialPort::Data8);
    ser->setParity(QSerialPort::EvenParity);
    ser->setFlowControl(QSerialPort::NoFlowControl);
    ser->setStopBits(QSerialPort::OneStop);
    if(!ser->open(QIODevice::ReadWrite))          // открываем данный порт с проверкой
        // если порт открылся с ошибкой значит занят или не рабочий.
    {
        errors.comportOpenErr=1;
        SendGsmErrors(&errors);
        return -1;
        //ErrorMsg("COM ПОРТ ЗАНЯТ!");      // Выводим Message
    }
    /*настроим интервальный таймер для измерений таймаутов ожидания пакетом от модуля */
    gsmtimer=new QTimer(this);
    gsmtimer->setSingleShot(true);
    connect(gsmtimer,SIGNAL(timeout()),SLOT(gsmTimerExpired()));
   /*сигнал readyRead привязываем к объекту QSerialPort serial, слот RecieveBytes() к данному классу окна MainWindow*/
   connect(ser,SIGNAL(readyRead()),this,SLOT(RecieveBytes()));
   connect(ser,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(errorHandle(QSerialPort::SerialPortError)));
   return 0;
}
/*
 *
 */
void BGS2_E::gsmTimerExpired(void)
{
    timerdelay=0;
    if(!errors.comportConnErr && !errors.comportOpenErr)
    {
        errors.comportConnErr=1;
        SendGsmErrors(&errors);
    }
}
/*
 *
 */
void BGS2_E::errorHandle(QSerialPort::SerialPortError error)
{
    if(error==QSerialPort::ResourceError)
    {
        if(!errors.comportOpenErr)
        {
            errors.comportOpenErr=1;
            SendGsmErrors(&errors);

            gsmParam.netReg=-1;
            gsmParam.rssi=99;
            SendGsmParam(&gsmParam);
        }
        threadExitRequest=1;
    }
}
/*
 *
 */
void BGS2_E::RecieveBytes(void)
{
    int startIdx=0,endIdx,size;
    QByteArray temp;
    QByteArray arr=serial->readAll();

    size=arr.size();
    do
      {
        if(flagMsgComplete)
        {
            flagMsgComplete=false;
            startIdx=arr.indexOf("\r\n",startIdx);
            endIdx=arr.indexOf("\r\n",startIdx+2);
            if(startIdx < 0)
                break;
            else if(endIdx > 0)
            {
                temp=arr.mid(startIdx+2,endIdx-startIdx-2);
                gsm_str.enqueue(temp);   // записываем принятое сообщение в очередь

                flagMsgComplete=true;
                startIdx=(endIdx+2);
            }
            else
            {
                inputMsg.append(temp);
                break;
            }
        }
        else
        {
            endIdx=arr.indexOf("\r\n",0);
            if(endIdx > 0)
            {
                temp=arr.mid(0,endIdx);
                inputMsg.append(temp);
                gsm_str.enqueue(inputMsg);   // записываем принятое сообщение в очередь
                inputMsg.clear();
                flagMsgComplete=true;
                startIdx=(endIdx+2);
            }
            else
            {
                inputMsg.append(temp);
                break;
            }
        }
     }while(startIdx < size);
}
/*
 *
 */
//
//
//
int BGS2_E::ProcessURC(void)
{
   QString str;
   if(threadExitRequest)
       return -1;
   while(!urc.isEmpty() && !threadExitRequest)
   {
      str=urc.dequeue();
      if(str.contains("RING"))
      {
         ATH();
      }
      else if(str.contains("NO CARRIER"))
      {
        //hangUp=true;
        if(callState)
        {
            RedrawMainWindow *ev=new RedrawMainWindow(QEvent::User);
            ev->SendingMsg(RedrawMainWindow::CALL112_BUTTON_PRESS);
            ev->SendingData(nullptr);
            QApplication::postEvent(mainW,ev);
        }
      }
      else if(str.contains("BUSY"))
      {
          //hangUp=true;
          if(callState)
          {
              RedrawMainWindow *ev=new RedrawMainWindow(QEvent::User);
              ev->SendingMsg(RedrawMainWindow::CALL112_BUTTON_PRESS);
              ev->SendingData(nullptr);
              QApplication::postEvent(mainW,ev);
          }

      }
      else if(str.contains("NO DIALTONE"))
      {

      }
   }
   return 0;

}
/*
 *
 */
int BGS2_E::WaitForString(const char *s,QString *waitedStr,int timeout)
{
    QString str;
    timerdelay=1;
    gsmtimer->start(timeout*10);
    while(timerdelay && !threadExitRequest)
    {
       if(!gsm_str.isEmpty())
       {
           str=gsm_str.dequeue();
           if(str.contains(s))
           {
               gsmtimer->stop();
               break;
           }
           else // если вытащили из контейнера очереди не то что ожидали значит это URC проскочил
           {    // перекладываем это сообщение в контейнер URC
              urc.enqueue(str);
           }
       }
       else
       {
          //QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
          loop->processEvents(QEventLoop::ExcludeUserInputEvents);
          thread()->msleep(5);
       }
    }
    if(!timerdelay)
    {
      return -1;
    }
    else if(errors.comportConnErr)
    {
        errors.comportConnErr=0;
        SendGsmErrors(&errors);
    }

    if(waitedStr)
        waitedStr->append(str);
    return 0;
}
/*
 *
 */
int BGS2_E::AT(void)
{
    if(threadExitRequest)
        return -1;
    serial->write("AT\r");
    if(WaitForString("OK",nullptr,GSM_TOUT)<0)
        return -1;
    thread()->msleep(GSM_CMD_PAUSE);
    return 1;
}
/*
 *
 */
int BGS2_E::ATE(void)
{
    if(threadExitRequest)
        return -1;
    serial->write("ATE0\r");
    if(WaitForString("OK",nullptr,GSM_TOUT)<0)
        return -1;
    thread()->msleep(GSM_CMD_PAUSE);
    return 1;
}
/*
 *
 */
int BGS2_E::AT_CREG(void)
{
    bool ok;
    QString response;
    QString s;
    int result;
    int startIdx;

    if(threadExitRequest)
        return -1;
    serial->write("AT+CREG?\r");
    if(WaitForString("+CREG:",&response,GSM_TOUT)<0)
        return -1;
    if(WaitForString("OK",nullptr,GSM_TOUT)<0)
        return -1;
    startIdx=response.indexOf(',');
    startIdx+=1;
    s=response.mid(startIdx,1);
    result=static_cast<int>(s.toUInt(&ok));
    thread()->msleep(GSM_CMD_PAUSE);
    if(ok==true)
    {
        if(result==1 || result==5)
            return result;
        else    return 0;
    }
    else
     return -1;
}
/*
 *
 **/
int BGS2_E::AT_CSQ(void)
{
    bool ok;
    QString response;
    QString s;
    int result;
    int startIdx,endIdx;

    if(threadExitRequest)
        return -1;
    serial->write("AT+CSQ\r");
    if(WaitForString("+CSQ:",&response,GSM_TOUT)<0)
        return -1;
    if(WaitForString("OK",nullptr,GSM_TOUT)<0)
        return -1;
    startIdx=response.indexOf(':');
    startIdx+=2;
    endIdx=response.indexOf(',');
    s=response.mid(startIdx,endIdx-startIdx);
    result=static_cast<int>(s.toUInt(&ok));
    thread()->msleep(GSM_CMD_PAUSE);
    if(ok==true)
     return result;
    else
     return -1;
}
/*
 *
 *
 */
int BGS2_E::AT_SM20(int mode)
{
    QString s="AT^SM20=";
    if(threadExitRequest)
        return -1;
    s.append(QString::number(mode).append('\r'));
    QByteArray arr=s.toUtf8();
    serial->write(arr);
    if(WaitForString("OK",nullptr,GSM_TOUT)<0)
        return -1;
    thread()->msleep(GSM_CMD_PAUSE);
    return 1;
}
/*
 *
 */
int BGS2_E::ATD(QString &s)
{
    QString str="ATD";
    if(threadExitRequest)
        return -1;
    str.append(s).append(";\r");
    QByteArray arr=str.toUtf8();
    serial->write(arr);
    if(WaitForString("OK",nullptr,GSM_TOUT)<0)
        return -1;
    thread()->msleep(GSM_CMD_PAUSE);
    return 1;
}
/*
 *
 */
int BGS2_E::ATH(void)
{
    if(threadExitRequest)
        return -1;
    serial->write("ATH\r");
    if(WaitForString("OK",nullptr,GSM_TOUT)<0)
        return -1;
    thread()->msleep(GSM_CMD_PAUSE);
    return 1;
}
//
//
//
