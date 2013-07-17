#ifndef TRANSFER_H
#define TRANSFER_H

#include <QObject>
#include <QThread>

#include "nxtclass.h"
/*
class NxtTransfer : public QThread
{
    Q_OBJECT
public:
    NXTcom * nxtcom;
public:
    NxtTransfer(NXTcom * nxtcom_,QObject * parent=0);
protected:
    void run();
public slots:
    bool Write(char * mess);
signals:
    bool Send(char * mess);
};*/

#include "qsocket.h"

class Transfer : public QThread
{
    Q_OBJECT
public:
    Qserver * server;
    NXTclass ** nxtclass;
public:
    explicit Transfer(NXTclass ** nxtclass_,QObject *parent = 0);
protected:
    void run();    
};

#endif // TRANSFER_H
