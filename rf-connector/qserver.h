#ifndef QSERVER_H
#define QSERVER_H

#include "QThread"
#include <qlocalsocket.h>
#include <qlocalserver.h>

#include "QDebug"
class Transp:public QObject
{
    Q_OBJECT
public:
public slots:
    void Trslot()
    {
        qDebug()<<"Transp_signal"<<QThread::currentThreadId();
    }

};

class QServer:public QThread
{
    Q_OBJECT
public:
    Transp * transp;
private:
    bool DispLog;
    QLocalSocket *socket;
    char * name;
    int messageSize;
    QLocalServer *server;
public:
    explicit QServer(char * name_,int messageSize_,QObject *parent=0);
protected:
    void run();
private slots:
    void NewConnection();
    void ClearConnect();
public:
    bool Read(char * buf);
    bool Write(char * buf);
};

#endif // QSERVER_H
