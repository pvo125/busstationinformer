#include "mainwindow.h"
#include <QApplication>
#include <QMediaPlayer>
#include <QVideoWidget>
#ifndef Q_OS_WIN
    #include <sys/types.h>
    #include <unistd.h>
    #include <sys/resource.h>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(argv[1]);
    w.setGeometry(0,0,1920,1080);
    w.show();//FullScreen();
    return a.exec();
}
