//#include <QGLWidget.h>
#include "widget.h"
#include "ui_widget.h"

#include "Qstring"

#include <QDebug>
#include <QTimer>

#define MAX_DEVICE max_device
//--------------------INI----------------
Widget::Widget(BtPCModul * btmodul_,NXTclass ** nxtclass_,int max_device_,Qserver *qserver_, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Widget)
{
    qserver=qserver_;
    max_device=max_device_;
//    MAX_DEVICE=;
    nxtclass=nxtclass_;
    btmodul=btmodul_;
    RobActNom=1;
    ui->setupUi(this);
//RemControl
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(RC_send()));
    for(int i=0; i<256; i++)
        keys[i]=0;
    key_shift=0;

    connect(btmodul,SIGNAL(NewDevice()),this,SLOT(SearchingNewDevice()));
    connect(qserver,SIGNAL(NewChange()),this,SLOT(NewChange()));
    ui->GB_search->setHidden(1);
    ui->GB_search->setGeometry(0,0,300,200);
    this->setGeometry(300,300,300,200);
    ui->GB_remote->setHidden(1);
    ui->GB_remote->setGeometry(0,0,300,200);
    refresh();
}

Widget::~Widget()
{
  delete ui;
}
//---------------------sys----------------------------------
void Widget::NewChange()
{
    refresh();
}
void Widget::on_comboBox_activated(int index)
{
  RobActNom=index+1;
  qDebug()<<RobActNom;
  btmodul->GetDeviceInfo(nxtclass[RobActNom]);
  refresh();
}

void Widget::refresh()
{
    ui->lineEdit->setText(nxtclass[RobActNom]->BTaddr);
    ui->lineEdit_2->setText(QString().setNum(nxtclass[RobActNom]->port));
    ui->checkBox->setChecked(nxtclass[RobActNom]->authenticated);
    ui->C_Connect->setChecked(nxtclass[RobActNom]->connected);
    ui->B_Send->setEnabled(nxtclass[RobActNom]->authenticated && (nxtclass[RobActNom]->port>0));
    ui->B_Connect->setEnabled(nxtclass[RobActNom]->authenticated && (nxtclass[RobActNom]->port>0));
    ui->B_Disconnect->setEnabled(nxtclass[RobActNom]->authenticated && (nxtclass[RobActNom]->port>0));

    ui->B_remcontrol->setEnabled(nxtclass[RobActNom]->connected);
    ui->C_pipe->setChecked(qserver->IsConnected());
    {
        ui->comboBox->clear();
        char temp[BLUETOOTH_MAX_SIZE_NAME+3];
        for(int i=1; i<=MAX_DEVICE;i++)
        {
            ZeroMemory(temp,BLUETOOTH_MAX_SIZE_NAME+3);
            memcpy(temp+3,nxtclass[i]->Name,BLUETOOTH_MAX_SIZE_NAME);
            if (i<10)
            {
                temp[0]='0'+i;
                temp[1]=' ';
                temp[2]=' ';
            }
            else
            {
                temp[0]='0'+(i/10)%10;
                temp[1]='0'+i%10;
                temp[2]=' ';
            }
            ui->comboBox->addItem(temp);
        }
        ui->comboBox->setCurrentIndex(RobActNom-1);
    }
    repaint();
}

//-----------------AUT-------------------------
void Widget::on_B_AUT_clicked()
{
    btmodul->Authenticating(nxtclass[RobActNom]);
    btmodul->InstalDriver(nxtclass[RobActNom]);
    btmodul->GetDeviceInfo(nxtclass[RobActNom]);
    btmodul->PortSearch(nxtclass[RobActNom]);
    refresh();
}
//--------------------------------------------
//                  Search
//--------------------------------------------
void Widget:: SearchingNewDevice()
{
    ui->listWidget->clear();
    for(int i=0; i<btmodul->m_device_id; i++)
    {
        ui->listWidget->addItem(QString(btmodul->BTaddr[i]).append(" - ").append(btmodul->SaveName[i]));
    }
    repaint();
}

void Widget::on_B_SEARCH_clicked()
{
    ui->GB_search->setHidden(0);
    ui->GB_ost->setHidden(1);
    SearchingNewDevice();
    btmodul->start();
}
void Widget::on_Search_back_clicked()
{
    btmodul->terminate();
    ui->GB_search->setHidden(1);
    ui->GB_ost->setHidden(0);
    refresh();
}
void Widget::on_listWidget_doubleClicked(const QModelIndex &index)
{
    int N=*((int*)(&index));
    memcpy((nxtclass[RobActNom])->Name,btmodul->SaveName[N],BLUETOOTH_MAX_SIZE_NAME);
    memcpy((nxtclass[RobActNom])->BTaddr,btmodul->BTaddr[N],18);
    btmodul->GetDeviceInfo(nxtclass[RobActNom]);  
    btmodul->PortSearch(nxtclass[RobActNom]);
    on_Search_back_clicked();
}
//----------------------------------------------
//                      com
//----------------------------------------------
void Widget::on_B_Connect_clicked()
{
    nxtclass[RobActNom]->Open();
    refresh();
}

void Widget::on_B_Disconnect_clicked()
{
    nxtclass[RobActNom]->Close();
    refresh();
}

void Widget::on_B_Send_clicked()
{
    qDebug()<<nxtclass[RobActNom]->Write(ui->lineEdit_3->text().toUtf8().data());
    refresh();
}
//-----------------------------------------------
//              remotecontrol
//-----------------------------------------------
void Widget::on_B_RC_back_clicked()
{
    ui->GB_remote->setHidden(1);
    ui->GB_ost->setHidden(0);
    timer->stop();
    refresh();
}
void Widget::on_B_remcontrol_clicked()
{
    ui->GB_remote->setHidden(0);
    ui->GB_ost->setHidden(1);
    this->setFocus();
    timer->start(30);
    refresh();
}


void Widget::keyPressEvent(QKeyEvent * key)
{
    if (key->key()<256)
        keys[key->key()]=1;
    if (key->key()==Qt::Key_Escape)
    {
        on_B_RC_back_clicked();
    }
    if (key->key()==Qt::Key_Shift)
    {
        key_shift=1;
    }
}

void Widget::keyReleaseEvent(QKeyEvent * key)
{
    if (key->key()<256)
        keys[key->key()]=0;
    if (key->key()==Qt::Key_Shift)
    {
        key_shift=0;
    }
}


void Widget::RC_send(void)
{
    char buf[32]="       ";
    int L=0,R=0,S=0,K=0,P=0;
    if (keys['W'])
    {
        L+=50;
        R+=50;
    }
    if (keys['S'])
    {
        L-=50;
        R-=50;
    }
    if (keys['A'])
    {
        L-=25;
        R+=25;
    }
    if (keys['D'])
    {
        L+=25;
        R-=25;
    }
    if (keys[' ']||keys['Q'])
        K=1;
    if (keys['E'])
        K=-1;
    if (keys['Z']||keys['B'])
        P=1;
    if (key_shift)
    {
        L=L*2;
        R=R*2;
        if (abs(L)>100) L=L/abs(L)*100;
        if (abs(R)>100) R=R/abs(R)*100;
    }
    qDebug()<<L<<" "<<R<<" "<<K<<P<<S<<endl;
    buf[0]=L;
    buf[1]=R;
    buf[2]=K;
    buf[3]=P;
    buf[4]=S;
    nxtclass[RobActNom]->Write(buf);
}
//-----------------------------------------------
//              test
//-----------------------------------------------
/*
void Widget::on_pushButton_clicked()
{
    qDebug()<<"ARA";
}


void Widget::on_pushButton_4_clicked()
{
    refresh();
}

void Widget::on_pushButton_5_clicked()
{
    char buf[100];
    if (qserver->Read(buf))
        qDebug()<<buf;
}
#include"Windows.h"
void Widget::on_pushButton_6_clicked()
{
    qDebug()<<"Pause";

   Sleep(3000);
    qDebug()<<"UnPause";
}
void Widget::on_pushButton_11_clicked()
{
    ui->listWidget->setHidden(1);
    ui->Search_back->setHidden(1);
}

void Widget::on_pushButton_12_clicked()
{
    ui->listWidget->setHidden(0);
    ui->Search_back->setHidden(0);
}

//-----------------TEST-----------------

void Widget::on_pushButton_18_clicked()
{
   ui->GB_remote->setHidden(!ui->GB_remote->isHidden());
}*/
//-------------------------------------
//     remote control
//-------------------------------------
/*
void Widget::on_B_RC_back_clicked()
{
    RemControl->terminate();
}

void Widget::on_CB_RC_port_activated(const QString &arg1)
{

}

void Widget::on_B_RC_sensor_start_clicked()
{
    qDebug("focus");
    this->setFocus();
}

void Widget::on_B_remcontrol_clicked()
{
                       bool k=this->hasFocus();

                       qDebug()<<k;
}

void Widget::keyReleaseEvent(QKeyEvent * key)
{
    qDebug("key");
    bool k=this->hasFocus();
    this->
    qDebug()<<k;
    //enum keystroke_type key_type;
    switch( key->key() )
    {
    case Qt::Key_Left:
        qDebug()<<"left";
        break;
    case Qt::Key_Right:
        qDebug()<<"right";
        break;
    case Qt::Key_Up:
        qDebug()<<"up";
        break;
    case Qt::Key_Down:
        qDebug()<<"down";
        break;
    }
}
*/
//-------------------------------------------------------
//
//-------------------------------------------------------


