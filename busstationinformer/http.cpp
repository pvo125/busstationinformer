
#include <http.h>
#include <QTextCodec>
/*
 *
 */
httpProcess::httpProcess(QMainWindow *parent)
{
   buffIndex=0;
   parentWindow=parent;
   url.setUrl("http://84.22.159.130:2929/tablo/?id=100&ver=1.0.2&csq=87&tpcb=42&tcpu=47&ext=27&up=3218&br=6 HTTP/1.0");
   request=QNetworkRequest(url);
   connect(&httpRequestTimer,SIGNAL(timeout()),this,SLOT(httpTimerExpired()));
   httpRequestTimer.setInterval(30000);
   httpRequestTimer.start();


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
}
/*
 *
 */
void httpProcess::httpFinished(void)
{
   reply->deleteLater();
   buffIndex=!buffIndex;
}
/*
 *
 */
void httpProcess::httpReadyRead(void)
{
    QByteArray dataRaw;
    dataRaw=reply->readAll();
    if(buffIndex)   //1
        parcingData(dataRaw,*((MainWindow*)parentWindow)->routlistBack);
    else            //0
        parcingData(dataRaw,*((MainWindow*)parentWindow)->routlistFront);

}
/*
 *
 */
void httpProcess::startRequest(void)
{
    reply=netManager.get(request);
    connect(reply,SIGNAL(finished()),this,SLOT(httpFinished()));
    connect(reply,SIGNAL(readyRead()),this,SLOT(httpReadyRead()));
}
//
void httpProcess::httpTimerExpired(void)
{
    startRequest();
    httpRequestTimer.start(30000);
}
