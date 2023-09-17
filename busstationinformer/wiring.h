#ifndef ONEWIRE_H
#define ONEWIRE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include "mainwindow.h"
#include <QFile>
#include <pthread.h>

class WiringPins : public QWidget
{
 Q_OBJECT
public:
    explicit WiringPins(MainWindow *w=nullptr);  // w указатель на класс главного окна
    ~WiringPins();

    char path[50];
    char rom[20];
    char buf[100];
    DIR *dirp;
    struct dirent *direntp;
    bool initCompletedFlag;
    pthread_t buttonsThread;
    pthread_t w1Thread;

    void SendTempr(float *pTempr);
    void runW1(void);
    void runButtons(void);

};

#endif // 1_WIRE_H
