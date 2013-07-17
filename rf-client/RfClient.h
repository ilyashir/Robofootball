#ifndef RFCLIENT_H
#define RFCLIENT_H

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <process.h>
#include <QThread>
#include <QTime>
#include <QDir>

#include "robocup_ssl_client.h"
#include "timer.h"

#include <QApplication>
#include <QFont>
#include <QPushButton>
#include <QWidget>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <exception>

#include "messages_robocup_ssl_detection.pb.h"
#include "messages_robocup_ssl_geometry.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"

#include "RfData.h"
#include "RfThread.h"
#include "GraphicsPrimitives.h"
#include "ClientThreading.h"
#include "SoccerScene.h"
#include "ui_widget.h"


using namespace std;

namespace Ui
{
  class Widget;
}

class RfClient : public QWidget
{

    Q_OBJECT

protected:

    RfThread * thread; // new thread
    RfData * rfdata; // new rf data

//    int bindPipe();

//    void daemonize();
//    void mainloop();
//    void commit();

public:

    RfClient(RfThread* thread_, RfData* rfdata_, QWidget *parent = 0);
    ~RfClient();

    QFrame *Spielfeld;
    QGridLayout *gridLayout_7;
    QGridLayout *gridLayout;
    SoccerScene* soccerScene;

    QMutex* drawMutex;
    ViewUpdateThread* viewThread;



private slots:
    void updateView();
//    void exec();
//    void start();
//    void stop();

private:
  Ui::Widget *ui;

};

#endif // RFCLIENT_H
