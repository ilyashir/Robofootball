#pragma once
#ifndef PIPE_H
#define PIPE_H

#include <winsock2.h>
#include <stdio.h>
#include "Windows.h"
//using namespace std;


#define BUFFER_SIZE MessageSize*MaxMessage 
//Name given to the pipe
#define PIPE_HANDLE "\\\\.\\pipe\\"
//Pipe name format - \\servername\pipe\pipename

//This pipe is for server on the same computer, 
//however, pipes can be used to
//connect to a remote server

class Pipe
{
public:
	char * PipeAddr; 
    wchar_t * wPipeAddr;
	HANDLE hPipe;
	size_t MessageSize;	
	bool DispLog;
	bool status;
	virtual bool Connect()=0;
	OVERLAPPED * overlap;
	Pipe()
	{
		DispLog=true;
		status=false;
		overlap=new OVERLAPPED;
		memset(overlap, 0, sizeof *overlap);
		overlap->hEvent= CreateEvent( 
         NULL,    //  default security attribute 
         TRUE,    // manual-reset event 
         TRUE,    // initial state = signaled 
        NULL);   // unnamed event object 
		if (overlap == NULL) 
        {
			printf("CreateEvent failed with %d.\n", GetLastError()); 
        }
	}
	int Write(char * Buffer,DWORD * cbBytes,size_t BufferSize=0)
	{
		//Sleep(1);//sorry
		if (BufferSize==0) 
		{
			BufferSize=MessageSize;
		}
		if( WriteFile( 
			hPipe,                // handle to pipe 
			Buffer,             // buffer to write from
			BufferSize,   // number of 233......bytes to write, include the NULL
			cbBytes,             // number of bytes written 
			 overlap
			 ))                // not overlapped I/O 
		{
			if (*cbBytes==0)
			{
				if (DispLog) printf("Send() failed!\n");
				return 2;
			}
			if (BufferSize != *cbBytes)
			{
				if (DispLog) printf("Send() Insufficient size of buffer!\n");
				return 3;
			}
			if (DispLog) printf("Send() was successful.\n");
			return 1;
		}
		else
		{
			CancelIo(hPipe);
			int ERR=GetLastError();
			if (ERR==ERROR_IO_PENDING)
			{
				return ERROR_IO_PENDING;
			}		
			if (DispLog) printf("Error occurred while recving from the client: %d\n", ERR); 
			//CloseHandle(hPipe);
			return 0;  //Error
		}
	}
	int Read(char * Buffer,DWORD * cbBytes,size_t BufferSize=0)
	{
		//Sleep(1);//sorry =(
		if (BufferSize==0) 
		{
			BufferSize=MessageSize;
		}
		if (
			ReadFile( 
			hPipe,              // handle to pipe 
			Buffer,				// buffer to receive data 
			BufferSize,			// size of buffer 
			cbBytes,            // number of bytes read 
			overlap)// overlapped I/O
			)      
		{
			if (*cbBytes==0)
			{
				if (DispLog) printf("Recv() failed!\n");
				return 2;
			}
			if (BufferSize != *cbBytes)
			{
				if (DispLog) printf("Recv() Insufficient size of buffer!\n");
				return 3;
			}
			if (DispLog) printf("Recv() was successful.\n");
			return 1;
		}
		else
		{
			CancelIo(hPipe);
			int ERR=GetLastError();
			if (ERR==ERROR_IO_PENDING)
			{
				return ERROR_IO_PENDING;
			}		
			if (DispLog) printf("Error occurred while recving from the client: %d\n", ERR); 
			//CloseHandle(hPipe);	
			return 0; //ERROR
		}
	}	
	int Write(char * Buffer,size_t BufferSize=0)
	{
		DWORD cbBytes;
		return Write(Buffer,&cbBytes,BufferSize);		
	}
	int Read(char * Buffer,size_t BufferSize=0)
	{
		DWORD cbBytes;
		return Read(Buffer,&cbBytes,BufferSize);		
	}
	int Recv(char * Buffer,size_t BufferSize=0)
	{
		int re;	
		while ((re=Read(Buffer,BufferSize))==ERROR_IO_PENDING) 
			Sleep(1);
		return re;
	}
	bool Send(char * Buffer,size_t BufferSize=0)
	{
		int re;	
		while ((re=Write(Buffer,BufferSize))==ERROR_IO_PENDING)
			Sleep(1);
		return re;
	}
};

class Pipe_Client: public Pipe
{
public:
    Pipe_Client()
    {
        MessageSize=100;
        char * PipeName = "addrPipe";
        PipeAddr=new char[strlen(PipeName)+strlen(PIPE_HANDLE)+1];
        strcpy_s(PipeAddr,strlen(PIPE_HANDLE)+1,PIPE_HANDLE);
        strcpy_s(PipeAddr+strlen(PIPE_HANDLE),strlen(PipeName)+1,PipeName);

        int len = strlen(PipeAddr)+1;
        wPipeAddr = new wchar_t[len];

        memset(wPipeAddr,0,len);
        ::MultiByteToWideChar(  CP_ACP, NULL,PipeAddr, -1, wPipeAddr,len );
    }
	Pipe_Client(char * PipeName, size_t MessageSize_)
	{
		MessageSize=MessageSize_;
		PipeAddr=new char[strlen(PipeName)+strlen(PIPE_HANDLE)+1];
		strcpy_s(PipeAddr,strlen(PIPE_HANDLE)+1,PIPE_HANDLE);
		strcpy_s(PipeAddr+strlen(PIPE_HANDLE),strlen(PipeName)+1,PipeName);    

        int len = strlen(PipeAddr)+1;
        wPipeAddr = new wchar_t[len];

        memset(wPipeAddr,0,len);
        ::MultiByteToWideChar(  CP_ACP, NULL,PipeAddr, -1, wPipeAddr,len );
	}
    Pipe_Client(Pipe_Client &pc){
        MessageSize=pc.MessageSize;
        wPipeAddr = pc.wPipeAddr;
    }
	bool Connect()
	{
        if (DispLog) printf("Openning Pipe Server by address: %s\n",PipeAddr);
		//Connect to the server pipe using CreateFile()
	    hPipe = CreateFile( 
        wPipeAddr,   // pipe name
	    GENERIC_READ |  // read and write access 
	    GENERIC_WRITE, 
	    0,              // no sharing 
	    NULL,           // default security attributes
	    OPEN_ALWAYS,  // opens existing pipe
	    FILE_FLAG_OVERLAPPED,              // default attributes 
	    NULL);          // no template file 
		ConnectNamedPipe(hPipe,0);
		if (INVALID_HANDLE_VALUE == hPipe) 
	    {
			if (DispLog) printf("Error occurred while connecting" 
				" to the server: %d\n", GetLastError()); 
			//One might want to check whether the server pipe is busy
			//This sample will error out if the server pipe is busy
			//Read on ERROR_PIPE_BUSY and WaitNamedPipe() for that
			return 0;  //Error
	    }
	    else
	    {
			if (DispLog) printf("CreateFile() was successful.\n");
			return 1;
		}
		//We are done connecting to the server pipe,
	    //we can start communicating with 
	    //the server using ReadFile()/WriteFile() 
	    //on handle - hPipe
	}
	~Pipe_Client()
	{
		CloseHandle(hPipe);
	}
};

class Pipe_Server: public Pipe
{
	int MaxMessage;
public:
    Pipe_Server(){
        MaxMessage=100;
        MessageSize=100;
        char *PipeName = "addrPipe";
        PipeAddr=new char[strlen(PipeName)+strlen(PIPE_HANDLE)+1];
        strcpy_s(PipeAddr,strlen(PIPE_HANDLE)+1,PIPE_HANDLE);
        strcpy_s(PipeAddr+strlen(PIPE_HANDLE),strlen(PipeName)+1,PipeName);

        int len = strlen(PipeAddr)+1;
        wPipeAddr = new wchar_t[len];

        memset(wPipeAddr,0,len);
        ::MultiByteToWideChar(  CP_ACP, NULL,PipeAddr, -1, wPipeAddr,len );
    }

	Pipe_Server(char * PipeName, size_t MessageSize_,int MaxMessage_)
	{
		MaxMessage=MaxMessage_;
		MessageSize=MessageSize_;
		PipeAddr=new char[strlen(PipeName)+strlen(PIPE_HANDLE)+1];
		strcpy_s(PipeAddr,strlen(PIPE_HANDLE)+1,PIPE_HANDLE);
		strcpy_s(PipeAddr+strlen(PIPE_HANDLE),strlen(PipeName)+1,PipeName);

        int len = strlen(PipeAddr)+1;
        wPipeAddr = new wchar_t[len];

        memset(wPipeAddr,0,len);
        ::MultiByteToWideChar(  CP_ACP, NULL,PipeAddr, -1, wPipeAddr,len );
	}

    Pipe_Server(Pipe_Server &ps){
        MaxMessage=ps.MaxMessage;
        MessageSize=ps.MessageSize;
        wPipeAddr = ps.wPipeAddr;
    }

	bool Connect()
	{
		hPipe = CreateNamedPipe(
            wPipeAddr,             // pipe name
			PIPE_ACCESS_DUPLEX |  // read/write access 
			FILE_FLAG_OVERLAPPED,      
			PIPE_TYPE_MESSAGE |       // message type pipe 
			PIPE_READMODE_MESSAGE |   // message-read mode 
			PIPE_WAIT,                // blocking mode 
			PIPE_UNLIMITED_INSTANCES, // max. instances  
			BUFFER_SIZE,              // output buffer size
			BUFFER_SIZE,              // input buffer size 
			5000,//NMPWAIT_USE_DEFAULT_WAIT, // client time-out 
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
	bool listen()
	{
		if (DispLog) printf("Waiting for client connection...\n");	//Wait for the client to connect
		BOOL bClientConnected = ConnectNamedPipe(hPipe, 0);
        
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
	~Pipe_Server()
	{	
		if (status) CloseHandle(hPipe);
	}
};
#endif
