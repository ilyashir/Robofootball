#include "qsocket.h"

#define MAX_DEVICE max_device

Qserver::Qserver(char * name_,int messageSize_,NXTclass ** nxtclass_,int max_device_,QObject *parent)
    :QObject(parent),nxtclass(nxtclass_),messageSize(messageSize_)
{
    max_device=max_device_;
    BAutoRead=0;
    DispLog=1;
    socket = 0;
    server = 0;
    name=new char[strlen(name_)+1];
    memcpy(name,name_,strlen(name_)+1);    
}

bool Qserver::Read(char *buf)
{
    if ((socket)&&(socket->isReadable()))
    {
        if (DispLog) printf("Read : ");
        qint64 re=socket->read(buf,messageSize);
        if (re>0)
        {
            if (messageSize!=re)
                printf("WARING! socket insufficient size buffer by read %d\n",re);
            if (DispLog) qDebug()<<buf;
            return 1;
        }
        else
        {
            if (DispLog) printf("Error_read_socket %d\n",re);
            return 0;
        }
    }
    else
    {
        if (DispLog) printf("Error, socket is not Readable\n");
        return 0;
    }
}

bool Qserver::Write(char * buf)
{
    if ((socket)&&(socket->isWritable()))
    {
        if (DispLog) printf("Wtire : ");
        qint64 re=socket->write(buf,messageSize);
        if (re>0)
        {
            if (messageSize!=re)
                printf("WARING! socket insufficient size buffer by write %d\n",re);
            else if (DispLog) printf("OK\n");
            return 1;
        }
        else
        {
            if (DispLog) printf("Error_socket_write %d\n",re);
            return 0;
        }
    }
    else
    {
        if (DispLog) printf("Error, socket is not Writable\n");
        return 0;
    }
}

//Включение\отключение автоматического приёма сообщений
bool Qserver::ActAutoRead(bool Act)
{
    if (socket)
    {
        if (Act)
        {
            return connect(socket, SIGNAL(readyRead()),this, SLOT(MessageParsing()));
        }
        else
        {
            return disconnect(socket, SIGNAL(readyRead()),this, SLOT(MessageParsing()));
        }
    }
    else return 0;
}

//Разбор сообщения
//buf[0] - соманда (1- пересылать на устройства, 2 - для БТмодуля)
//buf[1] - номер робота-адресата
bool Qserver::MessageParsing()
{
    char * buf=new char[messageSize];
    while (Read(buf))
    {
        if (buf[0]==1)
        {
            if ((buf[1]>0)&&(buf[1]<=MAX_DEVICE))
            {
                nxtclass[buf[1]]->Write(buf+2);
               // qDebug()<<"NewMess "<<(int)buf[1];
            }
            if (buf[1]==0)
                for(int i=1; i<=MAX_DEVICE; i++)
                    nxtclass[i]->Write(buf+2);
        }
    }  
//    emit(NewChange());
    return 1;
}


//-------
//Server
//-------

void Qserver::Connect()
{
    server = new QLocalServer(this);

    connect(server, SIGNAL(newConnection()),
            this, SLOT(NewConnection()));

    if (!server->listen(name))
        printf("server : ERROR LISTEN \n");
    else
        printf("server : Server is OK \n");
}

void Qserver::NewConnection()
{
    if (!socket)
    {
        printf("server : New Connection , BAutoRead = %s\n",BAutoRead ? "true" : "false");
        socket=server->nextPendingConnection();
        connect(socket, SIGNAL(disconnected()),
                this, SLOT(ClearConnect()));
        ActAutoRead(BAutoRead);   
    }
    emit(NewChange());
}
void Qserver::ClearConnect()
{    
    ActAutoRead(0);
    socket->close();
    socket=0;
    printf("server : socket Disconnected\n");
    emit(NewChange());
}


bool Qserver::IsConnected()
{
    return(socket>0);
}
