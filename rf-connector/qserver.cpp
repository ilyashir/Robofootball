#include "qserver.h"

QServer::QServer(char * name_,int messageSize_,QObject *parent)
    :QObject(parent)
{
    DispLog=1;
    name=new char[strlen(name_)+1];
    memcpy(name,name_,strlen(name_)+1);
    messageSize=messageSize_;

    server = new QLocalServer(this);
    connect(server, SIGNAL(newConnection()),
            this, SLOT(NewConnection()));

    if (!server->listen(name))
        qDebug()<<"ERROR LISTEN";
    else
        qDebug()<<"Server is OK";

    socket=0;
}

void QServer::NewConnection()
{
    qDebug()<<"---Servers connect Thread"<<currentThreadId();

    qDebug("server : New Connection");
    if (!socket)
    {
        socket=server->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()),transp, SLOT(Trslot()));
        connect(socket, SIGNAL(disconnected()),
                this, SLOT(ClearConnect()));

    }
}
void QServer::ClearConnect()
{
    disconnect(socket, SIGNAL(readyRead()),transp, SLOT(Trslot()));

    socket->abort();
    socket->close();
    socket=0;
    qDebug()<<"socket Disconnected";
}

bool QServer::Read(char *buf)
{
    if ((socket)&&(socket->isReadable()))
    {
        qint64 re=socket->read(buf,messageSize);
        if (DispLog) printf("Read : ");
        if (re>=0)
        {
            if (messageSize!=re)
                printf("WARING! socket insufficient size buffer by read\n");
            if (DispLog) qDebug()<<buf;
            return 1;
        }
        else
        {
            printf("Error_read_socket\n");
            return 0;
        }
    }
    else
    {
        printf("Error, socket is not Readable\n");
        return 0;
    }
}
bool QServer::Write(char * buf)
{
    if ((socket)&&(socket->isWritable()))
    {
        if (DispLog) printf("Wtire : ");
        qint64 re=socket->write(buf,messageSize);
        if (re>=0)
        {
            if (messageSize!=re)
                printf("WARING! socket insufficient size buffer by write\n");
            else if (DispLog) printf("OK\n");
            return 1;
        }
        else
        {
            printf("Error_socket_write\n");
            return 0;
        }
    }
    else
    {
        printf("Error, socket is not Writable\n");
        return 0;
    }
}
