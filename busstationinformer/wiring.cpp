#include "wiring.h"
#include <QApplication>
#ifndef Q_OS_WIN
    #include <wiringPi.h>
#endif

static MainWindow *mainWind;   // указатель на класс главного окна
static bool soundButtonReq=false;

void * threadFunc(void *arg)
{
   WiringPins *p=(WiringPins*)arg;
   p->run();
   return 0;
}

void ButtonISR(void)
{
    soundButtonReq=true;
}

WiringPins::WiringPins(MainWindow *w):
    path{"/sys/bus/w1/devices/"},initCompletedFlag{false}
{
    mainWind=w;
    //mount the device:
#ifndef Q_OS_WIN
    system("sudo modprobe w1-gpio");
    system("sudo modprobe w1-therm");
    // Check if /sys/bus/w1/devices/ exists.
    if((dirp = opendir(path)) == NULL)
     {
          //printf("opendir error\n");
          return;
     }
    // Reads the directories or files in the current directory.
     while((direntp = readdir(dirp)) != NULL)
     {
          // If 28-00000 is the substring of d_name,
          // then copy d_name to rom and print rom.
          if(strstr(direntp->d_name,"28-"))
          {
              strcpy(rom,direntp->d_name);
              //printf(" rom: %s\n",rom);
          }
      }
      closedir(dirp);
      // Append the String rom and "/w1_slave" to path
      // path becomes to "/sys/bus/w1/devices/28-00000xxxx/w1_slave"
      strcat(path,rom);
      strcat(path,"/w1_slave");
      initCompletedFlag=true;

      /* Настройка вывода GPIO_0 на детекцию кнопки по заднему фронту */
      wiringPiSetup();
      pinMode(0,INPUT);
      pullUpDnControl(0,PUD_UP);
      wiringPiISR(0,INT_EDGE_FALLING,ButtonISR);
#endif

      pthread_create(&wiringThread,0,threadFunc,this);
}

WiringPins::~WiringPins()
{
    int ret=pthread_cancel(wiringThread);
    if(ret==0)
      pthread_join(wiringThread,NULL);
}

/*
 *
 */
void WiringPins::SendTempr(float *pTempr)
{
    RedrawMainWindow *ev=new RedrawMainWindow((QEvent::Type)(QEvent::User));
    ev->SendingMsg(RedrawMainWindow::UPDATE_TEMPR);
    ev->SendingData(pTempr);
    QApplication::postEvent(mainWind,ev);
}
/*
 *
 */
void WiringPins::run(void)
{
#ifndef Q_OS_WIN
  char *temp;
  float value;
#endif


  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
  QFile file(path);

   while(1)
   {
#ifndef Q_OS_WIN
      file.open(QIODevice::ReadOnly);
      QByteArray arr=file.readAll();
      file.close();
       // Returns the first index of 't'.
      temp = strchr(arr.data(),'t');
       // Read the string following "t=".
      sscanf(temp,"t=%s",temp);
       // atof: changes string to float.
      value = atof(temp)/1000;
      SendTempr(&value);


      if(soundButtonReq==true)
      {
          delay(100);
          if(digitalRead(0)==LOW)
          {
            RedrawMainWindow *ev=new RedrawMainWindow((QEvent::Type)(QEvent::User));
            ev->SendingMsg(RedrawMainWindow::SOUND_BUTTON_PRESS);
            ev->SendingData(NULL);
            QApplication::postEvent(mainWind,ev);
          }
          soundButtonReq=false;
      }
      delay(500);
      if(soundButtonReq==true)
      {
          if(digitalRead(0)==LOW)
          {
              RedrawMainWindow *ev=new RedrawMainWindow((QEvent::Type)(QEvent::User));
              ev->SendingMsg(RedrawMainWindow::SOUND_BUTTON_PRESS);
              ev->SendingData(NULL);
              QApplication::postEvent(mainWind,ev);
          }
          soundButtonReq=false;
      }
      delay(500);
#else
      sleep(1);
#endif
   }
}



