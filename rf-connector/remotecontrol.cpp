#include "remotecontrol.h"

RemoteControl::RemoteControl(int * pRobActNom_,QObject *parent)
:pRobActNom(pRobActNom_),QThread(parent)
{

}
#include <QAbstractButton>
#include <qdebug.h>
#include <Windows.h>
void RemoteControl::run()
{
    int L,R,S,K,P;
    char buf[32]="LRKBS";
    while(1)
    {
        /*
        if (GetAsyncKeyState('W'))
        {
            L+=50;
            R+=50;
        }
        if (GetAsyncKeyState('S'))
        {
            L-=50;
            R-=50;
        }
        if (GetAsyncKeyState('A'))
        {
            L-=25;
            R+=25;
        }
        if (GetAsyncKeyState('D'))
        {
            L+=25;
            R-=25;
        }
        if (GetAsyncKeyState(VK_SPACE)<0||GetAsyncKeyState('Q'))
            K=1;
        if (GetAsyncKeyState('E'))
            K=-1;
        if (GetAsyncKeyState('Z'))
            P=1;
        if (GetAsyncKeyState(VK_SHIFT)<0)
        {
            L=L*2;
            R=R*2;
            if (abs(L)>100) L=L/abs(L)*100;
            if (abs(R)>100) R=R/abs(R)*100;
        }
        qDebug()<<L<<" "<<R<<" "<<K<<P<<S<<"\n";
        */buf[0]=L;
        buf[1]=R;
        buf[2]=K;
        buf[3]=P;
        buf[4]=S;


        sleep(1);
    }
}

void RemoteControl::send()
{

}
