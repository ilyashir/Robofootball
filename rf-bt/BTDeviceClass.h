#include <Winsock2.h>
using namespace System;
using namespace System::IO::Ports;
using namespace System::ComponentModel;

//SerialGate : Connect by COM
#pragma comment (lib, "SerialGate.lib")
#include "SerialGate.h"
#pragma comment (lib, "Bthprops.lib")
#include <BluetoothAPIs.h>

//BLUETOOTH_RADIO_INFO m_bt_info = {sizeof(BLUETOOTH_RADIO_INFO),0,};
BLUETOOTH_FIND_RADIO_PARAMS m_bt_find_radio = {sizeof(BLUETOOTH_FIND_RADIO_PARAMS)};
BLUETOOTH_DEVICE_SEARCH_PARAMS m_search_params = {
  sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS),
  1,
  1,
  1,
  1,
  1,
  15,
  NULL
};
/*
BOOL __cdecl auth_callback(LPVOID pvParam, PBLUETOOTH_DEVICE_INFO pDevice)
{
	system("pause");
printf("auth_callback\n");
int j, result;
HANDLE hRadio = (HANDLE)pvParam;
WCHAR tempkey[12] = {0};
PWCHAR pszPasskey = tempkey;
for (j = 0; j < 8; j+=2)
{
((unsigned char*)pszPasskey)[j] = '0';
}
for (j = 1; j < 8; j+=2)
{
((unsigned char*)pszPasskey)[j] = 0x0;
}
wprintf(L"pszPasskey sent in callback: %20s\n", pszPasskey);
result = BluetoothSendAuthenticationResponse(hRadio, pDevice, pszPasskey);
if (result != ERROR_SUCCESS)
{
	printf("OLOLOLO!!!");
//printf("%s\n", GetLastErrorMessage(GetLastError()));
} 
return true;
}*/
/*
BOOL PFN_AUTHENTICATION_CALLBACK(
  LPVOID pvParam,
  PBLUETOOTH_DEVICE_INFO pDevice
);
*//*
BOOL authenticationFunctionRFCOMM( LPVOID* param, BLUETOOTH_DEVICE_INFO*bdi )
{

//body...
return 0;
}*/

UCHAR UPIN[16] ="1234";
//
//Класс для BlueTooth устройства на компьютере
//поиск, инициализация устройств, сбор данных.
//
class BTconnect
{
public:
	PWSTR pin;
	HANDLE m_radio;
	//BLUETOOTH_FIND_RADIO_PARAMS m_bt_find_radio;
	//= {sizeof(BLUETOOTH_FIND_RADIO_PARAMS)};

	BTconnect()
	{
		m_radio=NULL;
		pin= SysAllocStringLen(0, 4);
		MultiByteToWideChar(CP_ACP, 0, "1234" ,4,pin ,4);
		//m_radio=NULL;
		//m_bt_find_radio = {sizeof(BLUETOOTH_FIND_RADIO_PARAMS)};

	}

	bool CullBuck()
	{
		
		//	BLUETOOTH_AUTHENTICATE_RESPONSE AuthRes;
	//AuthRes.authMethod = pAuthCallbackParams->authenticationMethod;
	//AuthRes.bthAddressRemote = pAuthCallbackParams->deviceInfo.Address;
	//AuthRes.negativeResponse = FALSE;
	//memcpy_s(AuthRes.pinInfo.pin, sizeof(AuthRes.pinInfo.pin), pCBParams->Passkey, pCBParams->PasskeyLen);
	//AuthRes.pinInfo.pinLength = pCBParams->PasskeyLen;
	
	//	DWORD Val = BluetoothSendAuthenticationResponse(m_radio, &AuthRes);
		/*if (Val != ERROR_SUCCESS)
		{
			DWORD LastError = GetLastError();
			cout<< "BluetoothSendAuthenticationResponseEx() failed. Error = " << LastError <<"\n";
			if (LastError == 0)
			{	//In some case, GetLastError() might return zero even if Val is not zero (1244 for example).
				LastError = Val;
			}
			
			//Indicate out CB that we got auth request but filed
			pCBParams->CB(Val, LastError);
		
		}
		else
		{
			//Indicate out CB that we got auth request and success.
			pCBParams->CB(ERROR_SUCCESS, 0);
		}*/
		return 0;
	}
	bool FindRadio()
	{		
		if(BluetoothFindFirstRadio(&m_bt_find_radio, &m_radio) != NULL)
		{
			printf("BluetoothFindFirstRadio() is OK!\n");
			CullBuck();
			return true;
		}
		else
		{
			printf("BluetoothFindFirstRadio() failed with error code %d\n", GetLastError());
			return false;	
		}
	}
	void GetInfo()
	{
		
		BLUETOOTH_RADIO_INFO radio_info = {sizeof(BLUETOOTH_RADIO_INFO),0,};
	    DWORD radio_ret = BluetoothGetRadioInfo(m_radio, &radio_info);
        if(radio_ret == ERROR_SUCCESS)
			printf("BluetoothGetRadioInfo() looks fine!\n");
		else
			printf("BluetoothGetRadioInfo() failed wit herror code %d\n", radio_ret);
		
		wprintf(L"\tInstance Name: %s\r\n", radio_info.szName);
		wprintf(L"\tAddress: %02X:%02X:%02X:%02X:%02X:%02X\r\n", 
			radio_info.address.rgBytes[5],radio_info.address.rgBytes[4], radio_info.address.rgBytes[3], 
			radio_info.address.rgBytes[2],radio_info.address.rgBytes[1], radio_info.address.rgBytes[0]);
		wprintf(L"\tClass: 0x%08x\r\n", radio_info.ulClassofDevice);
		wprintf(L"\tManufacturer: 0x%04x\r\n", radio_info.manufacturer);                                 
			//m_search_params.hRadio = m_radio;
			//ZeroMemory(&radio_info, sizeof(BLUETOOTH_DEVICE_INFO));
			//m_device_info.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);	
	}
	
	void Find_Device()
	{
		BLUETOOTH_DEVICE_INFO m_device_info = {sizeof(BLUETOOTH_DEVICE_INFO),0,};
		int m_device_id;

		m_search_params.hRadio = m_radio;
		ZeroMemory(&m_device_info, sizeof(BLUETOOTH_DEVICE_INFO));
		m_device_info.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);
		HBLUETOOTH_DEVICE_FIND m_bt_dev = NULL;

		printf("Start searching...");
		m_bt_dev= BluetoothFindFirstDevice(&m_search_params, &m_device_info);
		if(m_bt_dev != NULL)
			printf("\nBluetoothFindFirstDevice() is working!\n");
		else
			printf("\nBluetoothFindFirstDevice() failed with error code %d\n", GetLastError());
		
		m_device_id = 0;
		do
		{
			wprintf(L"\n\tDevice %d:\r\n", m_device_id);
			wprintf(L"  \tInstance Name: %s\r\n", m_device_info.szName);
			wprintf(L"  \tAddress: %02X:%02X:%02X:%02X:%02X:%02X\r\n", 
				m_device_info.Address.rgBytes[5],m_device_info.Address.rgBytes[4], m_device_info.Address.rgBytes[3], 
				m_device_info.Address.rgBytes[2],m_device_info.Address.rgBytes[1], m_device_info.Address.rgBytes[0]);
			wprintf(L"  \tClass: 0x%08x\r\n", m_device_info.ulClassofDevice);
			wprintf(L"  \tConnected: %s\r\n", m_device_info.fConnected ? L"true" : L"false");
			wprintf(L"  \tAuthenticated: %s\r\n", m_device_info.fAuthenticated ? L"true" : L"false");
			wprintf(L"  \tRemembered: %s\r\n", m_device_info.fRemembered ? L"true" : L"false");
			m_device_id++;
			// Well, the found device information can be used for further socket
			// operation such as creating a socket, bind, listen, connect, send, receive etc..
			// If no more device, exit the loop
			//if(!BluetoothFindNextDevice(m_bt_dev, &m_device_info))
			//	break;
		} 
		while(BluetoothFindNextDevice(m_bt_dev, &m_device_info));
			
		if(BluetoothFindDeviceClose(m_bt_dev) == TRUE)
			printf("\nBluetoothFindDeviceClose(m_bt_dev) is OK!\n");
		else
			printf("\nBluetoothFindDeviceClose(m_bt_dev) failed with error code %d\n", GetLastError());
	}

	BLUETOOTH_ADDRESS SaveAddr[100];
	char SaveName[100][248];
	int m_device_id;
	void Find_Device_Save(bool NewSearch=false)//char * addr[18],char * name[100])
	{		
		BLUETOOTH_DEVICE_INFO m_device_info = {sizeof(BLUETOOTH_DEVICE_INFO),0,};
		ZeroMemory(&m_device_info, sizeof(BLUETOOTH_DEVICE_INFO));
		m_device_info.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);		

		m_search_params.hRadio = m_radio;
		
		HBLUETOOTH_DEVICE_FIND m_bt_dev = NULL;

		printf("Start searching...");
		m_bt_dev= BluetoothFindFirstDevice(&m_search_params, &m_device_info);
		
		if(m_bt_dev != NULL)
			printf("\nBluetoothFindFirstDevice() is working!\n");
		else
			printf("\nBluetoothFindFirstDevice() failed with error code %d\n", GetLastError());
		
		m_device_id = 0;
		do
		{
			SaveAddr[m_device_id]=m_device_info.Address;
			for(int i=0; i<248; i++)
				SaveName[m_device_id][i]=m_device_info.szName[i];
			//wctomb( SaveName[100], &m_device_info.szName[0]);
			//SaveName[m_device_id]=m_device_info.szName;
			wprintf(L"\n\tDevice %d:\r\n", m_device_id);
			wprintf(L"  \tInstance Name: %s\r\n", m_device_info.szName);
			wprintf(L"  \tAddress: %02X:%02X:%02X:%02X:%02X:%02X\r\n", 
				m_device_info.Address.rgBytes[5],m_device_info.Address.rgBytes[4], m_device_info.Address.rgBytes[3], 
				m_device_info.Address.rgBytes[2],m_device_info.Address.rgBytes[1], m_device_info.Address.rgBytes[0]);
			wprintf(L"  \tClass: 0x%08x\r\n", m_device_info.ulClassofDevice);
			wprintf(L"  \tConnected: %s\r\n", m_device_info.fConnected ? L"true" : L"false");
			wprintf(L"  \tAuthenticated: %s\r\n", m_device_info.fAuthenticated ? L"true" : L"false");
			wprintf(L"  \tRemembered: %s\r\n", m_device_info.fRemembered ? L"true" : L"false");
			m_device_id++;
		} 
		while(BluetoothFindNextDevice(m_bt_dev, &m_device_info));
			
		if(BluetoothFindDeviceClose(m_bt_dev) == TRUE)
			printf("\nBluetoothFindDeviceClose(m_bt_dev) is OK!\n");
		else
			printf("\nBluetoothFindDeviceClose(m_bt_dev) failed with error code %d\n", GetLastError());
	}

	int COMs[100];
	int COM_id;
	void Find_COM_Save()
	{
		COM_id=0;
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
		for each(String^ port in serialPorts)
	    {
			char * a=(char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(port);
			int N=0; int i=2;
			while (a[i+1]!=0)
			{
				i++;
				N=N*10+a[i]-48;
			}
			COMs[COM_id]=N;
			COM_id++;
	    }
	}
};
