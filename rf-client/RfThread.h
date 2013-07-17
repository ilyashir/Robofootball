#ifndef RFTHREAD_H
#define RFTHREAD_H

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <process.h>
#include <QThread>
#include <QTime>
#include <QDir>

#include <QThread>
#include <QMutex>
#include "RfData.h"

#include "robocup_ssl_client.h"
#include "timer.h"

#include "messages_robocup_ssl_detection.pb.h"
#include "messages_robocup_ssl_geometry.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"

#define FRAPS 30

class RfThread : public QThread
{
public:
    explicit RfThread(RfData *rfdata);
    void run();
//    void setTargetWidget(QWidget *widget);
    int run_matlab();
    void stop_matlab();
    void client_open();
    char buffer[MESSAGE_SIZE*MAX_MESSAGE];
    int re;
    //-------Pack to Send_BT--------
    char pack[100];
    clock_t timeS;
    clock_t timerSend;
    int timecontrolS;
    clock_t timerSendControl;
    clock_t timerSendSSL;

private:
//  QWidget *targetWidget;
  QMutex mutex;
  RfData *currentRfData;

  ofstream logfile; // logfile

  char m_buffer[256];
  char sendString[256];
  volatile bool stopped;    // flag thread run
  volatile bool fmtlab;     // flag matlab run
  volatile bool fclient;    // flag open client
  volatile bool flog;       // flag logfile

  bool openlog();
  bool closelog();
  bool is_run();

public slots:
  void go();
  void brk();
  void rec();
  void stop();

};

#endif // RFTHREAD_H
