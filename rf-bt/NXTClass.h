using namespace System;
using namespace System::Security::Permissions;
using namespace Microsoft::Win32;

char * SysStringToChar(System::String^ string) 
{
return (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(string);
}/*
bool SearchStrInStr(char *a, char *b)
	{
		unsigned int i=0,j=0;
		for(int i=0; (i+strlen(b))<=strlen(a); i++)
		{
			bool t=true;
			for(j=0; j<strlen(b); j++)
			{
				if (a[i+j]!=b[j]) t=false;
			}
			if (t) return true;
		}
		return false;
	}*/


//
bool ReBtConnectError(DWORD re)
{
	switch (re)
	{
		case ERROR_SUCCESS:
		cout<<"CONNECTED"<<endl;
		return true;
		case ERROR_CANCELLED: 
		{cout<<"ERROR_CANCELLED"<<endl;}
		return 0;
		case ERROR_INVALID_PARAMETER:
		{cout<<"ERROR_INVALID_PARAMETER"<<endl;}
		return 0;
		case ERROR_NO_MORE_ITEMS:
		{cout<<"ERROR_NO_MORE_ITEMS"<<endl;}
		return 0;
		case ERROR_DEVICE_NOT_CONNECTED:
		{cout<<"ERROR_DEVICE_NOT_CONNECTED"<<endl;}
		return 0;
		case WAIT_TIMEOUT:
		{cout<<"WAIT_TIMEOUT"<<endl;}
		return 0;
		case ERROR_GEN_FAILURE:
		{cout<<"ERROR_GEN_FAILURE"<<endl;}
		return 0;
		case ERROR_NOT_AUTHENTICATED:
		{cout<<"ERROR_NOT_AUTHENTICATED"<<endl;}
		return 0;
		case ERROR_NOT_ENOUGH_MEMORY:
		{cout<<"ERROR_NOT_ENOUGH_MEMORY"<<endl;}
		return 0;
		case ERROR_REQ_NOT_ACCEP:
		{cout<<"ERROR_REQ_NOT_ACCEP"<<endl;}
		return 0;
		case ERROR_ACCESS_DENIED:
		{cout<<"ERROR_ACCESS_DENIED"<<endl;}
		return 0;
		case ERROR_NOT_READY:
		{cout<<"ERROR_NOT_READY"<<endl;}
		return 0;
		case ERROR_VC_DISCONNECTED:
		{cout<<"ERROR_VC_DISCONNECTED"<<endl;}
		return 0;
		default:
		{cout<<"ERROR_???"<<endl;}
		return 0;
	}
}
bool ReServisIniError(DWORD re)
{
	//cout<<".";
	switch (re)
	{
	case ERROR_SUCCESS:
	cout<<"       Services_SUCCES"<<endl;
			return 1;
	case ERROR_INVALID_PARAMETER:
	cout<<"        ERROR_INVALID_PARAMETER"<<endl;
			return 0;
	case ERROR_SERVICE_DOES_NOT_EXIST:
	cout<<"        ERROR_SERVICE_DOES_NOT_EXIST"<<endl;
			return 0;
	case  E_INVALIDARG:
	cout<<"         DRIVER_BE_INSTALLED(UINSTALLED)"<<endl;
			return 0;	
	default:
		cout<<"????"<<endl;
		return 0;
	}
}

void stradd(char * a, char * b)
{
	int La=strlen(a);
	int Lb=strlen(b);
	//char * newa=new char[La+Lb+1];
	//memcpy(newa   ,a,La);
	memcpy(a+La,b,Lb);
	a[La+Lb]=0;
	//delete a;
	//a=newa;
}

int DefNXTGuid[4]={4353,268435456,-2147483520,-80438433};

char NXT_PROGRAMM_NAME[100]="BTtext.rxe";//(EXAMPLE) max_LN=100
#define DefProgrammName NXT_PROGRAMM_NAME
char * DefBTaddr="00:00:00:00:00:00";
char * Defpin="1234";

/* Convert mac address string to BTH_ADDR */
int str2ba(const char* straddr, BTH_ADDR* btaddr)
{
    int i;
    unsigned int aaddr[6 ];
    BTH_ADDR tmpaddr = 0;

    if (sscanf(straddr, "%02x:%02x:%02x:%02x:%02x:%02x",
            &aaddr[0], &aaddr[1], &aaddr[2],
            &aaddr[3], &aaddr[4], &aaddr[5]) != 6)
        return -1;
    
    *btaddr = 0;
    for (i = 0; i < 6; i++) {
        tmpaddr = (BTH_ADDR) (aaddr[i ] & 0xff);
        *btaddr = ((*btaddr) << 8) + tmpaddr;
    }
    return 0;
}
int COMsumm()
{
	array<String^>^ serialPorts = nullptr;
	try
	{
	   // Get a list of serial port names.	
	   serialPorts = SerialPort::GetPortNames();
	}
	catch (Win32Exception^ ex)
	{
       Console::WriteLine(ex->Message);
    }	
	int NCOM;
    // Display each port name to the console.
    for each(String^ port in serialPorts)
    {
		char * a=(char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(port);
		int N=0; int i=2;
		while (a[i+1]!=0)
		{
			i++;
			N=N*10+a[i]-48;
		}
		NCOM+=N;
    }
	return NCOM;
}
	

int ba2str(BLUETOOTH_ADDRESS btaddr,char * straddr)
{
	for(int i=0; i<18; i++)
	{
		if ((i+1)%3) straddr[i]=0;
		else straddr[i]=':';
	}
	for(int i=0; i<6; i++)
	{
		int d1=(int)	btaddr.rgBytes[i]/16;
		int d2=(int)	btaddr.rgBytes[i]%16;
		if ((d1>15)||(d2>15)) return 0;
		if (d1<10) straddr[(5-i)*3]='0'+d1;
		else straddr[(5-i)*3]='A'+d1-10;
		if (d2<10) straddr[(5-i)*3+1]='0'+d2;
		else straddr[(5-i)*3+1]='A'+d2-10;
	}
	straddr[17]=0;
	return 1;
}

bool ZeroBTaddr(BLUETOOTH_DEVICE_INFO * m_device_info)
{
	for (int i=0; i<6; i++)
	if (*(char*)(&m_device_info->Address.rgBytes[i])!=0) return false;
	return true;
}
//Проверка номера порта на задействованность в системе.
bool COMtest(int port)
{
	if (0==port) return false;
	array<String^>^ serialPorts = nullptr;
	try
	{
        // Get a list of serial port names.	
		serialPorts = SerialPort::GetPortNames();
	}
	catch (Win32Exception^ ex)
	{
		Console::WriteLine(ex->Message);
	}	
	// Display each port name to the console.
	for each(String^ sport in serialPorts)
	{
		char * a=(char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(sport);
		int N=0; int i=2;
		while (a[i+1]!=0)
		{
			i++;
			N=N*10+a[i]-48;
		}
		if (N==port) return true;	
	}
	return false;
}

class Device
{	
private:
	int SCOMSumm;
public:
	PWSTR pin;
	SerialGate * sg;
	GUID * guid;
	char reBTaddr[18];//="00:00:00:00:00:00";

public:
	
	bool installing;
	char * ProgAddr;
	char * ProgName;
	BLUETOOTH_DEVICE_INFO m_device_info;
	int port;
	int Number;
	char ProgrammName[100];

	bool authenticated;//Àóòåíòåôèêàöèÿ ïðîéäåíà.
	bool connected; //Ïîäêëþ÷åíèå äåéñòâèòåëüíî.
	bool activ;// BTaddr íàçíà÷åí.

	void SetCOMPort(int port1)
	{
		port=port1;
	}
	void SetBTaddr(char *BTaddr)
	{
		str2ba(BTaddr,(BTH_ADDR*)(&m_device_info.Address));
	}
	char * ReBTaddr()
	{
		ba2str(m_device_info.Address,reBTaddr);
		return reBTaddr;
	}
	void SetPin(char *Pin_Cod)
	{
		MultiByteToWideChar(CP_ACP, 0,Pin_Cod,4,pin ,4); 
	}
	void SetGuid(int Value[4])
	{
		memcpy(&(guid->Data1),Value,16);
	}	
	void SetProgrammName(char * ProgrammName_)
	{
		int i=0;
		while(ProgrammName_[i])
		{
			ProgrammName[i]=ProgrammName_[i];
			i++;
		}
		while(i<100)
		{
			ProgrammName[i]=0;
			i++;
		}
	}
	Device()
	{ 
    //Инициализация нулями.
		installing=0;
		activ=0;
		Number=0;
		connected=false;
		m_device_info.dwSize=sizeof(BLUETOOTH_DEVICE_INFO);
		m_device_info.fAuthenticated=0;
		m_device_info.fConnected=0;
		m_device_info.fRemembered=0;
		m_device_info.ulClassofDevice=0;
		m_device_info.szName[0]=0;
		pin= SysAllocStringLen(0, 4);
		guid=new GUID;

		SetProgrammName(DefProgrammName);
		SetBTaddr(DefBTaddr);
		SetGuid(DefNXTGuid);
		SetPin(Defpin);
		port=0;
	}
	//--------------------------
  //NextTool - Запуск программы на устройстве 	  	
	bool RunProgramm()
	{
		if (!ProgName) 
		{
			printf("Not set Program Name\n");
			return 1;
		}
		char * BTaddr=ReBTaddr();
		char ch[53+100]="NexTTool.exe /COM=BTH::NXT::00:00:00:00:00:00:0 -run=";
		stradd(ch,ProgName);		
		for(int i=0; i<17; i++)
			ch[28+i]=BTaddr[i];
		printf("%s\n",ch);
		system(ch);
		return 1;
	}
  //NextTool - Проверка уровня батареи.
	int ReBattery()
	{
    //Компановка запроса.
		char * addr=ReBTaddr();
		char ch[53+100]="NexTTool.exe /COM=BTH::NXT::00:00:00:00:00:00:0 -battery > NeXTToolOut.txt";
		//memcpy(ch,addr,17);
		for(int i=0; i<17; i++)
			ch[28+i]=addr[i];
		printf("%s\n",ch);
		system(ch);
      
		//Считывание ответа.
		FILE * f=fopen("NeXTToolOut.txt","r");
		fseek(f, 0, SEEK_END);
		int size = ftell(f);
		fseek(f, 0, SEEK_SET);
		if (size)
		{	
			char * buf=new char[size];
			buf[size-1]=0;
			fread(buf,1,size-1,f);
			printf("Suckesfully download %s",buf);
			fclose (f);
			delete(buf);
			return ((buf[0]-48)*1000+(buf[1]-48)*100+(buf[2]-48)*10+(buf[3]-48));	
		}
		else 
		{
			fclose (f);
			printf("ERROR. No answer by NXT\n");
			return 0;
		}

	}
 
  //NextTool - Загрузка программы на NXT.
  int DownloadProg()
	{
		if (!ProgAddr) 
		{
			printf("Not set Program Address\n");
			return 0;
		}
    //Компановка запроса.
		char * addr=ReBTaddr();
		int L=strlen(ProgAddr);
		char *ch=new char[2*L+200]; ch[0]=0;
		stradd(ch,"NexTTool.exe /COM=BTH::NXT::00:16:53:12:82:84:0");
		for(int i=0; i<17; i++)
			ch[28+i]=addr[i];
		stradd(ch," -download=");		
		stradd(ch,ProgAddr);
		stradd(ch," -listfiles=");
		stradd(ch,ProgName);
		stradd(ch," > NeXTToolOut.txt");
		printf("%s\n",ch);
		system(ch);
		delete ch;

		FILE * f=fopen("NeXTToolOut.txt","r");
		fseek(f, 0, SEEK_END);
		int size = ftell(f);
		fseek(f, 0, SEEK_SET);
		if (size)
		{	
			char * buf=new char[size];
			buf[size-1]=0;
			fread(buf,1,size-1,f);
			printf("Suckesfully download %s",buf);
			fclose (f);
			delete(buf);
			return 1;
		}
		else 
		{
			fclose (f);
			printf("ERROR. No answer by NXT\n");
			return 0;
		}
	}
	int DownloadProg2()
	{
		if (!ProgAddr) 
		{
			printf("Not set Program Address\n");
			return 0;
		}
		char *addr=ReBTaddr();
		int L=strlen(ProgName);
		char *ch=new char[2*L+200]; ch[0]=0;
		stradd(ch,"NexTTool.exe /COM=BTH::NXT::00:00:00:00:00:00:0 -download=");
		for(int i=0; i<17; i++)
			ch[28+i]=addr[i];
		stradd(ch,ProgName);
		printf("%s\n",ch);
		system(ch);
		delete ch;

		return 0;
		
	}
	
	//Вывод всех параметров на экран.
	bool GetInfo()
	{
		if (ERROR_SUCCESS!=BluetoothGetDeviceInfo(0,&m_device_info)) cout<<"	!!!ERROR Get Params\n";//) {cout<<"Not_Search_Device"<<endl; return 0;}
		else
		{
			authenticated=((m_device_info.fAuthenticated)&&(m_device_info.fRemembered));
			wprintf(L"  \tInstance Name: %s\r\n", m_device_info.szName);
			wprintf(L"  \tAddress: %02X:%02X:%02X:%02X:%02X:%02X\r\n", 
				m_device_info.Address.rgBytes[5],m_device_info.Address.rgBytes[4], m_device_info.Address.rgBytes[3], 
				m_device_info.Address.rgBytes[2],m_device_info.Address.rgBytes[1], m_device_info.Address.rgBytes[0]);
			wprintf(L"  \tClass: 0x%08x\r\n", m_device_info.ulClassofDevice);
			wprintf(L"  \tConnected: %s\r\n", m_device_info.fConnected ? L"true" : L"false");
			wprintf(L"  \tAuthenticated: %s\r\n", m_device_info.fAuthenticated ? L"true" : L"false");
			wprintf(L"  \tRemembered: %s\r\n", m_device_info.fRemembered ? L"true" : L"false");
			cout<<"  \tCOM-port: "<<port<<"\r\n";
		}
		return 1;
	}

//----------------------------------AUT
	bool AuthenticatingS()
	{
		return ReBtConnectError(BluetoothAuthenticateDevice(0,0,&m_device_info,pin,4));
	}
	bool Authenticating()//authenticated
	{
		DWORD re=BluetoothAuthenticateDevice(0,0,&m_device_info,pin,4);
		if ((re==ERROR_NO_MORE_ITEMS)||(re==ERROR_SUCCESS))
		{
			authenticated=true;
			printf("Device #%d Authenticate: OK\n",Number);
		}
		else
		{
			authenticated=false;
			printf("Device #%d Authenticate: ",Number);
			return ReBtConnectError(re);
		}
		return authenticated;
	}

//-------------COM-----------COM------------Driver INSTAL - COM
	//Проверка действительности порта.
  bool COMtest()
	{
		if (::COMtest(port)) return true;
		port=0;
		return false;
	}
  //Установка драйвера блютус устройства
	bool Enable()
	{
		bool re=ReServisIniError(BluetoothSetServiceState (0,&m_device_info,guid,BLUETOOTH_SERVICE_ENABLE));
		COMtest();
		return(re);
	}
   //Удаление драйвера.
	bool Disable()
	{
		bool re=ReServisIniError(BluetoothSetServiceState (0,&m_device_info,guid,BLUETOOTH_SERVICE_DISABLE ));
		COMtest();
		return (re);
	}
   //Установка драйвера с отслеживанием com-порта
	bool Enable_ReCom()
	{
		int NCOM=COMsumm();
		if (!ReServisIniError(BluetoothSetServiceState (0,&m_device_info,guid,BLUETOOTH_SERVICE_ENABLE)))
			return 0;
		while(COMsumm()==NCOM);
		port=COMsumm()-NCOM;
		if (!COMtest()) printf("ERROR: not result port.");
		printf("COM port: %d\n",port);
		return 1;
	}
   //Поиск COM-порта в реестре.
	int SearchRegCOM()
	{
		char * addr=ReBTaddr();
		char addrCl[13];
		for(int i=0; i<18; i++)
			addrCl[i-i/3]=addr[i];
		printf("Search reestr for %s\n",addrCl);
		port=0;
		RegistryKey ^ tempKey0 = Registry::LocalMachine->OpenSubKey( "SYSTEM\\CurrentControlSet\\Enum\\BTHENUM" );
		//Console::WriteLine( "There are {0} subkeys under Test9999.", test9999->SubKeyCount.ToString() );
		array<String^>^subKeyNames = tempKey0->GetSubKeyNames();
		for ( int i = 0; i < subKeyNames->Length; i++ )
		{
			RegistryKey ^ tempKey1 = tempKey0->OpenSubKey( subKeyNames[ i ] );
			//Console::WriteLine( "\nThere are {0} values for {1}.", tempKey->ValueCount.ToString(), tempKey->Name );	
			array<String^>^subKeyNames2 = tempKey1->GetSubKeyNames();
			for ( int j = 0; j < subKeyNames2->Length; j++ )
			{
				//Console::WriteLine( "{0,-8}: {1}", valueNames[ j ], tempKey->GetValue( valueNames[ j ] )->ToString() );
				RegistryKey ^ tempKey2 = tempKey1->OpenSubKey( subKeyNames2[ j ] );
				
				char * a=SysStringToChar(tempKey2->Name);
				if (SearchStrInStr(addrCl,a)) 
				{
					array<String^>^subKeyNames3 = tempKey2->GetSubKeyNames();
					for(int jj=0; jj<subKeyNames3->Length; jj++)
					if (SearchStrInStr("Device Parameters",SysStringToChar(subKeyNames3[jj])))
					{
						RegistryKey ^ tempKey3 = tempKey2->OpenSubKey(subKeyNames3[jj]);
			
						array<String^>^valueNames = tempKey3->GetValueNames();
						for ( int jjj = 0; jjj < valueNames->Length; jjj++ )
						if (SearchStrInStr("PortName",SysStringToChar(valueNames[jjj])))
						{
							char * b =SysStringToChar(tempKey3->GetValue(valueNames[jjj])->ToString());
							printf("Reestr reciving key:\n%s",a);
							printf(" - %s ",b);
							int report=0;
							int ii=3;
							while(b[ii])
							{
								report=report*10+b[ii]-48;
								ii++;
							}
							if (::COMtest(report))
							{
								
								if ((port)&&(port!=report))
								{
									printf("- ERROR!!! VARIABLE REESTR INFORMATION!!!");
								}
								printf("\n");
								port=report;
							}
							else 
							{
								printf("- NOT activited COM\n");
							}
						}
					}
				}
			}
		}
		return port;
	}

	bool InstalDriver()
	{
		port=0;	   
		SCOMSumm=COMsumm();	
		DWORD re=BluetoothSetServiceState(0,&m_device_info,guid,BLUETOOTH_SERVICE_ENABLE);
		if (re==E_INVALIDARG)
		{
			return (SearchRegCOM()>0);
		}
		if (re==ERROR_SUCCESS)
		{
			installing=true;
			return true;
		}
		return false;
	}
  
	bool ECOMSumm()
	{
		if ((port)||(SCOMSumm!=COMsumm()))
		{
			if (SearchRegCOM())
			{
				installing=false;
				return true;
			}
		}
		return false;
	}

//-------SG-----------------SG--------------SG-----------SG--------------SG
	//Открытие COM-порта посредством создания структуры SerialGate.
   bool SgConnect()
	{
		if (connected) return 1;
		if (sg==0) sg=new SerialGate;
		int rate = 57600;
		bool re = sg->Open(port, rate);
		if(re)
		{
			printf("Open OK!\n");
			connected=true;
			return 1;
		}
		else
		{
			printf("Open Error\n");
			connected=false;
			sg->Close();
			return 0;
		}  	
	}
   //Отправка на устройства.
	bool SgSend(char * mes)
	{		
		if (!connected) return false;
		char * buf= new char[NXT_MESS_SIZE+2];
		
		buf[0]=32;
		buf[1]=0;
		for (int i=0; i<NXT_MESS_SIZE; i++) 
		{
			buf[2+i]=mes[i];		
		}
		
		bool re= sg->Send(&buf[0], NXT_MESS_SIZE+2);
		delete(buf);
		if (!re) printf("Device #%d, Sending ERROR!!!\n",Number);
		return re;
	}
   //Принятие с устройства.
	bool SgRecv(char * buf)
	{
		if (!connected) return false;
		return (sg->Recv(buf, NXT_MESS_SIZE+2));
	}
   //Принятие с устройства
	bool SgRecv(char * mes, int * size)
	{
		if (!connected) return false;
		char * buf= new char [NXT_MESS_SIZE+2];
		*size=sg->Recv(&buf[0], NXT_MESS_SIZE+2);
		for(int i=0; i<*size-2; i++)
		{
			mes[i]=buf[i+2];
		}
		return (*size>0);
	}
   //ЗАкрытие COM-порта.
	void SgClose()
	{
		if (connected)
		{
			sg->Close();
		}
		connected=false;
	}
   //Очистка порта.
	void SgClean()
	{
		sg->Clean();
	}
   //Переподключение
	void ReConnect()
	{
		int ReCon=true;
		while(ReCon)
		{
			ReCon=false;
			int Re=MessageBoxA(NULL,LPCSTR("Ïîâòîðèòü ïîïûòêó ïîäêëþ÷åíèÿ?"),LPCSTR("Óñòðîéñòâî íå îòâå÷àåò."),MB_YESNOCANCEL| MB_ICONINFORMATION);
			if ((Re==IDNO)||(Re==IDYES)) 
			{
				SgClose();
				if (Re==IDYES) 
				{
					ReCon=!SgConnect();
				}
			}
		}
	}
//---------------------------------------INST
	/*void Inst(char * addr=0)
	{
		if (addr==0) addr=ReBTaddr();
		else SetBTaddr(addr);
		while (!Authenticating())
		{
			int re = MessageBoxA(NULL,LPCSTR("Ïîâòîðèòü ïîïûòêó àóòåíòèôèêàöèè?\n Ïèí-Êîä: '1234'"),LPCSTR("Óñòðîéñòâî íå îòâå÷àåò."),MB_OKCANCEL| MB_ICONWARNING);
			if (re==IDCANCEL) return;
 		}
		if (!SearchRegCOM()) InstalDriver();
		while (!SearchRegCOM())
			Sleep(500);
		{
			int re = MessageBoxA(NULL,LPCSTR("Ïîäêëþ÷èòüñÿ ê óñòðîéñòâñó?\n(Çàïóñòèòå ïðîãðàììó)."),LPCSTR("Óñòðîéñòâî îæèäàåò."),MB_OKCANCEL| MB_ICONWARNING);
			if (re==IDCANCEL) return;
		}
		while(!SgConnect())
		{
			int re = MessageBoxA(NULL,LPCSTR("Ïîâòîðèòü ïîïûòêó ïîäêëþ÷åíèÿ?"),LPCSTR("Óñòðîéñòâî íå îòâå÷àåò."),MB_OKCANCEL| MB_ICONWARNING);
			if (re==IDCANCEL) return;
		}
		int re = MessageBoxA(NULL,LPCSTR("Ìîæåòå çàïóñêàòü èñïîëíåíèå."),LPCSTR("Óñòðîéñòâî ãîòîâî."),MB_OK| MB_ICONINFORMATION);
	}
	int FullInstal()
	{
		printf("Prepair device.\n Step1 : identificated... ");
		DWORD re=BluetoothAuthenticateDevice(0,0,&m_device_info,pin,4);
		while (!((re==ERROR_NO_MORE_ITEMS)||(re==ERROR_SUCCESS)))
		{
			printf("\nERROR (step1): ");
			ReBtConnectError(re);
			printf("Check device for working\n");
			system("pause");
			re=BluetoothAuthenticateDevice(0,0,&m_device_info,pin,4);
		}
		printf("OK!\n");
		//if (re==ERROR_NO_MORE_ITEMS) printf("OK! LATE CONNECTED\n");
		//if (re==ERROR_SUCCESS) printf("OK! CONNECT\n");
		//---------------------Driver
		printf(" Step2 : Instal driver...");
		bool re2;
		if (re2=!COMtest()) 
		{
			printf("\n");
			printf("Delete:");
			Disable(); 
			printf("Instal:");
			Enable_ReCom();
		}
		else printf("OK! COMport = %d\n",port);
		//printf("PLS, starting NXT program %s end press enter...\n",NXT_PROGRAMM_NAME);
		//system("pause");
		printf(" Step3 : Connecting...");
		if (re2) 
		{
			printf("Check device for working and press enter\n");
			system("pause");
		}
		RunProgramm();
		
		while (!SgConnect())
		{
			printf("OK!\nCheck NXT program for starting...\n");
			system("pause");
		}
		if (SgSend("0"))
			printf("\nNXT IS READY, RUN PROGRAMM\n");
		else 
			printf("\nUNCNOWN ERROR\n");
		return true;
	}*/
};



	/*wprintf(L"  \tAddress: %02X:%02X:%02X:%02X:%02X:%02X\r\n", 
			m_device_info.Address.rgBytes[5],m_device_info.Address.rgBytes[4], m_device_info.Address.rgBytes[3], 
			m_device_info.Address.rgBytes[2],m_device_info.Address.rgBytes[1], m_device_info.Address.rgBytes[0]);
	*/	/*Service NewService(int * Volume)
	{
		return Service(Volume, m_device_info);
	}*/
	
	/*
			wprintf(L"  \tInstance Name: %s\r\n", m_device_info.szName);
			wprintf(L"  \tAddress: %02X:%02X:%02X:%02X:%02X:%02X\r\n", 
				m_device_info.Address.rgBytes[5],m_device_info.Address.rgBytes[4], m_device_info.Address.rgBytes[3], 
				m_device_info.Address.rgBytes[2],m_device_info.Address.rgBytes[1], m_device_info.Address.rgBytes[0]);
			wprintf(L"  \tClass: 0x%08x\r\n", m_device_info.ulClassofDevice);
			wprintf(L"  \tConnected: %s\r\n", m_device_info.fConnected ? L"true" : L"false");
			wprintf(L"  \tAuthenticated: %s\r\n", m_device_info.fAuthenticated ? L"true" : L"false");
			wprintf(L"  \tRemembered: %s\r\n", m_device_info.fRemembered ? L"true" : L"false");
	*///cout<<"NexTTool.exe /COM=BTH::NXT::00:16:53:12:82:84:0 -run=BTtext.rxe"<<endl;
				//cout<<ProgrammName<<endl;

				/*
	bool SgSend(char * mes, int cbuf=MESSAGE_SIZE)
	{		
		if (!connected) return false;
		char * buf= new char[NXT_MESS_SIZE+2];
		for (int i=0; i<NXT_MESS_SIZE+2; i++) 
		{
			buf[i]=0;
		}
		buf[0]=32;
		buf[1]=0;
		int i=-1;
		do
		{
			i++;
			buf[2+i]=mes[i];
		}
		while ((i<15)&&(mes[i]));
		
		buf[17]=0;
		bool re= sg->Send(&buf[0], NXT_MESS_SIZE+2);
		delete(buf);
		if (!re) printf("Device #%d, Sending ERROR!\n",Number);
		return re;
	}
	*/