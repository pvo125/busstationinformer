#include "mainwindow.h"
#include <QApplication>
#include <QMediaPlayer>
#include <QVideoWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(argv[1]);
    w.showFullScreen();
    return a.exec();
}
