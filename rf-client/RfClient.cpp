#include "RfClient.h"

RfClient::RfClient(RfThread *thread_, RfData *rfdata_, QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget())
{    

    drawMutex = new QMutex();
    //setFixedSize(400, 200);

    thread=thread_;
    rfdata=rfdata_;
    try{
        ui->setupUi(this);
    }catch(exception& e) {
        cout << "error!" << endl;
        cout << e.what() << endl;

    }

    soccerScene = new SoccerScene();
    ui->view->setScene(soccerScene);
    viewThread = new ViewUpdateThread(soccerScene, drawMutex);
    viewThread->start(QThread::NormalPriority);

    connect(viewThread, SIGNAL(updateView()), this, SLOT(updateView()));
    connect(ui->sceneSlider, SIGNAL(valueChanged(int)), soccerScene, SLOT(setSizeScene(int)));

//    QPushButton *quit = new QPushButton(tr("Quit"), this);
//    //quit->setGeometry(62, 40, 75, 30);
//    quit->setFont(QFont("Times", 18, QFont::Bold));

//    QPushButton *go = new QPushButton(tr("&GO"));
//    go->setFont(QFont("Times", 18, QFont::Bold));

//    QPushButton *brk = new QPushButton(tr("&Break"));
//    brk->setFont(QFont("Times", 18, QFont::Bold));

//    QPushButton *rec = new QPushButton(tr("&REC"));
//    rec->setFont(QFont("Times", 18, QFont::Bold));

//    QPushButton *stop = new QPushButton(tr("&STOP"));
//    stop->setFont(QFont("Times", 18, QFont::Bold));

////    connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
//  //  connect(go, SIGNAL(clicked()), &thread, SLOT(go()));
//    //connect(brk, SIGNAL(clicked()), this, SLOT(exec()));
// //   connect(rec, SIGNAL(clicked()), &thread, SLOT(rec()));
// //   connect(stop, SIGNAL(clicked()), &thread, SLOT(stop()));

//    QHBoxLayout *topLayout = new QHBoxLayout;
//    topLayout->addWidget(go);
//    topLayout->addWidget(brk);
//    topLayout->addStretch(1);

//    QGridLayout *gridLayout = new QGridLayout;
//    gridLayout->addWidget(quit, 0, 0);
//    gridLayout->addLayout(topLayout, 0, 1);
//    gridLayout->addWidget(rec, 1, 0);
//    gridLayout->addWidget(stop, 1, 1);
//    gridLayout->setColumnStretch(1, 10);
//    setLayout(gridLayout);

//    startTimer(1000);
}

RfClient::~RfClient()
{
  thread->terminate();
  delete ui;
  while(thread->isRunning())
      ;
}

void RfClient::updateView()
{
//  static float lastScale = 0;
//  if ( shutdownSoccerView )
//  {
//    return;
//  }
////  if ( scalingRequested )
////  {
////    qreal factor = matrix().scale ( drawScale, drawScale ).mapRect ( QRectF ( 0, 0, 1, 1 ) ).width();
////    if ( factor > 0.07 && factor < 100.0 )
////      scale ( drawScale, drawScale );
////    scalingRequested = false;
////  }
//  //causes segfaults, update-queue is evil
//  //*** glibc detected *** PATH/ssl-vision-read-only/bin/graphicalClient: malloc(): memory corruption (fast): 0xb15682f2 ***

//  //  Program received signal SIGSEGV, Segmentation fault.
//  //  [Switching to Thread 0xb494ab90 (LWP 13615)]
//  //  0xb766db63 in QRegion::operator+= () from /usr/lib/libQtGui.so.4
    ui->view->viewport()->update();
//  this->viewport()->update();
}

//int RfClient::bindPipe(){

//    cout << "bind!" << endl;

//    if (BtClient.Connect())
//    {
//        BtClient.DispLog=false;
//    }
//    else
//        return -1;

//    return 0;

//}

//void RfClient::exec(){

////    if(bindPipe()) {
////        cerr << "BT Server daemon not started." << endl;
////        return;
////    }

////    cout << "bind ok" << endl;

//}


//void RfClient::start()
//{

//}

//void RfClient::stop()
//{

//}




