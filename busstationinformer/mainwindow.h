#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>
#include <QtMultimedia/QMediaPlayer>
#include <QVector>

typedef struct
{
   QString routNumber;
   QString routName;
   QString timeLeft;
}ROUT_ITEM;

class httpProcess;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    httpProcess *http;
    QTimer secTimer;
    QTimer routViewTimer;
    QMediaPlayer *player;
    QVector<ROUT_ITEM> *routlistFront;
    QVector<ROUT_ITEM> *routlistBack;

    int count=2;
private:
    Ui::MainWindow *ui;


protected:
    virtual void keyPressEvent(QKeyEvent *ev);

private slots:
    void secTimerExpired(void);
    void routViewTimerExpired(void);
    void playerStateChanged(QMediaPlayer::State);
};
#endif // MAINWINDOW_H
