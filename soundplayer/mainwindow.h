#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>

typedef struct
{
   QString routNumber;
   QString routName;
   QString timeLeft;
}ROUT_ITEM;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum SND_TIMETRACK
    {
       SND_TRACK,
       SND_TIME
    };
    explicit MainWindow(const char *param_routs, QWidget *parent = 0);
    ~MainWindow();
private:
    int soundTrackCount;
    enum SND_TIMETRACK timetrackFlag;
    QVector<ROUT_ITEM>  *currRoutList;
    QMediaPlayer *player;
    Ui::MainWindow *ui;
private slots:
     void PlayerStateChanged(QMediaPlayer::State);

};

#endif // MAINWINDOW_H
