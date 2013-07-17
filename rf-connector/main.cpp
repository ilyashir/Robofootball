#define MAX_DEVICE 12

#include <QApplication>

#include "widget.h"

#include "Qsocket.h"

#include "BtPCModul.h"
#include <qdebug.h>
#include "filework.h"
#include "Transfer.h"

#include "Windows.h"

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   BtPCModul btmodul; //btmodul.FindRadio();
   if (!btmodul.FindRadio())
    {
        system("pause");
        return 0;
    }

    btmodul.GetPCInfo();
    NXTclass * nxtclass[1+MAX_DEVICE];
    for (int i=0; i<=MAX_DEVICE; i++)
        nxtclass[i]=new NXTclass(i);

    FileWork filework(nxtclass,MAX_DEVICE);
    filework.Read();

    for (int i=1; i<=MAX_DEVICE; i++)
    {
        printf("[%d]\n",i);
        btmodul.GetDeviceInfo(nxtclass[i]);
    }
    filework.Write();

    Transfer transfer(nxtclass);
    transfer.server=new Qserver("BTmodul",100,nxtclass,MAX_DEVICE);
    transfer.server->moveToThread(&transfer);
    transfer.start();

    qDebug()<<"Starting Widget...";
    Widget w(&btmodul,nxtclass,MAX_DEVICE,transfer.server);

    w.show();
    //Qserver.start();
    qDebug()<<"start";
//    return 0;
        return a.exec();
}
