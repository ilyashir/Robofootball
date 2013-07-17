#ifndef RFDATA_H
#define RFDATA_H

#include "robocup_ssl_client.h"
#include "timer.h"
#include "rfprotocol.h"
#include "engine.h"
#include "Pipe.h"

#define TEAM_COUNT 48
#define BALL_COUNT 3
#define TEAM_COUNT_d BALL_COUNT*sizeof(double)
#define BALL_COUNT_d BALL_COUNT*sizeof(double)

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

class RfData
{
public:

    RCConfig config;      // struct config
    SEngine seng;         // struct engine
    SDate sdat;           // struct date

    bool recb; // rec or not rec?

    Pipe_Client BtClient;       // bluetooth pipe client
    RoboCupSSLClient client;    // ssl client
    SSL_WrapperPacket packet;   // ssl wrapper packet

public:
    RfData(RCConfig* conf);
    RfData(RfData& dat);
    virtual ~RfData(){
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

//    void runMatlab();

};

#endif // RFDATA_H
