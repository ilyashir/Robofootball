//���������� ���������

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
Device R[MAX_DEVICE+1]; //������ ��� NXT ������������
#include "PipeChannel.h"
#include "fileWork.h"
BTconnect MyBTdevice; //����� ��� BlueTooth ����������
#include "BtForm.h"

[STAThreadAttribute]
void main()
{

	if (!MyBTdevice.FindRadio()) //���������� ���������� ���������� �� ����������
	{
		printf("ERROR!!! Testing you blueTooth apparatus!\n");
		system("pause");return ;
	}
	MyBTdevice.GetInfo(); //������� ���������� � ���������� BlueTooth �� ���������
	FileWork File("Bots.ini",R); //����� ��� ���-�����
	File.Read();//������� ��������� �������� �� ���-�����
	File.TestData(); //�������� ������������� � *�� ����������* ����������
	for (int i=0; i<=MAX_DEVICE;i++) //����� ���������� � �����������
	{
		R[i].Number=i;
		if (R[i].activ)
		{
			cout<<"device "<<i<<endl;
			R[i].GetInfo();
		}
	}
	
	ActSending();  //��������� �������� BT �������
	//ActRecving();  //��������� �������� BT ������� 
	ActRecvPipe(); //��������� �������� Pipe
		
	HWND console = GetConsoleWindow();
	ShowWindow(console, SW_HIDE);

	FORM::Application::EnableVisualStyles();
	FORM::Application::SetCompatibleTextRenderingDefault(false); 
	// �������� �������� ���� � ��� ������
	FORM::Application::Run(gcnew FORM::Form1());
	// ���������� ����������
	File.write();
	return ;
}