#include "Pipe.h"
#define ACK_MESG_RECV "Message received successfully\n"
#define ACK_LOSS_MESSAGE "WORRY!!! Maby loss messages or stopped serer!!!!!\n"
#include <process.h>

// 0 - ������������ ���� �1: ������������ �� ���������(1) ��� (0) ��� (2-��� ������)
// 1 - ����� ������

int NOMERATIONb;
Pipe_Server * pPipe;

//������� ���������
class LETTONS
{
	struct Letton
	{
		char * buf;
		Letton * next;
		Letton(char * buf_)
		{
			next=0;
			buf=new char[NXT_MESS_SIZE];
			memcpy(buf,buf_,NXT_MESS_SIZE);
		}
		~Letton()
		{
			delete(buf);
		}
	};

	Letton * LetBegin;
	Letton * LetEnd;
	
public:
	bool overlap;
	int Nlet;
	LETTONS()
	{
		overlap=0;
		Nlet=0;
		LetBegin=0;
		LetEnd=0;
	}
	bool add(char * buf)
	{
		while(overlap);
		overlap=1;
		if (Nlet>=MAX_SAVE_LETTONS) 
		{
			
			overlap=0;
			return false;
		}
		Nlet++;
		if (LetBegin==0)
		{
			Letton * save=new Letton(buf);
			LetBegin=save;
			LetEnd=LetBegin;
		}
		else
		{
			LetEnd->next=new Letton(buf);
			LetEnd=LetEnd->next;
		}
		overlap=0;
		return true;
	}
	bool del()
	{
		while(overlap);
		overlap=1;
		if (!LetBegin) 
		{
			overlap=0;
			return 0;
		}
		Nlet--;
		Letton * LetSave=LetBegin;
		LetBegin=LetBegin->next;
		delete(LetSave);
		overlap=0;
		return 1;
	}
	char * re()
	{
		if (!LetBegin) return 0;
		else return LetBegin->buf;
	}
}Lettons[MAX_DEVICE+1];

//���������� ���������.
bool SaveMes(char * buffer,int Ndest)
{
	if (Ndest==0)
	{
		for(int Ndest=1; Ndest<=MAX_DEVICE; Ndest++)
		{
			Lettons[Ndest].add(buffer); 
		}
	}
	else
	{
		Lettons[Ndest].add(buffer); 
	}
	return true;
}

//�������� ������������ ��������� � ������.
void ClearLettons()
{
	printf("CLEAR ALL LETTONS\n");
	for(int N=1; N<=MAX_DEVICE; N++)
	{
		while(Lettons[N].re())
		{
			Lettons[N].del();
		}
	}
}

//����� ��� �������� ���������
void RecvingLettons(void * arg)
{
	int N=*((int*)arg);
	printf(" %d ",N);
	NOMERATIONb=true;
	int re;
	char * buf=new char[MESSAGE_SIZE];
	while(1)	//��������� �����.
	{
		while (R[N].connected)
		{
			re=R[N].SgRecv(buf);
			if (re>0)
			{
				printf("Recv by #%d :%s\n",N,buf+2);
				buf[0]=4;
				buf[1]=N;
				pPipe->Send(buf);
			}
			if (re<0)
			{
				printf("NOT RECV BY NXT #%d!!\n",N);
			}
			Sleep(10);	
		}
		Sleep(100);
	}
}

//�������� ������ ��� �������� ���������
void ActRecving()
{
	printf("Activated recvings for devices");
	for(int i=1; i<=MAX_DEVICE; i++)
	{
		NOMERATIONb=false;
		_beginthread(RecvingLettons,4,&i);
		while(!NOMERATIONb)
		{Sleep(1);}
	}
	printf("\n");
}

//����� ��� �������� ���������.
void SendingLettons(void * arg)
{
	int N=*((int*)arg);
	printf(" %d ",N);
	NOMERATIONb=true;

	while(1)
	{
		while (R[N].connected)
		{
			if (Lettons[N].re())
			{	
				if (R[N].SgSend(Lettons[N].re()))
				{
					Lettons[N].del();	
				}
				else
				{
					R[N].ReConnect();
				}
			}
			else 
			Sleep(1);
		}
		Sleep(100);
	}
}

//�������� ������ ��� �������� ���������.
void ActSending()
{
	printf("Activated sendings for devices");
	for(int i=1; i<=MAX_DEVICE; i++)
	{
		NOMERATIONb=false;
		_beginthread(SendingLettons,4,&i);
		while(!NOMERATIONb)
		{Sleep(1);}
	}
	printf("\n");
}



//��������� ��������� "��� BT-������".
bool LetForModul(char *mes)
{
	if ((mes[1]==1))
	{
		ClearLettons();
		return 1;
	}
	return 1;
}

//��������� � ������������� �������� ���������.
//0 - ��������� ���������.
//1 - �� ������, ��������� ��������� ����������.
//2 - ����� ��� ������.
//3 - ��� ���������������� ������ �������-��������.
int Discript(char *buffer)
{
	//��������� �� ���������
	if (buffer[0]==0) return 0;
	//������ ������ ��� �������
	if (buffer[0]==1)
	{
		if ((buffer[1]>MAX_DEVICE)||(buffer[1]<0)) return 3;
		SaveMes(buffer+2,buffer[1]);
		return 1;
	}
	//������ ������ ��� ������
	if (buffer[0]==2)
	{
		LetForModul(buffer);
		return 2;
	}
	return 0;
}


//-----------------------------------------------


bool Pipe_ON=false;
bool PipeConnect=false;

//�������� ���� ��� �������� ��������� �� �����.
void RecvPipe(void *)
{
	Pipe_ON=true;
	while(Pipe_ON)
	{
		Pipe_Server Pipe(PIPE_NAME,MESSAGE_SIZE,MAX_MESSAGE); 
		pPipe=&Pipe;
		if ((Pipe.Connect())&&(Pipe.listen()))
		{
			Pipe.DispLog=false;
			PipeConnect=true;
			char * buffer=new char[MESSAGE_SIZE];
			int re;
			while(re=Pipe.Recv(buffer))
			{
				printf("Recv message :%s\n",buffer);

				if (re!=1)
					printf("CORRUPTED BUFFER by Server\n");
				
				if (!(re=Discript(buffer)))
				{
					printf("!!!ERROR MESSAGEs!!! %d\n",re);
				}
			}
			PipeConnect=false;
			printf("Client Disconnect\n");
			delete(buffer);
		}
	}
}

//�������� ������ ��� �����.
void ActRecvPipe()
{
	if(!Pipe_ON) _beginthread(RecvPipe,0,0);
}
