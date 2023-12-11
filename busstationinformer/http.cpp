
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
void httpProcess::SendWeatherTempr(int *pTempr)
{
    RedrawMainWindow *ev=new RedrawMainWindow((QEvent::Type)(QEvent::User));
    ev->SendingMsg(RedrawMainWindow::UPDATE_WEATHER_TEMPR);
    ev->SendingData(pTempr);
    QApplication::postEvent(parent(),ev);
}
/*
 *
 */
int httpProcess::parsingConfigParam(const char *token,QByteArray &array,QString &param)
{
    int startIndex,endIdx;

    int len=strlen(token);
    startIndex=array.indexOf(token);
    if(startIndex>=0)
    {
      startIndex+=len;
      endIdx=array.indexOf(';',startIndex);
      param=array.mid(startIndex,endIdx-startIndex);
      return 0;
    }
    else
        return -1;
}
/*
 *
 */
httpProcess::httpProcess(MainWindow *w):
    buffIndex{-1}
  ,httpReqState{HTTP_REQ_IDLE},weatherReqState{HTTP_REQ_IDLE}
{
    setParent(w,Qt::Window);
    errors.allbits=0;

   QFile file(QApplication::applicationDirPath()+"/appconfig.txt");
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

   QString id;
   if(parsingConfigParam("id=",array,id)<0)
   {
       RedrawMainWindow *ev=new RedrawMainWindow(QEvent::User);
       ev->SendingMsg(RedrawMainWindow::FILECONFIG_ERR_MESSAGE);
       errors.fileconfigErr=1;
       ev->SendingData((uint32_t*)&errors);
       QApplication::postEvent(parent(),ev);
       return;
   }

   QString server;
   if(parsingConfigParam("url=",array,server)<0)
   {
       RedrawMainWindow *ev=new RedrawMainWindow(QEvent::User);
       ev->SendingMsg(RedrawMainWindow::FILECONFIG_ERR_MESSAGE);
       errors.fileconfigErr=1;
       ev->SendingData((uint32_t*)&errors);
       QApplication::postEvent(parent(),ev);
       return;
   }
   server.append("/tablo/?id="+id+"&ver=1.0.2&csq=87&tpcb=42&tcpu=47&ext=27&up=3218&br=6 HTTP/1.0");

   url.setUrl(server);
  // url.setUrl("http://84.22.159.130:2929/tablo/?id=100&ver=1.0.2&csq=87&tpcb=42&tcpu=47&ext=27&up=3218&br=6 HTTP/1.0");
   request=QNetworkRequest(url);
   connect(&httpRequestTimer,SIGNAL(timeout()),this,SLOT(httpTimerExpired()));
   httpRequestTimer.setInterval(45000);
   httpRequestTimer.start();
   startRequest();

   /* weather */
   QString latitude,longitude,apikey;
   if(parsingConfigParam("lat=",array,latitude)==0)
   {
     if(parsingConfigParam("lon=",array,longitude)==0)
     {
        if(parsingConfigParam("apikey=",array,apikey)<0)
            apikey.append("dd616505-6ec2-433b-a08e-a99a9bfcee40");
     }
     else
     {
         longitude.append("92.052572");
         apikey.append("dd616505-6ec2-433b-a08e-a99a9bfcee40");
     }
   }
   else
   {
       longitude.append("92.052572");
       latitude.append("56.810563");
       apikey.append("dd616505-6ec2-433b-a08e-a99a9bfcee40");
   }
   url.clear();
   url.setUrl("https://api.weather.yandex.ru/v2/informers?lat="+latitude+"&lon="+longitude+"&lang=en_US");   //56.010563&lon=92.852572
   //url.setUrl("http://api.openweathermap.org/data/2.5/find?q=Krasnoyarsk,RU&type=like&units=metric&APPID=10d39378d7d6d6ac8dabf60d74155c76"); //1d879dedc7592d989f5aefb7f38b8ca1
   weatherRequest=QNetworkRequest(url);
   //weatherRequest.setHeader(QNetworkRequest::ContentTypeHeader,"text/html");
   weatherRequest.setRawHeader(QByteArray("X-Yandex-API-Key"),QByteArray(apikey.toUtf8())) ;
   connect(&weatherRequestTimer,SIGNAL(timeout()),this,SLOT(weatherTimerExpired()));
   //weatherRequestTimer.setInterval(1800000);
   startWeatherRequest();
}
/*
 *
 */
httpProcess::~httpProcess()
{   }
/*
 *
 */
void httpProcess::parsingRoutsData(QByteArray &data, QVector<ROUT_ITEM> &routlist)
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
//
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
//
void httpProcess::httpReadyRead(void)
{
     if(!reply->error())
     {
        QByteArray dataRaw;
        dataRaw=reply->readAll();
        if(buffIndex==0)   // 0
            parsingRoutsData(dataRaw,*((MainWindow*)parent())->routlistBack);
        else            //1, -1
            parsingRoutsData(dataRaw,*((MainWindow*)parent())->routlistFront);
     }
     else
       reply->abort();

}
//
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
/*
 *
 */
float httpProcess::parsingWeatherData(QByteArray &data)
{
    float tempr;
    int idxStart=data.indexOf("\"temp\":");
    int idxEnd=data.indexOf(',',idxStart);
    if(idxStart>=0 && idxEnd>=0)
    {
        idxStart+=7;
        QByteArray temp=data.mid(idxStart,idxEnd-idxStart);
        bool ok;
        tempr=temp.toFloat(&ok);
        if(ok==true)
            return tempr;
        else
          return -1000;
    }
    else
      return -1000;
}
//
void httpProcess::sslErrors(const QList<QSslError> &errors)
{
    QString errorString;
    for (const QSslError &error : errors) {
        if (!errorString.isEmpty())
            errorString += '\n';
        errorString += error.errorString();
    }
}
//
void httpProcess::weatherFinished(void)
{
    int err=weatherReply->error();
    if(!err)
    {
        weatherRequestTimer.start(1800000);
        weatherReqState=HTTP_REQ_COMPLETED;
//        if(errors.connectionErr==1)
//        {
//           RedrawMainWindow *ev=new RedrawMainWindow(QEvent::User);
//           ev->SendingMsg(RedrawMainWindow::CONNECT_ERR_MESSAGE);
//           errors.connectionErr=0;
//           ev->SendingData(&errors.allbits);
//           QApplication::postEvent(parent(),ev);
//        }
    }
    else
    {
//       if(errors.connectionErr==0)
//       {
//            RedrawMainWindow *ev=new RedrawMainWindow(QEvent::User);
//            ev->SendingMsg(RedrawMainWindow::CONNECT_ERR_MESSAGE);
//            errors.connectionErr=1;
//            ev->SendingData(&errors.allbits);
//            QApplication::postEvent(parent(),ev);
//       }
       weatherRequestTimer.start(60000);
       weatherReqState=HTTP_REQ_IDLE;
    }
    weatherReply->deleteLater();
}
//
void httpProcess::weatherReadyRead(void)
{
     if(!weatherReply->error())
     {
        QByteArray dataRaw;
        dataRaw=weatherReply->readAll();
        float temp=parsingWeatherData(dataRaw);
        if(temp!=-1000)
        {
            weatherTempr=temp;
            SendWeatherTempr(&weatherTempr);
        }
     }
     else
       weatherReply->abort();
}
//
void httpProcess::startWeatherRequest(void)
{


    weatherReply=netManager.get(weatherRequest);
    connect(weatherReply,SIGNAL(finished()),this,SLOT(weatherFinished()));
    connect(weatherReply,SIGNAL(readyRead()),this,SLOT(weatherReadyRead()));
    //connect(&netManager,SIGNAL( sslErrors(QNetworkReply*,QList<QSslError>)) , SLOT( sslErrors(QNetworkReply*,QList<QSslError>)));
    connect(weatherReply, &QNetworkReply::sslErrors, this, &httpProcess::sslErrors);
    weatherReqState=HTTP_REQ_BUSY;


}
//
void httpProcess::weatherTimerExpired(void)
{
     startWeatherRequest();
}
//
//
//
