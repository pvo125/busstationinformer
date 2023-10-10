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

#define PI_SOUND_PIN    0
#define PI_112CALL_PIN  1
#define PI_LEDBLINK_PIN 2

class WiringPins : public QWidget
{
 Q_OBJECT
public:
    bool initCompletedFlag;
    explicit WiringPins(MainWindow *w=nullptr);  // w указатель на класс главного окна
    ~WiringPins();
    void runW1(void);
    void runButtons(void);
private:
    char path[50];
    char rom[20];
    char buf[100];
    DIR *dirp;
    struct dirent *direntp;
    pthread_t buttonsThread;
    pthread_t w1Thread;
    bool callstate;
    void SendTempr(float *pTempr);


};

#endif // 1_WIRE_H
