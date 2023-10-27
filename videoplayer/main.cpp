#include "mainwindow.h"
#include <QApplication>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <iostream>

using namespace std;
int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        QApplication a(argc, argv);
        MainWindow w(argv[1]);
        w.setGeometry(0,0,1280,720);
        w.show();
        return a.exec();
    }
    else
    {
       std::cout<<"No file name!";
       return 0;
    }
}
