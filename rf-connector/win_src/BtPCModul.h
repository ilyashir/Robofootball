//-------------------------------------------------------
//Класс для BlueTooth устройства на компьютере
//поиск, инициализация устройств, сбор данных о них.
//-------------------------------------------------------
#ifndef BTPCMODUL_H
#define BTPCMODUL_H


#include "..\nxtclass.h"

#include <Winsock2.h>
//#include "remotecontrol.h"
#define BLUETOOTH_MAX_SIZE_NAME 248 //BLUETOOTH_MAX_NAME_SIZE

#include <QThread>

#define NXT_GUID {4353,268435456,-2147483520,-80438433};

#ifndef QPRINTF_H
  #define QPRINTF_H
  #include "qdebug.h"
#endif //QPRINTF_H

class BtPCModul: public QThread
{
    Q_OBJECT
public:
    BtPCModul(QObject *parent=0);
public:
    PWSTR pin; //Стандартный Пин-код 1234
    HANDLE m_radio; //Указатель Bt компьютера.
public:
    bool FindRadio(); //Найти драйвер BlueTooth на компьютере
    void GetPCInfo(); //Сбор данных о компьютере
public:
        //Структуры памяти параметров найденных устройств
        //BLUETOOTH_ADDRESS SaveAddr[100];
        char BTaddr[100][18];
        char SaveName[100][BLUETOOTH_MAX_SIZE_NAME];
        int m_device_id;
        void Find_Device(int time);//Функция поиска устройств
    protected:
        void run(); //Запуск поиска устройств в цикле
    signals:
        void NewDevice(); //сигнал найденного нового устройства
public:
    bool Authenticating(NXTclass *nxtclass);//Запрос авторизации
    bool GetDeviceInfo(NXTclass *nxtclass); //сбор параметров с устройства
    bool InstalDriver(NXTclass * nxtclass); //Установка драйвера
    int PortSearch(NXTclass * nxtclass); //Поиск порта в реестре
    bool ComTest(NXTclass * nxtclass);
};
#endif // BTMODUL_H
