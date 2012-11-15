#ifndef RFCLIENT_H
#define RFCLIENT_H

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <QThread>
#include <windows.h>
#include <process.h>
#include "robocup_ssl_client.h"
#include "timer.h"

#include "messages_robocup_ssl_detection.pb.h"
#include "messages_robocup_ssl_geometry.pb.h"
#include "messages_robocup_ssl_wrapper.pb.h"

#include "rfprotocol.h"
#include "engine.h"
#include "Pipe.h"

#define TEAM_COUNT 48
#define BALL_COUNT 3
#define TEAM_COUNT_d BALL_COUNT*sizeof(double)
#define BALL_COUNT_d BALL_COUNT*sizeof(double)

using namespace std;

typedef struct {
    Engine  *ep;
    mxArray *Rules_length;
    mxArray *Rules_count;
    mxArray *Ball;
    mxArray *Blue;
    mxArray *Yellow;
    mxArray *Rule;
    mxArray *Back_Params;
    mxArray *Back_Amount;
    mxArray *Back_Length;
} SEngine;

typedef struct {
    double *Ball;
    double *Blue;
    double *Yellow;
    double *Rule;
    double *Back_Params;
} SDate;

typedef struct {
    string	name;
    const char*  file_of_matlab;
//    string  f_pipe; // pipe file name of bluetooth server
    int		RULE_AMOUNT;
    int		RULE_LENGTH;
    int		BACK_AMOUNT;
    int     BACK_LENGTH;
} RCConfig;

class RfClient {

protected:

    RCConfig config;      // struct config
    SEngine seng;         // struct engine
    SDate sdat;           // struct date

//    HANDLE hMutexBC;      // mutex bluetooth pipe client
//    HANDLE hMutexSC;      // mutex SSL client

    Pipe_Client BtClient; // bluetooth pipe client

//    vector<ExchangePacket> regulatorsRequestQueue;
//    vector<ExchangePacket> clientsPacketsQueue;

//    static unsigned __stdcall ThreadBC(void* pParam); //thread bluetooth pipe client
//    unsigned __stdcall ThreadSC(void* pParam); //thread SSL client


    int bindPipe();

    void daemonize();
    void mainloop();
    //void commit();

public:

    RfClient(RCConfig conf);
    ~RfClient(){
        mxDestroyArray(seng.Ball);
        mxDestroyArray(seng.Blue);
        mxDestroyArray(seng.Yellow);
        mxDestroyArray(seng.Rules_length);
        mxDestroyArray(seng.Rules_count);
        mxDestroyArray(seng.Rule);
        mxDestroyArray(seng.Back_Params);
        mxDestroyArray(seng.Back_Amount);
        mxDestroyArray(seng.Back_Length);
        free(sdat.Blue);
        free(sdat.Yellow);
        free(sdat.Rule);
        free(sdat.Ball);
        free(sdat.Back_Params);
    }

    /**
     * Start the server daemon and run its main loop
     */
    void exec();

};

#endif // RFCLIENT_H
