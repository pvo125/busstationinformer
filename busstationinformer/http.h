#ifndef HTTP_H
#define HTTP_H
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

    explicit httpProcess(MainWindow *w=nullptr);    //  w указатель на класс главного окна
    ~httpProcess();
private:

    int          buffIndex;
    float weatherTempr;
    enum HTTP_REQUEST_STATE  httpReqState;
    QTimer                httpRequestTimer;
    QNetworkAccessManager netManager;
    QUrl                  url;
    QNetworkReply         *reply;
    QNetworkRequest       request;

    enum HTTP_REQUEST_STATE  weatherReqState;
    QTimer                weatherRequestTimer;
    QNetworkReply         *weatherReply;
    QNetworkRequest       weatherRequest;
    ERRORS errors;

    int parsingConfigParam(const char *token, QByteArray &array, QString &);
    void startRequest(void);
    void parsingRoutsData(QByteArray &data,QVector<ROUT_ITEM> &list);
    float parsingWeatherData(QByteArray &data);
    void SendBuffIdx(int *pIdx);
    void SendWeatherTempr(float *pTempr);
    void startWeatherRequest(void);

public slots:
    void httpFinished();
    void httpReadyRead();
    void httpTimerExpired();

    //void sslErrors(QNetworkReply*,QList<QSslError>);
    void sslErrors(const QList<QSslError> &errors);
    void weatherFinished();
    void weatherReadyRead();
    void weatherTimerExpired();
};



#endif // HTTP_H
