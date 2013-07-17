
#include "filework.h"
#include "QDebug"

#define MAX_DEVICE max_device

FileWork::FileWork(NXTclass ** nxtclass_, int max_device_,QString fileaddr_)
{
    max_device=max_device_;
    fileaddr=fileaddr_;
    nxtclass=nxtclass_;
}

FileWork::~FileWork()
{
    Write();
}
void FileWork::Read()
{
    QFile file(fileaddr);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    //QTextStream out(&file);   //out << temp;    //out << "\n";
    int control=0,ieq=0;
    QString str;

    if (file.isReadable())
    {
        while (!file.atEnd())
        {
            str=file.readLine(1000);
            //qDebug()<<str;

            if ( (0==str.indexOf("[")))
            {
                int it;
                if ((-1)!=(it=str.indexOf("]")))
                    control=(str.mid(1,it-1)).toInt();
                if (control<=0 || control>MAX_DEVICE)
                    control=0;
                qDebug()<<"["<<control<<"]";
            }

            if ( control && (0==str.indexOf("COMport")) && ((-1)!=(ieq=str.indexOf('='))))
            {
                nxtclass[control]->port=(str.mid(ieq+1)).toInt();
                qDebug()<<"    COMport"<<"= "<<nxtclass[control]->port;//
            }

            if ( control && (0==str.indexOf("BTaddr")) && ((-1)!=(ieq=str.indexOf('='))))
            {
                while (str[ieq+1]==' ') ieq++;
                if (nxtclass[control]->SetBTaddr(str.mid(ieq+1)))
                   qDebug()<<"    BTaddr"<<" = "<<nxtclass[control]->BTaddr;
            }
            if ( control && (0==str.indexOf("ProgAddr")) && ((-1)!=(ieq=str.indexOf('='))))
            {
                while (str[ieq+1]==' ') ieq++;
                nxtclass[control]->ProgAddr=str.mid(ieq+1);
                nxtclass[control]->ProgAddr.remove(-1,1);
                   qDebug()<<"    ProgAddr"<<" = "<<nxtclass[control]->ProgAddr;
            }
        }
    }
    file.close();
}


void FileWork::Write()
{
    QFile file(fileaddr);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    for (int i=1; i<=MAX_DEVICE; i++)
    {
        if (nxtclass[i]->ZeroBTaddr())
        {
            out<<'['<<i<<"]\n";
            out<<"BTaddr = "<<nxtclass[i]->BTaddr<<'\n';
            if (nxtclass[i]->port)
                out<<"COMport = "<<nxtclass[i]->port<<'\n';
            if (!nxtclass[i]->ProgAddr.isNull())
                out<<"ProgAddr = "<<nxtclass[i]->ProgAddr<<'\n';
        }
    }
    file.close();
}
