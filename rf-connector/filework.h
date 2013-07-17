//---------------------------------------------------------
// Класс для работы с файлом содержащим параметры для BTмодуля.
// Формат файла:
//
// <Имя глобального параметра> = <Значение>
// [<(Цифра) Номер используемого робота для последующих значений>]
// <Имя параметра для робота> = <Значение>
//------------------------------------------------------------------
#ifndef FILEWORK_H
#define FILEWORK_H

#include <QFile>

#include "nxtclass.h"
#include "QString"

class FileWork:public QFile
{
private:
    int max_device;
    QString fileaddr;
    NXTclass ** nxtclass;
public:
    FileWork(NXTclass **nxtclass_,int max_device_,QString fileaddr_="Bots.ini");
    ~FileWork();
    void Read();
    void Write();
};

#endif // FILEWORK_H
