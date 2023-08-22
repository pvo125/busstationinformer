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

class httpProcess: public QWidget{

Q_OBJECT

public:
    explicit httpProcess(QMainWindow *parent=nullptr);
    ~httpProcess();
    void startRequest(void);
    void parcingData(QByteArray &data,QVector<ROUT_ITEM> &list);

private:
    bool          buffIndex;
    QMainWindow *parentWindow;
    QTimer                httpRequestTimer;
    QNetworkAccessManager netManager;
    QUrl                  url;

    QNetworkReply *reply;
    QNetworkRequest request;

public slots:
    void httpFinished();
    void httpReadyRead();

    void httpTimerExpired();


};



#endif // HTTP_H
