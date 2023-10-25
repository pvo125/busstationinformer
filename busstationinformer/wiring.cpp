#include "wiring.h"
#include <QApplication>
#ifndef Q_OS_WIN
    #include <wiringPi.h>
#endif

static bool soundButtonReq=false;
static bool call112ButtonReq=false;

void * w1ThreadFunc(void *arg)
{
   WiringPins *p=(WiringPins*)arg;
   p->runW1();
   return 0;
}

void * buttonsThreadFunc(void *arg)
{
   WiringPins *p=(WiringPins*)arg;
   p->runButtons();
   return 0;
}

void ButtonSoundplay_ISR(void)
{
    soundButtonReq=true;
}

void ButtonCall112_ISR(void)
{
   call112ButtonReq=true;
}

WiringPins::WiringPins(MainWindow *w):
    initCompletedFlag{false},path{"/sys/bus/w1/devices/"}
{
    setParent(w,Qt::Window);
    callstate=0;
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

      wiringPiSetup();
       /* Настройка вывода GPIO_0 на детекцию кнопки озвучки маршрутов по заднему фронту  */
      pinMode(PI_SOUND_PIN,INPUT);
      pullUpDnControl(PI_SOUND_PIN,PUD_UP);
      wiringPiISR(PI_SOUND_PIN,INT_EDGE_FALLING,ButtonSoundplay_ISR);

      /* Настройка вывода GPIO_1 на детекцию кнопки "вызова 112" по заднему фронту */
      pinMode(PI_CALL112_PIN,INPUT);
      pullUpDnControl(PI_CALL112_PIN,PUD_UP);
      wiringPiISR(PI_CALL112_PIN,INT_EDGE_FALLING,ButtonCall112_ISR);

      pinMode(PI_LEDBLINK_PIN,OUTPUT);
#endif

//      struct sched_param param;
//      pthread_attr_t attr;
//      pthread_attr_init(&attr);
//      pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
//      pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
//      param.sched_priority = 80;
//      pthread_attr_setschedparam(&attr, &param);
//      pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
      pthread_create(&w1Thread,NULL,w1ThreadFunc,this);
      pthread_create(&buttonsThread,NULL,buttonsThreadFunc,this);
}

WiringPins::~WiringPins()
{
    int ret=pthread_cancel(w1Thread);
    if(ret==0)
      pthread_join(w1Thread,NULL);
    ret=pthread_cancel(buttonsThread);
    if(ret==0)
      pthread_join(buttonsThread,NULL);
}

/*
 *
 */
void WiringPins::SendTempr(float *pTempr)
{
    RedrawMainWindow *ev=new RedrawMainWindow((QEvent::Type)(QEvent::User));
    ev->SendingMsg(RedrawMainWindow::UPDATE_TEMPR);
    ev->SendingData(pTempr);
    QApplication::postEvent(parent(),ev);
}
/*
 *
 */
void WiringPins::runW1(void)
{
  int unused;
#ifndef Q_OS_WIN
  char *temp;
  float value;
#endif

  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &unused);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,&unused);
  QFile file(path);

   while(1)
   {
#ifndef Q_OS_WIN
      w1_mutex.lock();
          file.open(QIODevice::ReadOnly);
          QByteArray arr=file.readAll();
          file.close();
      w1_mutex.unlock();
       // Returns the first index of 't'.
      temp = strchr(arr.data(),'t');
       // Read the string following "t=".
      sscanf(temp,"t=%s",temp);
       // atof: changes string to float.
      value = atof(temp)/1000;
      SendTempr(&value);
      flag=1;
      delay(800);
      flag=2;
      delay(200);
      flag=3;
#else
      sleep(1);
#endif
   }
}
/*
 *
 */
void WiringPins::runButtons(void)
{
  int unused;
  int state=0;

  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &unused);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,&unused);

   while(1)
   {
#ifndef Q_OS_WIN
      if(soundButtonReq==true)
      {
          delay(50);
          if(digitalRead(PI_SOUND_PIN)==LOW)
          {
            RedrawMainWindow *ev=new RedrawMainWindow((QEvent::Type)(QEvent::User));
            ev->SendingMsg(RedrawMainWindow::SOUND_BUTTON_PRESS);
            ev->SendingData(NULL);
            QApplication::postEvent(parent(),ev);
          }
          soundButtonReq=false;
      }
      if(call112ButtonReq==true)
      {
          delay(50);
          if(digitalRead(PI_CALL112_PIN)==LOW)
          {
            RedrawMainWindow *ev=new RedrawMainWindow((QEvent::Type)(QEvent::User));
            ev->SendingMsg(RedrawMainWindow::CALL112_BUTTON_PRESS);
            callstate ^=1;
            ev->SendingData(&callstate);
            QApplication::postEvent(parent(),ev);
          }
          call112ButtonReq=false;
      }

      digitalWrite(PI_LEDBLINK_PIN,state);
      state ^=1;
      delay(200);
#else
      sleep(1);
#endif
   }
}


