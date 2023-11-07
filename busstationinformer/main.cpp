#include "mainwindow.h"
#include <QApplication>
#ifndef Q_OS_WIN
    #include <sys/types.h>
    #include <unistd.h>
    #include <sys/resource.h>
#endif

int main(int argc, char *argv[])
{
//    int which = PRIO_PROCESS;
//    id_t pid;
//    int priority = 2;
//    //int ret;
//    pid = getpid();
//    setpriority(which, pid, priority);

    QApplication a(argc, argv);
    MainWindow w;
#ifdef Q_OS_WIN
    w.show();
#else
    w.show();//FullScreen();
#endif
    return a.exec();
}
