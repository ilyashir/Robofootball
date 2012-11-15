#ifndef PIPE_CLIENT
#define PIPE_CLIENT

//#include <winsock2.h>
#include <stdio.h>
#include <iostream>

//Name given to the pipe
#define MESSAGE_SIZE 100
#define MAX_MESSAGE 1000
#define BUFFER_SIZE MessageSize*MaxMessage
#define PIPE_HANDLE "\\\\.\\pipe\\"
//Pipe name format - \\servername\pipe\pipename

//This pipe is for server on the same computer, 
//however, pipes can be used to
//connect to a remote server
class Pipe
{
public:

	HANDLE hPipe;
	size_t MessageSize;	
	bool DispLog;
	BOOL Send(char * Buffer,DWORD * cbBytes,size_t BufferSize=0)
	{
		if (BufferSize==0) 
		{
			BufferSize=MessageSize;//strlen(Buffer)+1;
		}
		//Send the message to server
		BOOL bResult = WriteFile( 
			hPipe,                // handle to pipe 
			Buffer,             // buffer to write from
			BufferSize,   // number of bytes to write, include the NULL
			cbBytes,             // number of bytes written 
			NULL);                // not overlapped I/O 
		
		if ( (!bResult) || (*cbBytes==0) ) 
		{
			if (DispLog) printf("Error occurred while writing" 
                 " to the server: %d\n", GetLastError()); 
			//CloseHandle(hPipe);
			return 0;  //Error
		}
		else
		{
			if (BufferSize != *cbBytes)
			{
				if (DispLog) printf("Send() Insufficient size of buffer!\n");
				return 1;
			}
			if (DispLog) printf("Send() was successful.\n");
			return 1;
		}
	}
	BOOL Send(char * Buffer,size_t BufferSize=0)
	{
		DWORD cbBytes;
		return Send(Buffer,&cbBytes,BufferSize);		
	}
	
	BOOL Recv(char * Buffer,DWORD * cbBytes,size_t BufferSize=0)
	{
		if (BufferSize==0) 
		{
			BufferSize=MessageSize;//strlen(Buffer)+1;
		}

		BOOL bResult = ReadFile( 
			hPipe,              // handle to pipe 
			Buffer,				// buffer to receive data 
			BufferSize,			// size of buffer 
			cbBytes,            // number of bytes read 
			NULL);              // not overlapped I/O 
		
		if ( (!bResult) || (*cbBytes==0) ) 
		{
			if (DispLog) printf("Error occurred while reading" 
                 " from the server: %d\n", GetLastError()); 
			//CloseHandle(hPipe);	
			return 0; //ERROR
		}	
		else
		{
			if (BufferSize != *cbBytes)
			{
				if (DispLog) printf("Recv() Insufficient size of buffer!\n");
				return 1;
			}
			if (DispLog) printf("Recv() was successful.\n");
			return 1;
		}
	}	
	BOOL Recv(char * Buffer,size_t BufferSize=0)
	{
		DWORD cbBytes;
		return Recv(Buffer,&cbBytes,BufferSize);		
	}
};

class Pipe_Server: public Pipe
{
	char * PipeAddr; 
	bool status;
	int MaxMessage;
	WCHAR WPipeAddr[109];
public:
	Pipe_Server(char * PipeName, size_t MessageSize_=MESSAGE_SIZE, int MaxMessage_=MAX_MESSAGE)
	{
		MaxMessage=MaxMessage_;
		MessageSize=MessageSize_;
		PipeAddr=new char[strlen(PipeName)+strlen(PIPE_HANDLE)+1];
		strcpy_s(PipeAddr,strlen(PIPE_HANDLE)+1,PIPE_HANDLE);
		strcpy_s(PipeAddr+strlen(PIPE_HANDLE),strlen(PipeName)+1,PipeName);
		status=false;
		OemToChar(PipeAddr,WPipeAddr);
	}
	bool Connect()
	{
		hPipe = CreateNamedPipe(
			WPipeAddr,//g_szPipeName,             // pipe name 
			PIPE_ACCESS_DUPLEX,       // read/write access 
			PIPE_TYPE_MESSAGE |       // message type pipe 
			PIPE_READMODE_MESSAGE |   // message-read mode 
			PIPE_WAIT,                // blocking mode 
			PIPE_UNLIMITED_INSTANCES, // max. instances  
			BUFFER_SIZE,              // output buffer size
			BUFFER_SIZE,              // input buffer size 
			NMPWAIT_USE_DEFAULT_WAIT, // client time-out 
			NULL);                    // default security attribute 
		if (INVALID_HANDLE_VALUE == hPipe) 
		{
			if (DispLog) printf("Error occurred while creating the pipe: %d\n", GetLastError()); 
		    return false;  //Error
			status=false;
		}
		else	
		{
		    if (DispLog) printf("CreateNamedPipe(%s) was successful.\n",PipeAddr);
			return true;
			status=true;
		}
	}
	~Pipe_Server()
	{	
		if (status) CloseHandle(hPipe);
	}
	bool listen()
	{
		if (DispLog) printf("Waiting for client connection...\n");	//Wait for the client to connect
		BOOL bClientConnected = ConnectNamedPipe(hPipe, NULL);
		if (FALSE == bClientConnected)	
		{
			if (DispLog) printf("Error occurred while connecting to the client: %d\n", GetLastError()); 
			status=false;
			CloseHandle(hPipe);
			return 0;  //Error
		}
		else
		{
			if (DispLog) printf("ConnectNamedPipe() was successful.\n");
			return 1;
		}
	}
};

#endif