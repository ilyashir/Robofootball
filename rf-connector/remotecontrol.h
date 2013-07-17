#ifndef REMOTECONTROL_H
#define REMOTECONTROL_H

#include <QThread>
class RemoteControl: public QThread
{
    int * pRobActNom;
public:
    RemoteControl(int * pRobActNom, QObject *parent=0);
protected:
    void run();
public:
    void send();
};

#endif // REMOTECONTROL_H
