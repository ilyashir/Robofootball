
#include "BtPCModul.h"

#include <C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include\BluetoothAPIs_Qfixed.h>
#include "SearchCom.h"

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

BTH_ADDR str2ba(const char* straddr)
{
    BTH_ADDR btaddr;
    str2ba(straddr, &btaddr);
    return btaddr;
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
        int d1=((int)btaddr.rgBytes[i]/16)%16;
        int d2=(int)btaddr.rgBytes[i]%16;
        if (d1<10) straddr[(5-i)*3]='0'+d1;
        else straddr[(5-i)*3]='A'+d1-10;
        if (d2<10) straddr[(5-i)*3+1]='0'+d2;
        else straddr[(5-i)*3+1]='A'+d2-10;
    }
    straddr[17]=0;
    return 1;
}

#include "oleauto.h"

BtPCModul::BtPCModul(QObject *parent)
    :QThread(parent)
{
    m_radio=NULL;
    pin= SysAllocStringLen(0, 4);
    MultiByteToWideChar(CP_ACP, 0, "1234" ,4,pin ,4);

    m_device_id=0;
    for (int i=0; i<100; i++)
    {
        memset(BTaddr[i], 0,18);
        memset(SaveName[i], 0,BLUETOOTH_MAX_SIZE_NAME);
    }
}

bool BtPCModul :: FindRadio()
{
    BLUETOOTH_FIND_RADIO_PARAMS m_bt_find_radio= {sizeof(BLUETOOTH_FIND_RADIO_PARAMS)};
	if(BluetoothFindFirstRadio(&m_bt_find_radio, &m_radio) != NULL)
	{
        printf("BluetoothFindFirstRadio() is OK!\n");
		return true;
	}
    else
	{
        printf("BluetoothFindFirstRadio() failed with error code %d\n", GetLastError());
		return false;
	}
}

void BtPCModul :: GetPCInfo()
{
    BLUETOOTH_RADIO_INFO radio_info = {sizeof(BLUETOOTH_RADIO_INFO),0,};
    DWORD radio_ret = BluetoothGetRadioInfo(m_radio, &radio_info);
    if(radio_ret == ERROR_SUCCESS)
		printf("BluetoothGetRadioInfo() looks fine!\n");
	else
    {
        printf("BluetoothGetRadioInfo() failed with error code %d\n", radio_ret);
        return;
    }

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

void BtPCModul::run()
{    
    Find_Device(0);
    Find_Device(1);
    Find_Device(2);
    Find_Device(4);
    Find_Device(8);
    Find_Device(16);
    Find_Device(32);
}

void BtPCModul ::Find_Device(int time)
{
    //m_device_id=0;
    BLUETOOTH_DEVICE_INFO m_device_info= {sizeof(BLUETOOTH_DEVICE_INFO),0,};
    ZeroMemory(&m_device_info, sizeof(BLUETOOTH_DEVICE_INFO));
    m_device_info.dwSize = sizeof(BLUETOOTH_DEVICE_INFO);
    BLUETOOTH_DEVICE_SEARCH_PARAMS m_search_params = {
      sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS),
      1,
      1,
      1,
      1,
      1,
      time,
      NULL
    };
    m_search_params.hRadio = m_radio;
    HBLUETOOTH_DEVICE_FIND  m_bt_dev = BluetoothFindFirstDevice(&m_search_params, &m_device_info);

	printf("Start searching...");

	if(m_bt_dev != NULL)
		printf("\nBluetoothFindFirstDevice() is working!\n");
    else
    {
		printf("\nBluetoothFindFirstDevice() failed with error code %d\n", GetLastError());
        return;
    }
	do
	{

        //SaveAddr[m_device_id]=m_device_info.Address;
        // Well, the found device information can be used for further socket
		// operation such as creating a socket, bind, listen, connect, send, receive etc..
		// If no more device, exit the loop
		//if(!BluetoothFindNextDevice(m_bt_dev, &m_device_info))
        char newaddr[18];
        ba2str(m_device_info.Address,newaddr);
        bool bnew=true;
        for (int i=0; i<m_device_id; i++)
        {
            if (!memcmp(newaddr,BTaddr[i],18))
            {
                bnew=false;
            }
        }
        if (bnew)
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

            memcpy(BTaddr[m_device_id],newaddr,18);
            //ba2str(m_device_info.Address,BTaddr[m_device_id]);
            for(int j=0; j<BLUETOOTH_MAX_SIZE_NAME; j++)
                SaveName[m_device_id][j]=m_device_info.szName[j];
            m_device_id++;
            emit NewDevice();
        }
	}
    while((BluetoothFindNextDevice(m_bt_dev, &m_device_info)));
    if(BluetoothFindDeviceClose(m_bt_dev))
		printf("\nBluetoothFindDeviceClose(m_bt_dev) is OK!\n");
	else
		printf("\nBluetoothFindDeviceClose(m_bt_dev) failed with error code %d\n", GetLastError());
}

bool BtPCModul ::InstalDriver(NXTclass * nxtclass)
{
    BLUETOOTH_DEVICE_INFO m_device_info = {sizeof(BLUETOOTH_DEVICE_INFO),str2ba(nxtclass->BTaddr),};
    int iguid[4]=NXT_GUID;
    GUID * guid=new GUID;
    memcpy(&(guid->Data1),(int*)iguid,16);
    HRESULT re=BluetoothSetServiceState (0,&m_device_info,guid,BLUETOOTH_SERVICE_ENABLE);
    if (re==E_INVALIDARG)
    {
        printf("Driver already installed\n");
        return (nxtclass->port>0);
    }
    if (re==ERROR_SUCCESS)
    {
        printf("Driver successfully installed\n");
        return true;
    }
    printf("Driver instal failed\n");
    return false;
}

bool BtPCModul ::Authenticating(NXTclass * nxtclass)
{
    BLUETOOTH_DEVICE_INFO m_device_info = {sizeof(BLUETOOTH_DEVICE_INFO),str2ba(nxtclass->BTaddr),};
    BluetoothGetDeviceInfo(m_radio,&m_device_info);
    DWORD re=BluetoothAuthenticateDevice(0,0,&m_device_info,pin,4);
    if ((re==ERROR_NO_MORE_ITEMS)||(re==ERROR_SUCCESS))
    {
        nxtclass->authenticated=true;
        printf("Device #%d Authenticate OK.\n",nxtclass->Number);
    }
    else
    {
        nxtclass->authenticated=false;
        printf("Device #%d Authenticate ERROR.\n",nxtclass->Number);
        return 0;//ReBtConnectError(re);
    }
    return nxtclass->authenticated;
}

bool BtPCModul ::GetDeviceInfo(NXTclass *nxtclass)
{
    BLUETOOTH_DEVICE_INFO m_device_info = {sizeof(BLUETOOTH_DEVICE_INFO),str2ba(nxtclass->BTaddr),};

    if (ERROR_SUCCESS!=BluetoothGetDeviceInfo(m_radio,&m_device_info))
    {
        printf("	!!!ERROR Get Params\n");   //) {cout<<"Not_Search_Device"<<endl; return 0;}
        nxtclass->authenticated=0;
        return 0;
    }
    else
    {
        wprintf(L"  \tInstance Name: %s\r\n", m_device_info.szName);
        wprintf(L"  \tAddress: %02X:%02X:%02X:%02X:%02X:%02X\r\n",
            m_device_info.Address.rgBytes[5],m_device_info.Address.rgBytes[4], m_device_info.Address.rgBytes[3],
            m_device_info.Address.rgBytes[2],m_device_info.Address.rgBytes[1], m_device_info.Address.rgBytes[0]);
        wprintf(L"  \tClass: 0x%08x\r\n", m_device_info.ulClassofDevice);
        wprintf(L"  \tConnected: %s\r\n", m_device_info.fConnected ? L"true" : L"false");
        wprintf(L"  \tAuthenticated: %s\r\n", m_device_info.fAuthenticated ? L"true" : L"false");
        wprintf(L"  \tRemembered: %s\r\n", m_device_info.fRemembered ? L"true" : L"false");
//        cout<<"  \tCOM-port: "<<nxtclass->port<<"\r\n";
    }
    nxtclass->authenticated=((m_device_info.fAuthenticated)&&(m_device_info.fRemembered));
    for(int i=0; i<BLUETOOTH_MAX_SIZE_NAME; i++)
        nxtclass->Name[i]=m_device_info.szName[i];
    return 1;
}

int BtPCModul::PortSearch(NXTclass *nxtclass)
{
    nxtclass->port=SearchCom(nxtclass->BTaddr);
    return nxtclass->port;
}


bool BtPCModul::ComTest(NXTclass * nxtclass)
{
    if (::ComTest(nxtclass->port))
    {
        nxtclass->port=0;
        return false;
    }
    else
    {
        return true;
    }
}
//--------------------------
//!!!!!!!!!!!!!!!!!!!!!!!!!
//---------------------------
/*bool ComTest(int port)
{
return false;
}
int SearchCom(char * addr)
{
    return 0;
}
*/
