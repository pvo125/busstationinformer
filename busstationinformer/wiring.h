#ifndef ONEWIRE_H
#define ONEWIRE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

#include <QMainWindow>
#include "mainwindow.h"
#include <QFile>
#include <pthread.h>

class WiringPins : public QWidget
{
 Q_OBJECT
public:
    explicit WiringPins(MainWindow *w=nullptr);
    ~WiringPins();

    char path[50];
    char rom[20];
    char buf[100];
    DIR *dirp;
    struct dirent *direntp;
    bool initCompletedFlag;
    pthread_t wiringThread;

    void SendTempr(float *pTempr);
    void run(void);

};

#endif // 1_WIRE_H
