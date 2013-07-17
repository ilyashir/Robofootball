#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "ui_widget.h"

#include "qdebug.h"
#include "BtPCModul.h"
#include "nxtclass.h"
#include "Qsocket.h"

#include <QKeyEvent>
namespace Ui
{
  class Widget;
}

class Widget : public QWidget
{
  Q_OBJECT
public:
  BtPCModul * btmodul;
  NXTclass ** nxtclass;
  Qserver * qserver;
  //RemoteControl * RemControl;
private:
  Ui::Widget *ui;
  int max_device;
  int RobActNom;

  //---INI---
public:
  explicit Widget(BtPCModul * btmodul_,NXTclass ** nxtclass_,int max_device_,Qserver *qserver_,QWidget *parent = 0);
  ~Widget();

  //---sys---
private:
  void refresh();
private slots:
  void NewChange();
  void on_comboBox_activated(int index);

  //---AUT---
private slots:
  void on_B_AUT_clicked();

  //---Search
private slots:
  void on_B_SEARCH_clicked();
  void on_listWidget_doubleClicked(const QModelIndex &index);
  void on_Search_back_clicked();
  void SearchingNewDevice(); //out
  //---com---
private slots:
  void on_B_Connect_clicked();
  void on_B_Disconnect_clicked();
  void on_B_Send_clicked();

  /*
  void on_pushButton_4_clicked();

  void on_pushButton_5_clicked();

  void on_pushButton_6_clicked();

  void on_pushButton_clicked();

  void on_pushButton_11_clicked();

  void on_pushButton_12_clicked();
//private slots:
//private slots:

 // void Widget::keyReleaseEvent(QKeyEvent * key);

//  void on_pushButton_18_clicked();

//public slots:

  void on_B_RC_back_clicked();
  void on_CB_RC_port_activated(const QString &arg1);
  void on_B_RC_sensor_start_clicked();
  void on_B_remcontrol_clicked();
  */
};

#endif // WIDGET_H
