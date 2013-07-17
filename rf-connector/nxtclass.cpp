#include "nxtclass.h"


NXTcom::NXTcom(QObject *parent)
    :QObject(parent)
{
    connected=0;
    com=0;
}
NXTcom::~NXTcom()
{
    Close();
}
bool NXTcom::Open()
{
    if (com)
    {
        connected=1;
        return 0;
    }
    QString Sport;
    Sport.setNum(port);
    com=new QextSerialPort(QString("//./COM").append(Sport));
    com->open(QIODevice::ReadWrite);
    if (com->isOpen())
    {
        connected=1;
        return 1;
    }
    else
    {
        Close();
        return 0;
    }

}

bool NXTcom::Close()
{
    connected=0;
    if (com)
    {
        com->close();
        com->disconnect();
        //com->reset();
        delete(com);
        com=0;
        return 1;
    }
    return 0;
}

bool NXTcom::Read(char * buf,int lenbuf)
{
    if ((!connected) ||(!com->isReadable()))
        return 0;
    return com->read(buf,lenbuf);
}

bool NXTcom::Write(char * buf,int lenbuf)
{
    if ((!connected) || (!com->isWritable()))
        return 0;
    char * mess=new char[lenbuf+2];
    memcpy(mess+2,buf,lenbuf);
    mess[0]=32;
    mess[1]=0;
    printf(mess+2);
    bool re=com->write(mess,lenbuf+2);
    delete mess;
    return re;
}


NXTclass::NXTclass(int Numder_,char * BTaddr_,int port_)
    :NXTcom()
{
    authenticated=false;
    Number=Numder_;
    Name[0]=0;
    if (BTaddr_)
        memcpy(BTaddr,BTaddr_,18);
    else
        memcpy(BTaddr,(char *)DEF_BT_ADDR,18);

    port=port_;
}
NXTclass::~NXTclass()
{

}
bool NXTclass::SetBTaddr(QString str)
{
    while (str[0]==' ')
        str.remove(0,1);
    for(int i=0; i<17; i++)
    {
        if ((i+1)%3)
        {
            if ( ((str[i]>='0')&&(str[i]<='9'))||
                 ((str[i]>='a')&&(str[i]<='f'))||
                 ((str[i]>='A')&&(str[i]<='F')) )
            {
                BTaddr[i]=str.toUtf8()[i];
            }
            else
            {
                memcpy(BTaddr,(char *)DEF_BT_ADDR,18);
                return 0;
            }
        }
        else
        {
            if (str[i]==':')
            {
                BTaddr[i]=':';
            }
            else
            {
                memcpy(BTaddr,(char *)DEF_BT_ADDR,18);
                return 0;
            }
        }
    }
    BTaddr[17]=0;
    return 1;
}

bool NXTclass::ZeroBTaddr()
{
    return memcmp(BTaddr,(char *)DEF_BT_ADDR,18);
}
