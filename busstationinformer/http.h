#ifndef HTTP_H
#define HTTP_H
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QTimer>
#include "mainwindow.h"
#include <QVector>
#include <QEvent>
#include <QFile>


class UpdateRoutList : public QEvent
{
public:
   int msg;
   void* pData;
   enum REDRAW_MSG
   {
      UPDATE_ROUT_LIST=1,
   };
   UpdateRoutList(Type type) : QEvent(type) { }
   ~UpdateRoutList();
   void SetMsg(int p) { msg=p; }
   int GetMsg(void) { return msg; }
   void SetData(void *p) {pData=p;}
   void * GetData(void) { return pData; }
};










class httpProcess: public QWidget{

Q_OBJECT

public:
    enum HTTP_REQUEST_STATE
    {
        HTTP_REQ_IDLE,
        HTTP_REQ_BUSY,
        HTTP_REQ_COMPLETED
    };

    explicit httpProcess(QMainWindow *parent=nullptr);
    ~httpProcess();
private:

    int          buffIndex;
    enum HTTP_REQUEST_STATE  httpReqState;

    QMainWindow           *parentWindow;
    QTimer                httpRequestTimer;
    QNetworkAccessManager netManager;
    QUrl                  url;
    QNetworkReply         *reply;
    QNetworkRequest       request;

    void startRequest(void);
    void parcingData(QByteArray &data,QVector<ROUT_ITEM> &list);
    void SendUpdateMsg(enum UpdateRoutList::REDRAW_MSG msg, void *pData);

public slots:
    void httpFinished();
    void httpReadyRead();
    void httpTimerExpired();
};



#endif // HTTP_H
