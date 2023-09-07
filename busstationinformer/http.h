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



class httpProcess: public QWidget{

Q_OBJECT

public:
    enum HTTP_REQUEST_STATE
    {
        HTTP_REQ_IDLE,
        HTTP_REQ_BUSY,
        HTTP_REQ_COMPLETED
    };

    explicit httpProcess(MainWindow *w=nullptr);
    ~httpProcess();
private:

    int          buffIndex;
    enum HTTP_REQUEST_STATE  httpReqState;

    MainWindow           *mainW;          // // указатель на класс главного окна
    QTimer                httpRequestTimer;
    QNetworkAccessManager netManager;
    QUrl                  url;
    QNetworkReply         *reply;
    QNetworkRequest       request;
    ERRORS errors;

    void startRequest(void);
    void parcingData(QByteArray &data,QVector<ROUT_ITEM> &list);
    void SendBuffIdx(int *pIdx);

public slots:
    void httpFinished();
    void httpReadyRead();
    void httpTimerExpired();
};



#endif // HTTP_H
