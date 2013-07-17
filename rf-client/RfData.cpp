#include "RfData.h"

RfData::RfData(RCConfig *conf)
{    
    memset(&config, 0, sizeof(RCConfig));
    memcpy(&config, conf, sizeof(RCConfig));

    Pipe_Client _Pipe(PIPE_NAME,MESSAGE_SIZE);

    cout << PIPE_NAME << endl;

    BtClient = _Pipe;

    for(int i=0; i<5; i++){
        cout << config.file_of_matlab[i];
    }
    cout << endl;
    cout << config.name << endl;
    cout << config.BACK_AMOUNT << endl;
    cout << config.BACK_LENGTH << endl;
    cout << config.RULE_AMOUNT << endl;
    cout << config.RULE_LENGTH << endl;

    seng.Rules_length = mxCreateDoubleMatrix(1, 1, mxREAL);
    seng.Rules_count  = mxCreateDoubleMatrix(1, 1, mxREAL);
    seng.Ball         = mxCreateDoubleMatrix(1, 3, mxREAL);
    seng.Blue         = mxCreateDoubleMatrix(12, 4, mxREAL);
    seng.Yellow       = mxCreateDoubleMatrix(12, 4, mxREAL);
    seng.Rule         = mxCreateDoubleMatrix(config.RULE_LENGTH, config.RULE_AMOUNT, mxREAL);
    seng.Back_Params  = mxCreateDoubleMatrix(config.BACK_LENGTH, config.BACK_AMOUNT, mxREAL);
    seng.Back_Amount  = mxCreateDoubleMatrix(1, 1, mxREAL);
    seng.Back_Length  = mxCreateDoubleMatrix(1, 1, mxREAL);

    sdat.Ball        = (double*)malloc(BALL_COUNT_d);
    sdat.Blue        = (double*)malloc(TEAM_COUNT_d);
    sdat.Yellow      = (double*)malloc(TEAM_COUNT_d);
    sdat.Rule        = (double*)malloc(config.RULE_LENGTH*config.RULE_AMOUNT*sizeof(double));
    sdat.Back_Params = (double*)malloc(config.BACK_AMOUNT*config.BACK_LENGTH*sizeof(double));

    //----Clear arrays----
    for(int i=0; i<BALL_COUNT; ++i){
        sdat.Ball[i]=0;
        cout << sdat.Ball[i] << " ";
    }
//    cout << endl;
//    for(int i=0; i<config.BACK_AMOUNT*config.BACK_LENGTH; ++i){
//        sdat.Back_Params[i]=0;
//        cout << sdat.Back_Params[i] << " ";
//    }

    cout << "create back params - OK" << endl;
    for(int i=0; i<TEAM_COUNT; ++i){
        sdat.Blue[i]=0;
        sdat.Yellow[i]=0;
        cout << i << ">>" << sdat.Blue[i] << " ";
    }

    cout << "create teams params - OK" << endl;
    for(int i=0; i<config.RULE_LENGTH*config.RULE_AMOUNT; ++i){
        sdat.Rule[i]=0;
    }
    cout << "create rf-clien object - OK"  << endl;
}

RfData::RfData(RfData &dat)
{
    memset(&config, 0, sizeof(RCConfig));
    memcpy(&config, &dat.config, sizeof(RCConfig));

    BtClient = dat.BtClient;

    seng.Rules_length = mxCreateDoubleMatrix(1, 1, mxREAL);
    seng.Rules_count  = mxCreateDoubleMatrix(1, 1, mxREAL);
    seng.Ball         = mxCreateDoubleMatrix(1, 3, mxREAL);
    seng.Blue         = mxCreateDoubleMatrix(12, 4, mxREAL);
    seng.Yellow       = mxCreateDoubleMatrix(12, 4, mxREAL);
    seng.Rule         = mxCreateDoubleMatrix(config.RULE_LENGTH, config.RULE_AMOUNT, mxREAL);
    seng.Back_Params  = mxCreateDoubleMatrix(config.BACK_LENGTH, config.BACK_AMOUNT, mxREAL);
    seng.Back_Amount  = mxCreateDoubleMatrix(1, 1, mxREAL);
    seng.Back_Length  = mxCreateDoubleMatrix(1, 1, mxREAL);

    sdat.Ball        = (double*)malloc(BALL_COUNT_d);
    sdat.Blue        = (double*)malloc(TEAM_COUNT_d);
    sdat.Yellow      = (double*)malloc(TEAM_COUNT_d);
    sdat.Rule        = (double*)malloc(config.RULE_LENGTH*config.RULE_AMOUNT*sizeof(double));
    sdat.Back_Params = (double*)malloc(config.BACK_AMOUNT*config.BACK_LENGTH*sizeof(double));

    memset(&seng, 0, sizeof(SEngine));
    memcpy(&seng, &dat.seng, sizeof(SEngine));

    for(int i=0; i<BALL_COUNT; ++i){
        sdat.Ball[i]=dat.sdat.Ball[i];
    }

    for(int i=0; i<TEAM_COUNT; ++i){
        sdat.Blue[i]=dat.sdat.Blue[i];
        sdat.Yellow[i]=dat.sdat.Yellow[i];
    }
    for(int i=0; i<config.RULE_LENGTH*config.RULE_AMOUNT; ++i){
        sdat.Rule[i]=dat.sdat.Rule[i];
    }



}
