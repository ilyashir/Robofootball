#include <windows.h>

#define MAX_WIN_PORT 255

struct Port
{
	unsigned char Id;	
	bool Availbl;
};

struct PortInfo
{
	Port p[MAX_WIN_PORT];
	unsigned char koll;
};


extern "C" class __declspec (dllexport)  SerialGate
{	
public:
	
	enum IN_LINES_NAME {CTS, DSR, RING, RLSD};
	enum OUT_LINES_NAME {DTR, RTS};	

	SerialGate();
	~SerialGate();	

	bool Open(int port, int baud);	
	int Send(char* buff, int szBuff);
	int Recv(char* buff, int szBuff);	
	void SetLine(OUT_LINES_NAME ln, bool state);
	bool GetLine(IN_LINES_NAME ln);
	void GetPortsInfo(PortInfo* pi);
	void Close();
	void Clean();

private:
	HANDLE m_hFile;
	bool state;
};



