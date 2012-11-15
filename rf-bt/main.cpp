//Глобальные параметры

char * PIPE_NAME="BtModul";
int MESSAGE_SIZE=100;
int MAX_MESSAGE=1000;
int MAX_SAVE_LETTONS=10;
int NXT_MESS_SIZE=32;
#define MAX_DEVICE 12

#include <stdio.h>
#include <iostream>
using namespace std;


	bool SearchStrInStr(char * a,char * b)
	{
		unsigned int ia=0, ib=0; 
		while ( (b[ib]!=0) && (a[ia]!=0) )
		{
			if (a[ia]==b[ia+ib])
			{
				ia++;
			}
			else
			{
				ia=0; ib++;
			}
		}
		return (a[ia]==0);
	}




#include "BTDeviceClass.h"
#include "NXTClass.h"
Device R[MAX_DEVICE+1]; //Классы для NXT исполнителей
#include "PipeChannel.h"
#include "fileWork.h"
BTconnect MyBTdevice; //Класс для BlueTooth устройства
#include "BtForm.h"

[STAThreadAttribute]
void main()
{

	if (!MyBTdevice.FindRadio()) //Подключить подходящее устройство на компьютере
	{
		printf("ERROR!!! Testing you blueTooth apparatus!\n");
		system("pause");return ;
	}
	MyBTdevice.GetInfo(); //Собрать информацию с устройства BlueTooth на копьютере
	FileWork File("Bots.ini",R); //Класс для ини-файла
	File.Read();//Считать параметры девайсов из ини-файла
	File.TestData(); //Проверка достоверности и *Не устаревших* параметров
	for (int i=0; i<=MAX_DEVICE;i++) //Выыод информации о устройствах
	{
		R[i].Number=i;
		if (R[i].activ)
		{
			cout<<"device "<<i<<endl;
			R[i].GetInfo();
		}
	}
	
	ActSending();  //Активация отправки BT пакетов
	//ActRecving();  //Активация принятия BT пакетов 
	ActRecvPipe(); //Активация принятия Pipe
		
	HWND console = GetConsoleWindow();
	ShowWindow(console, SW_HIDE);

	FORM::Application::EnableVisualStyles();
	FORM::Application::SetCompatibleTextRenderingDefault(false); 
	// Создание главного окна и его запуск
	FORM::Application::Run(gcnew FORM::Form1());
	// Сохранение параметров
	File.write();
	return ;
}