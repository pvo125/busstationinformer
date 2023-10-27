
#include <http.h>
#include <QTextCodec>
#include <QApplication>
#include <QByteArray>
#include <QFile>
/*
 *
 */
void httpProcess::SendBuffIdx(int *pIdx)
{
    RedrawMainWindow *ev=new RedrawMainWindow(QEvent::User);
    ev->SendingMsg(RedrawMainWindow::UPDATE_ROUT_LIST);
    ev->SendingData(pIdx);
    QApplication::postEvent(parent(),ev);
}
/*
 *
 */
httpProcess::httpProcess(MainWindow *w):
    buffIndex{-1}
  ,httpReqState{HTTP_REQ_IDLE}
{
    int startIndex,endIdx;

    setParent(w,Qt::Window);
    errors.allbits=0;
#ifdef Q_OS_WIN
   QFile file("appconfig.txt");
#else
   QFile file("/home/pi/app/appconfig.txt");
#endif
   if(file.open(QIODevice::ReadOnly)==false)
   {
       RedrawMainWindow *ev=new RedrawMainWindow(QEvent::User);
       ev->SendingMsg(RedrawMainWindow::FILECONFIG_ERR_MESSAGE);
       errors.fileconfigErr=1;
       ev->SendingData((uint32_t*)&errors);
       QApplication::postEvent(parent(),ev);
       return;
   }
   QByteArray array=file.readAll();
   file.close();
   startIndex=array.indexOf("id=");
   if(startIndex==-1)
   {
       RedrawMainWindow *ev=new RedrawMainWindow(QEvent::User);
       ev->SendingMsg(RedrawMainWindow::FILECONFIG_ERR_MESSAGE);
       errors.fileconfigErr=1;
       ev->SendingData((uint32_t*)&errors);
       QApplication::postEvent(parent(),ev);
       return;
   }
   startIndex+=3;
   endIdx=array.indexOf(';',startIndex);
   QString id=array.mid(startIndex,endIdx-startIndex);
   startIndex=array.indexOf("url=");
   if(startIndex==-1)
   {
       RedrawMainWindow *ev=new RedrawMainWindow(QEvent::User);
       ev->SendingMsg(RedrawMainWindow::FILECONFIG_ERR_MESSAGE);
       errors.fileconfigErr=1;
       ev->SendingData((uint32_t*)&errors);
       QApplication::postEvent(parent(),ev);
       return;
   }
   startIndex+=4;
   endIdx=array.indexOf(';',startIndex);
   QString server=array.mid(startIndex,endIdx-startIndex);

   server.append("/tablo/?id=");
   server.append(id);
   server.append("&ver=1.0.2&csq=87&tpcb=42&tcpu=47&ext=27&up=3218&br=6 HTTP/1.0");

   url.setUrl(server);
  // url.setUrl("http://84.22.159.130:2929/tablo/?id=100&ver=1.0.2&csq=87&tpcb=42&tcpu=47&ext=27&up=3218&br=6 HTTP/1.0");
   request=QNetworkRequest(url);
   connect(&httpRequestTimer,SIGNAL(timeout()),this,SLOT(httpTimerExpired()));
   httpRequestTimer.setInterval(45000);
   httpRequestTimer.start();
   startRequest();
}
/*
 *
 */
httpProcess::~httpProcess()
{   }
/*
 *
 */
void httpProcess::parcingData(QByteArray &data, QVector<ROUT_ITEM> &routlist)
{
    QByteArray temp;
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    ROUT_ITEM item;
    int startIdx=0;
    int endIdx;

    routlist.clear();
    while(1)
    {
       startIdx=data.indexOf("n=\"",startIdx);
       if(startIdx!=-1)
       {
            startIdx+=3;  // начало строки номера маршрута
            endIdx=data.indexOf(';',startIdx);   // конец строки номера маршрута
            temp=data.mid(startIdx,endIdx-startIdx);
            item.routNumber.append(codec->toUnicode(temp));
            temp.clear();

            startIdx=endIdx+1;   // переходим к названию маршрута
            endIdx=data.indexOf(';',startIdx);   // конец строки названия маршрута
            QByteArray temp=(data.mid(startIdx,endIdx-startIdx));
            QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
            item.routName.append(codec->toUnicode(temp));
            temp.clear();

            startIdx=endIdx+1;   // переходим к времени ожидания маршрута
            endIdx=data.indexOf("\r\n",startIdx);   // конец строки времени ожидания маршрута
            item.timeLeft.append(data.mid(startIdx,endIdx-1-startIdx));
            routlist.append(item);

            startIdx=endIdx+2;
            item.routName.clear();
            item.routNumber.clear();
            item.timeLeft.clear();
       }
       else
            break;
    }
    if(!errors.noactiveRouts && routlist.isEmpty())
    {
        RedrawMainWindow *ev=new RedrawMainWindow(QEvent::User);
        ev->SendingMsg(RedrawMainWindow::NO_ACTIVE_ROUTS);
        errors.noactiveRouts=1;
        ev->SendingData(&errors);
        QApplication::postEvent(parent(),ev);
    }
    else if(errors.noactiveRouts && !routlist.isEmpty())
    {
        RedrawMainWindow *ev=new RedrawMainWindow(QEvent::User);
        ev->SendingMsg(RedrawMainWindow::NO_ACTIVE_ROUTS);
        errors.noactiveRouts=0;
        ev->SendingData(&errors);
        QApplication::postEvent(parent(),ev);
    }

}
/*
 *
 */
void httpProcess::httpFinished(void)
{
    if(!reply->error())
    {
        if(buffIndex==0)  buffIndex=1;
        else              buffIndex=0;
        SendBuffIdx(&buffIndex);
        httpReqState=HTTP_REQ_COMPLETED;
        if(errors.connectionErr==1)
        {
           RedrawMainWindow *ev=new RedrawMainWindow(QEvent::User);
           ev->SendingMsg(RedrawMainWindow::CONNECT_ERR_MESSAGE);
           errors.connectionErr=0;
           ev->SendingData(&errors.allbits);
           QApplication::postEvent(parent(),ev);
        }
    }
    else
    {
       if(errors.connectionErr==0)
       {
            RedrawMainWindow *ev=new RedrawMainWindow(QEvent::User);
            ev->SendingMsg(RedrawMainWindow::CONNECT_ERR_MESSAGE);
            errors.connectionErr=1;
            ev->SendingData(&errors.allbits);
            QApplication::postEvent(parent(),ev);
       }
       httpReqState=HTTP_REQ_IDLE;
    }
    reply->deleteLater();
}
/*
 *
 */
void httpProcess::httpReadyRead(void)
{
     if(!reply->error())
     {
        QByteArray dataRaw;
        dataRaw=reply->readAll();
        if(buffIndex==0)   // 0
            parcingData(dataRaw,*((MainWindow*)parent())->routlistBack);
        else            //1, -1
            parcingData(dataRaw,*((MainWindow*)parent())->routlistFront);
     }
     else
       reply->abort();

}
/*
 *
 */
void httpProcess::startRequest(void)
{
    if(httpReqState!=HTTP_REQ_BUSY)
    {
        reply=netManager.get(request);
        connect(reply,SIGNAL(finished()),this,SLOT(httpFinished()));
        connect(reply,SIGNAL(readyRead()),this,SLOT(httpReadyRead()));
        httpReqState=HTTP_REQ_BUSY;
    }
    else if(httpReqState==HTTP_REQ_BUSY)
    {
      reply->abort();
      httpReqState=HTTP_REQ_IDLE;
    }
}
//
void httpProcess::httpTimerExpired(void)
{
    if(!((MainWindow*)parent())->extSoundPlayerActive)
    {
        startRequest();
        httpRequestTimer.start(45000);
    }
}
