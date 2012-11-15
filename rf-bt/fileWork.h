#include <fstream>
#include <stdlib.h>

//
// Класс для работы с файлом содержащим параметры для BTмодуля.
// Формат файла:
//
// <Имя глобального параметра> = <Значение> 
// [<(Цифра) Номер используемого робота для последующих значений>] 
// <Имя параметра для робота> = <Значение> 
//
class FileWork
{
	ifstream in;
	ofstream out;
	Device * R;
	char *Fileaddr;
public:
	FileWork(char *Fileaddr1,Device * R1)
	{
		Fileaddr=Fileaddr1;
		R=R1;
	}
	int StrToInt(char *a)
	{
		int V=0;
		int i=0;
		while((a[i]=='0')||(a[i]=='1')||(a[i]=='2')||(a[i]=='3')||(a[i]=='4')
			||(a[i]=='5')||(a[i]=='6')||(a[i]=='7')||(a[i]=='8')||(a[i]=='9'))
		{
			V=V*10+a[i]-48; i++;
		}
		return V;
	}
	//Поиск позиции символа "="
	int SearchRVN(char * a) 
	{	
		int i=0;
		while (a[i]!='=') i++;
		i++;
		while(a[i]==' ') i++;
		return i;
	}
	//Чтение параметров из файла.
	bool Read()
	{
		in.open(Fileaddr,ios::in);
		if(!in.is_open()) { cout<<"NotFound "<<Fileaddr<<endl; return 0;}
		int Control=0;
		char *buf=new char[100];
		cout<<"Reciving saving params:\n";
		while (!in.eof())
		{
			//Поиск в строчке подстрок с именем параметра и взятие значения.
			in.getline(buf,100);  
			if (SearchStrInStr("PIPE_NAME",buf)) 
			{
				//if (PIPE_NAME) delete(PIPE_NAME);
				PIPE_NAME=new char[strlen(buf+SearchRVN(buf))+1];
				memcpy(PIPE_NAME,buf+SearchRVN(buf),strlen(buf+SearchRVN(buf))+1);
				cout<<" PIPE_NAME ="<<PIPE_NAME<<endl;
			}
			if (SearchStrInStr("MESSAGE_SIZE",buf)) 
			{
				MESSAGE_SIZE=StrToInt(buf+SearchRVN(buf));
				cout<<" MESSAGE_SIZE ="<<MESSAGE_SIZE<<endl;
			}
			if (SearchStrInStr("MAX_MESSAGE",buf)) 
			{
				MAX_MESSAGE=StrToInt(buf+SearchRVN(buf));
				cout<<" MAX_MESSAGE ="<<MAX_MESSAGE<<endl;
			}
			if (SearchStrInStr("MAX_SAVE_LETTONS",buf)) 
			{
				MAX_SAVE_LETTONS=StrToInt(buf+SearchRVN(buf));
				cout<<" MAX_SAVE_LETTONS ="<<MAX_SAVE_LETTONS<<endl;
			}
			if (SearchStrInStr("NXT_MESS_SIZE",buf)) 
			{
				NXT_MESS_SIZE=StrToInt(buf+SearchRVN(buf));
				cout<<" NXT_MESS_SIZE ="<<NXT_MESS_SIZE<<endl;
			}

			if ((buf[0]=='[')&&(StrToInt(buf+1)<MAX_DEVICE)) 
			{
				Control=StrToInt(buf+1);
				R[Control].activ=true;
				cout<<"	Devices #"<<Control<<" : "<<endl;	
			}
			if (SearchStrInStr("BTaddr",buf)) 
			{
				cout<<"		BTaddr ="<<buf+SearchRVN(buf)<<endl;
				R[Control].SetBTaddr(buf+SearchRVN(buf));
			}
			if (SearchStrInStr("COMport",buf)) 
			{
				R[Control].SetCOMPort(StrToInt(buf+SearchRVN(buf)));
				cout<<"		COMport ="<<StrToInt(buf+SearchRVN(buf))<<endl;
			}
			if (SearchStrInStr("ProgName",buf)) 
			{
				int p=SearchRVN(buf);
				cout<<"		ProgName ="<<buf+p<<endl;
				int L=strlen(buf+p);
				R[Control].ProgName=new char[L+1];
				for(int i=0; i<=L; i++)
				R[Control].ProgName[i]=buf[i+p];
			}
			if (SearchStrInStr("ProgAddr",buf)) 
			{
				int p=SearchRVN(buf);
				cout<<"		ProgAddr ="<<buf+p<<endl;
				int L=strlen(buf+p);
				R[Control].ProgAddr=new char[L+1];
				for(int i=0; i<=L; i++)
				R[Control].ProgAddr[i]=buf[i+p];
			}
		}
		in.close();
		return 1;
	}
	//Сохранение параметров.
	void write()
	{
		FILE *fp;
		fp=fopen(Fileaddr, "w");
		fprintf(fp,"PIPE_NAME = %s\n",PIPE_NAME);
		fprintf(fp,"MESSAGE_SIZE = %d\n",MESSAGE_SIZE);
		fprintf(fp,"MAX_MESSAGE = %d\n",MAX_MESSAGE);
		fprintf(fp,"MAX_SAVE_LETTONS = %d\n",MAX_SAVE_LETTONS);
		fprintf(fp,"NXT_MESS_SIZE = %d\n",NXT_MESS_SIZE);
		for(int i=0; i<MAX_DEVICE; i++)
		{	
			bool ON=false;
			if (!ZeroBTaddr(&R[i].m_device_info))
			{
				if (!ON) {ON=true; fprintf(fp,"[%d]\n",i);}
				fprintf(fp,"BTaddr = %02X:%02X:%02X:%02X:%02X:%02X\n", 
					R[i].m_device_info.Address.rgBytes[5],R[i].m_device_info.Address.rgBytes[4], R[i].m_device_info.Address.rgBytes[3], 
					R[i].m_device_info.Address.rgBytes[2],R[i].m_device_info.Address.rgBytes[1], R[i].m_device_info.Address.rgBytes[0]);
			}
			if (R[i].port)
			{
				if (!ON) {ON=true; fprintf(fp,"[%d]\n",i);}
				fprintf(fp,"COMport = %d\n",R[i].port);
			}
			if (R[i].ProgName)
			{
				if (!ON) {ON=true; fprintf(fp,"[%d]\n",i);}
				fprintf(fp,"ProgName = %s\n",R[i].ProgName);
			}
			if (R[i].ProgAddr)
			{
				if (!ON) {ON=true; fprintf(fp,"[%d]\n",i);}
				fprintf(fp,"ProgAddr = %s\n",R[i].ProgAddr);
			}
		}	
		fclose(fp);
	}
	//Проверка данных на актуальность.
	void TestData(bool wr=false)
	{
		for(int i=0; i<MAX_DEVICE; i++)
		{
			if ((R[i].port)&&(!R[i].COMtest())) 
			{
				cout<<"Device #"<<i<<"Error ComPort!!! Dont Enable!!!"<<endl;
				R[i].port=0;
			}
			if ((R[i].activ)&&(!R[i].port)) R[i].SearchRegCOM();
		}
		if (wr) write();
	}
};



/*	bool SearchStrInStr(char *a, char *b)
	{
		unsigned int i=0,j=0;
		while (b[j]!=0)
		{
			if (a[i]!=' ')
				if (b[j]!=a[i]) return false;
				else j++;
			i++;
		}
		return true;
	}*/