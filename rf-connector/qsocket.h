//-----------------------------------------------------
//Класс SOCKET. инициализирующий и обслуживающий.
//Автоматически принимает сообщения поступающие из пайпа
//И передаёт их на отправку роботам.
//-----------------------------------------------------
#ifndef QSOCKET_H
#define QSOCKET_H

#include <QThread>
#include <qlocalsocket.h>
#include <qlocalserver.h>

#include "nxtclass.h"

class Qserver:public QObject
{
    Q_OBJECT
protected:
    char * name;
    int messageSize;
    int max_device;
public:
    QLocalSocket *socket;
    QLocalServer *server;
public:
    NXTclass ** nxtclass;
    bool DispLog;
public:
    explicit Qserver(char * name_,int messageSize_,NXTclass ** nxtclass_,int max_device_,QObject *parent = 0);
public:
    void Connect(); //Инициализация сервера
    bool Write(char * buf);
    bool Read(char * buf);
public:
    bool BAutoRead; //Включать или нет авто-чтение?
    bool ActAutoRead(bool Act=1); //Включение авто-чтения.
private slots:
    bool MessageParsing(); //Auto
    void NewConnection(); //server - новое подключение
    void ClearConnect(); //server - отключение
signals:
    void NewChange();
public:
    bool IsConnected();
};


#endif // QSOCKET_H
