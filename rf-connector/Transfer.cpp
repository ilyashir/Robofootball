#include "Transfer.h"

Transfer::Transfer(NXTclass ** nxtclass_,QObject *parent)
    :nxtclass(nxtclass_),QThread(parent)
{
    server=0;
}

void Transfer::run()
{
    server->BAutoRead=true;
    server->DispLog=0;
    server->Connect();
    exec();
}
