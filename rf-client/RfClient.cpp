#include "RfClient.h"

RfClient::RfClient(RCConfig conf)
{    
    memset(&config, 0, sizeof(RCConfig));
    memcpy(&config, &conf, sizeof(RCConfig));

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
    cout << endl;
    for(int i=0; i<config.BACK_AMOUNT*config.BACK_LENGTH; ++i){
        sdat.Back_Params[i]=0;
        cout << sdat.Back_Params[i] << " ";
    }
    cout << "create back params - OK" << endl;
    for(int i=0; i<TEAM_COUNT; ++i){
        sdat.Blue[i]=0;
        sdat.Yellow[i]=0;
        cout << sdat.Blue[i] << " ";
    }
    cout << "create teams params - OK" << endl;
    for(int i=0; i<config.RULE_LENGTH*config.RULE_AMOUNT; ++i){
        sdat.Rule[i]=0;
    }
    cout << "create rf-clien object - OK"  << endl;

}

void printRobotInfo(const SSL_DetectionRobot & robot) {
    printf("CONF=%4.2f ", robot.confidence());
    if (robot.has_robot_id()) {
        printf("ID=%3d ",robot.robot_id());
    } else {
        printf("ID=N/A ");
    }
    printf(" HEIGHT=%6.2f POS=<%9.2f,%9.2f> ",robot.height(),robot.x(),robot.y());
    if (robot.has_orientation()) {
        printf("ANGLE=%6.3f ",robot.orientation());
    } else {
        printf("ANGLE=N/A    ");
    }
    printf("RAW=<%8.2f,%8.2f>\n",robot.pixel_x(),robot.pixel_y());
}

int RfClient::bindPipe(){

    cout << "bind!" << endl;

    if (BtClient.Connect())
    {
        BtClient.DispLog=false;
    }
    else
        return -1;

    return 0;

}

void RfClient::exec(){

    if(bindPipe()) {
        cerr << "BT Server daemon not started." << endl;
        return;
    }

    cout << "bind ok" << endl;

    daemonize();
}

void RfClient::daemonize(){

    mainloop();

    exit(0);

}


//
//unsigned __stdcall RfClient::ThreadBC(void* pParam){

//    char buffer[MESSAGE_SIZE*MAX_MESSAGE];
//    int re;

//    while(true){

//        WaitForSingleObject(hMutexBC, INFINITE);

//        cout << "+";

//        if(re=BtClient.Recv(buffer))
//        {
//            cout << "Recv message:" << buffer << endl;
//            if (re!=1)
//                printf("CORRUPTED BUFFER by Server\n");

//            //                if (!(re=Discript(buffer)))
//            //                {
//            //                    printf("!!!ERROR MESSAGEs!!! %d\n",re);
//            //                }
//            if(buffer[0]==4)
//                cout << "GOOD!" << endl;
//            memcpy((char *) mxGetPr(seng.Back_Params), (char*) sdat.Back_Params, config.BACK_AMOUNT*sizeof(double));
//            engPutVariable(seng.ep, "Back_Params", seng.Back_Params);

//        }

//        Sleep(1);

//        ReleaseMutex(hMutexBC);
//    }

//}

////
//unsigned __stdcall ThreadSC(void* pParam){

//    RfClient *rc = pParam;
//    // create RoboCupSSLClient
//    RoboCupSSLClient client;
//    client.open(true);
//    SSL_WrapperPacket packet;

//    while(true) {

//        WaitForSingleObject(hMutexSC, INFINITE);

//        cout << "+";

//        if (client.receive(packet)) {
//            printf("-----Received Wrapper Packet---------------------------------------------\n");
//            //see if the packet contains a robot detection frame:
//            if (packet.has_detection()) {
//                SSL_DetectionFrame detection = packet.detection();
//                //Display the contents of the robot detection results:
//                //double t_now = GetTimeSec();

//    //                printf("-[Detection Data]-------\n");
//    //                //Frame info:
//    //                printf("Camera ID=%d FRAME=%d T_CAPTURE=%.4f\n",detection.camera_id(),detection.frame_number(),detection.t_capture());

//    //                printf("SSL-Vision Processing Latency                   %7.3fms\n",(detection.t_sent()-detection.t_capture())*1000.0);
//    //                printf("Network Latency (assuming synched system clock) %7.3fms\n",(t_now-detection.t_sent())*1000.0);
//    //                printf("Total Latency   (assuming synched system clock) %7.3fms\n",(t_now-detection.t_capture())*1000.0);
//                int idCam=detection.camera_id()+1;
//                int balls_n = detection.balls_size();
//                int robots_blue_n =  detection.robots_blue_size();
//                int robots_yellow_n =  detection.robots_yellow_size();

//                printf("Yes!");

//    //                //Ball info:
//    //                for (int i = 0; i < balls_n; i++) {
//    //                    SSL_DetectionBall ball = detection.balls(i);
//    //                    printf("-Ball (%2d/%2d): CONF=%4.2f POS=<%9.2f,%9.2f> ", i+1, balls_n, ball.confidence(),ball.x(),ball.y());
//    //                    if (ball.has_z()) {
//    //                        printf("Z=%7.2f ",ball.z());
//    //                    } else {
//    //                        printf("Z=N/A   ");
//    //                    }
//    //                    printf("RAW=<%8.2f,%8.2f>\n",ball.pixel_x(),ball.pixel_y());
//    //                }

//    //                //Blue robot info:
//    //                for (int i = 0; i < robots_blue_n; i++) {
//    //                    SSL_DetectionRobot robot = detection.robots_blue(i);
//    //                    printf("-Robot(B) (%2d/%2d): ",i+1, robots_blue_n);
//    //                    printRobotInfo(robot);
//    //                }

//    //                //Yellow robot info:
//    //                for (int i = 0; i < robots_yellow_n; i++) {
//    //                    SSL_DetectionRobot robot = detection.robots_yellow(i);
//    //                    printf("-Robot(Y) (%2d/%2d): ",i+1, robots_yellow_n);
//    //                    printRobotInfo(robot);
//    //                }

//                if(balls_n!=0){
//                    rc->sdat.Ball[0]=idCam;
//                    SSL_DetectionBall ball = detection.balls(0);
//                    rc->sdat.Ball[1]=ball.x();
//                    rc->sdat.Ball[2]=ball.y();
//                }
//                else
//                    if(rc->sdat.Ball[0]==idCam)
//                        rc->sdat.Ball[0]=0;

//                memcpy((char *) mxGetPr(rc->seng.Ball), (char *) rc->sdat.Ball, BALL_COUNT_d);
//                engPutVariable(rc->seng.ep, "Balls", rc->seng.Ball);

//                for (int j = 0; j < TEAM_COUNT/4; j++)
//                    if(rc->sdat.Blue[j]==idCam)
//                        rc->sdat.Blue[j]=0;
//                for (int i = 0; i < robots_blue_n; i++) {
//                    SSL_DetectionRobot robot = detection.robots_blue(i);
//                    rc->sdat.Blue[robot.robot_id()]=idCam;
//                    rc->sdat.Blue[robot.robot_id()+12]=robot.x();
//                    rc->sdat.Blue[robot.robot_id()+24]=robot.y();
//                    rc->sdat.Blue[robot.robot_id()+36]=robot.orientation();
//                }

//                memcpy((char *) mxGetPr(rc->seng.Blue), (char *) rc->sdat.Blue, TEAM_COUNT_d);
//                engPutVariable(rc->seng.ep, "Blues", rc->seng.Blue);

//                for (int j = 0; j < TEAM_COUNT/4; j++)
//                    if(rc->sdat.Yellow[j]==idCam)
//                        rc->sdat.Yellow[j]=0;
//                for (int i = 0; i < robots_yellow_n; i++) {
//                    SSL_DetectionRobot robot = detection.robots_yellow(i);
//                    rc->sdat.Yellow[robot.robot_id()]=idCam;
//                    rc->sdat.Yellow[robot.robot_id()+12]=robot.x();
//                    rc->sdat.Yellow[robot.robot_id()+24]=robot.y();
//                    rc->sdat.Yellow[robot.robot_id()+36]=robot.orientation();
//                }

//                memcpy((char *) mxGetPr(seng.Yellow), (char *) sdat.Yellow, TEAM_COUNT_d);
//                engPutVariable(seng.ep, "Yellows", seng.Yellow);

//            }

//            //see if packet contains geometry data:
//    //            if (packet.has_geometry()) {
//    //                const SSL_GeometryData & geom = packet.geometry();
//    //                printf("-[Geometry Data]-------\n");

//    //                const SSL_GeometryFieldSize & field = geom.field();
//    //                printf("Field Dimensions:\n");
//    //                printf("  -line_width=%d (mm)\n",field.line_width());
//    //                printf("  -field_length=%d (mm)\n",field.field_length());
//    //                printf("  -field_width=%d (mm)\n",field.field_width());
//    //                printf("  -boundary_width=%d (mm)\n",field.boundary_width());
//    //                printf("  -referee_width=%d (mm)\n",field.referee_width());
//    //                printf("  -goal_width=%d (mm)\n",field.goal_width());
//    //                printf("  -goal_depth=%d (mm)\n",field.goal_depth());
//    //                printf("  -goal_wall_width=%d (mm)\n",field.goal_wall_width());
//    //                printf("  -center_circle_radius=%d (mm)\n",field.center_circle_radius());
//    //                printf("  -defense_radius=%d (mm)\n",field.defense_radius());
//    //                printf("  -defense_stretch=%d (mm)\n",field.defense_stretch());
//    //                printf("  -free_kick_from_defense_dist=%d (mm)\n",field.free_kick_from_defense_dist());
//    //                printf("  -penalty_spot_from_field_line_dist=%d (mm)\n",field.penalty_spot_from_field_line_dist());
//    //                printf("  -penalty_line_from_spot_dist=%d (mm)\n",field.penalty_line_from_spot_dist());

//    //                int calib_n = geom.calib_size();
//    //                for (int i=0; i< calib_n; i++) {
//    //                    const SSL_GeometryCameraCalibration & calib = geom.calib(i);
//    //                    printf("Camera Geometry for Camera ID %d:\n", calib.camera_id());
//    //                    printf("  -focal_length=%.2f\n",calib.focal_length());
//    //                    printf("  -principal_point_x=%.2f\n",calib.principal_point_x());
//    //                    printf("  -principal_point_y=%.2f\n",calib.principal_point_y());
//    //                    printf("  -distortion=%.2f\n",calib.distortion());
//    //                    printf("  -q0=%.2f\n",calib.q0());
//    //                    printf("  -q1=%.2f\n",calib.q1());
//    //                    printf("  -q2=%.2f\n",calib.q2());
//    //                    printf("  -q3=%.2f\n",calib.q3());
//    //                    printf("  -tx=%.2f\n",calib.tx());
//    //                    printf("  -ty=%.2f\n",calib.ty());
//    //                    printf("  -tz=%.2f\n",calib.tz());

//    //                    if (calib.has_derived_camera_world_tx() && calib.has_derived_camera_world_ty() && calib.has_derived_camera_world_tz()) {
//    //                      printf("  -derived_camera_world_tx=%.f\n",calib.derived_camera_world_tx());
//    //                      printf("  -derived_camera_world_ty=%.f\n",calib.derived_camera_world_ty());
//    //                      printf("  -derived_camera_world_tz=%.f\n",calib.derived_camera_world_tz());
//    //                    }

//    //                }
//    //            }

//            Sleep(1);
//            ReleaseMutex(hMutexSC);

//        }
//    } // end while
//}


void RfClient::mainloop(){

    cout << "Work is started" << endl;
    if (!(seng.ep = engOpen(NULL))) {
            cerr << "Can't to Open Matlab Engine" << endl;
            //return 2;
        }

    // matlab buffer
    char m_buffer[256]={0};
    m_buffer[255]='\0';
    engOutputBuffer(seng.ep, m_buffer, 255);

    //-------Path_Of_Files-------
    TCHAR CurrentPath[MAX_PATH];
    char StringWithPath[MAX_PATH],StringPath[MAX_PATH];

    //-----Detect Directory-------
    printf("Matlab Engine is opened\nStart to detect directory\n");
    GetCurrentDirectory(sizeof(CurrentPath),CurrentPath);

#ifdef UNICODE
    wcstombs(StringPath, CurrentPath, MAX_PATH-1);
#else
    CharToOem(StringPath, CurrentPath);
#endif

    sprintf(StringWithPath, "cd %s;", StringPath);
    engEvalString(seng.ep, StringWithPath);
    printf("We are on %s\n",StringWithPath);

    //-----create Rules-----
    char sendString[256];
    sprintf (sendString, "Rules=zeros(%d, %d)", config.RULE_LENGTH, config.RULE_AMOUNT);
    engEvalString(seng.ep, sendString);
    engEvalString(seng.ep, "disp(1)");

    //    
//    hMutexBC = CreateMutex(NULL, FALSE, NULL);
//    hMutexSC = CreateMutex(NULL, FALSE, NULL);

//    DWORD dwID;
//    HANDLE hThreadBC = (HANDLE)_beginthreadex(NULL, 0, ThreadBC, NULL, 0, &dwID);
//    HANDLE hThreadSC = (HANDLE)_beginthreadex(NULL, 0, ThreadSC, NULL, 0, &dwID);


//    double RULES_LENGTH=config.RULE_LENGTH;
//    memcpy((char *) mxGetPr(seng.Rules_length), (char*) &RULES_LENGTH, 1*sizeof(double));
//    engPutVariable(seng.ep, "Rules_length", seng.Rules_length);

//    double RULES_AMOUNT=config.RULE_AMOUNT;
//    memcpy((char *) mxGetPr(seng.Rules_count), (char*) &RULES_AMOUNT, 1*sizeof(double));
//    engPutVariable(seng.ep, "Rules_amount", seng.Rules_count);

//    double BACKS_AMOUNT=config.BACK_AMOUNT;
//    memcpy((char *) mxGetPr(seng.Back_Amount), (char*) &BACKS_AMOUNT, 1*sizeof(double));
//    engPutVariable(seng.ep, "Back_amount", seng.Back_Amount);

//    double BACKS_LENGTH=config.BACK_LENGTH;
//    memcpy((char *) mxGetPr(seng.Back_Length), (char*) &BACKS_LENGTH, 1*sizeof(double));
//    engPutVariable(seng.ep, "Back_length", seng.Back_Length);

    // create RoboCupSSLClient
    RoboCupSSLClient client;
    client.open(true);
    SSL_WrapperPacket packet;

    // bt buffer
    char buffer[MESSAGE_SIZE*MAX_MESSAGE];
    int re;

    //-------Pack to Send_BT--------
    char pack[100]={0};

    // Work with packs

    while(true){

//        WaitForSingleObject(hMutexBC, INFINITE);
//        WaitForSingleObject(hMutexSC, INFINITE);
        if (client.receive(packet)) {
            printf("-----Received Wrapper Packet---------------------------------------------\n");
            //see if the packet contains a robot detection frame:
            if (packet.has_detection()) {
                SSL_DetectionFrame detection = packet.detection();
                //Display the contents of the robot detection results:
                //double t_now = GetTimeSec();

    //                printf("-[Detection Data]-------\n");
    //                //Frame info:
    //                printf("Camera ID=%d FRAME=%d T_CAPTURE=%.4f\n",detection.camera_id(),detection.frame_number(),detection.t_capture());

    //                printf("SSL-Vision Processing Latency                   %7.3fms\n",(detection.t_sent()-detection.t_capture())*1000.0);
    //                printf("Network Latency (assuming synched system clock) %7.3fms\n",(t_now-detection.t_sent())*1000.0);
    //                printf("Total Latency   (assuming synched system clock) %7.3fms\n",(t_now-detection.t_capture())*1000.0);
                int idCam=detection.camera_id()+1;
                int balls_n = detection.balls_size();
                int robots_blue_n =  detection.robots_blue_size();
                int robots_yellow_n =  detection.robots_yellow_size();

                cout << "Received SSL Packet" << endl;

    //                //Ball info:
    //                for (int i = 0; i < balls_n; i++) {
    //                    SSL_DetectionBall ball = detection.balls(i);
    //                    printf("-Ball (%2d/%2d): CONF=%4.2f POS=<%9.2f,%9.2f> ", i+1, balls_n, ball.confidence(),ball.x(),ball.y());
    //                    if (ball.has_z()) {
    //                        printf("Z=%7.2f ",ball.z());
    //                    } else {
    //                        printf("Z=N/A   ");
    //                    }
    //                    printf("RAW=<%8.2f,%8.2f>\n",ball.pixel_x(),ball.pixel_y());
    //                }

    //                //Blue robot info:
    //                for (int i = 0; i < robots_blue_n; i++) {
    //                    SSL_DetectionRobot robot = detection.robots_blue(i);
    //                    printf("-Robot(B) (%2d/%2d): ",i+1, robots_blue_n);
    //                    printRobotInfo(robot);
    //                }

    //                //Yellow robot info:
    //                for (int i = 0; i < robots_yellow_n; i++) {
    //                    SSL_DetectionRobot robot = detection.robots_yellow(i);
    //                    printf("-Robot(Y) (%2d/%2d): ",i+1, robots_yellow_n);
    //                    printRobotInfo(robot);
    //                }

                if(balls_n!=0){
                    sdat.Ball[0]=idCam;
                    SSL_DetectionBall ball = detection.balls(0);
                    sdat.Ball[1]=ball.x();
                    sdat.Ball[2]=ball.y();
                }
                else
                    if(sdat.Ball[0]==idCam)
                        sdat.Ball[0]=0;

                memcpy((char *) mxGetPr(seng.Ball), (char *) sdat.Ball, BALL_COUNT_d);
                engPutVariable(seng.ep, "Balls", seng.Ball);

                for (int j = 0; j < TEAM_COUNT/4; j++)
                    if(sdat.Blue[j]==idCam)
                        sdat.Blue[j]=0;
                for (int i = 0; i < robots_blue_n; i++) {
                    SSL_DetectionRobot robot = detection.robots_blue(i);
                    sdat.Blue[robot.robot_id()]=idCam;
                    sdat.Blue[robot.robot_id()+12]=robot.x();
                    sdat.Blue[robot.robot_id()+24]=robot.y();
                    sdat.Blue[robot.robot_id()+36]=robot.orientation();
                }

                memcpy((char *) mxGetPr(seng.Blue), (char *) sdat.Blue, TEAM_COUNT_d);
                engPutVariable(seng.ep, "Blues", seng.Blue);

                for (int j = 0; j < TEAM_COUNT/4; j++)
                    if(sdat.Yellow[j]==idCam)
                        sdat.Yellow[j]=0;
                for (int i = 0; i < robots_yellow_n; i++) {
                    SSL_DetectionRobot robot = detection.robots_yellow(i);
                    sdat.Yellow[robot.robot_id()]=idCam;
                    sdat.Yellow[robot.robot_id()+12]=robot.x();
                    sdat.Yellow[robot.robot_id()+24]=robot.y();
                    sdat.Yellow[robot.robot_id()+36]=robot.orientation();
                }

                memcpy((char *) mxGetPr(seng.Yellow), (char *) sdat.Yellow, TEAM_COUNT_d);
                engPutVariable(seng.ep, "Yellows", seng.Yellow);

            }

            //see if packet contains geometry data:
    //            if (packet.has_geometry()) {
    //                const SSL_GeometryData & geom = packet.geometry();
    //                printf("-[Geometry Data]-------\n");

    //                const SSL_GeometryFieldSize & field = geom.field();
    //                printf("Field Dimensions:\n");
    //                printf("  -line_width=%d (mm)\n",field.line_width());
    //                printf("  -field_length=%d (mm)\n",field.field_length());
    //                printf("  -field_width=%d (mm)\n",field.field_width());
    //                printf("  -boundary_width=%d (mm)\n",field.boundary_width());
    //                printf("  -referee_width=%d (mm)\n",field.referee_width());
    //                printf("  -goal_width=%d (mm)\n",field.goal_width());
    //                printf("  -goal_depth=%d (mm)\n",field.goal_depth());
    //                printf("  -goal_wall_width=%d (mm)\n",field.goal_wall_width());
    //                printf("  -center_circle_radius=%d (mm)\n",field.center_circle_radius());
    //                printf("  -defense_radius=%d (mm)\n",field.defense_radius());
    //                printf("  -defense_stretch=%d (mm)\n",field.defense_stretch());
    //                printf("  -free_kick_from_defense_dist=%d (mm)\n",field.free_kick_from_defense_dist());
    //                printf("  -penalty_spot_from_field_line_dist=%d (mm)\n",field.penalty_spot_from_field_line_dist());
    //                printf("  -penalty_line_from_spot_dist=%d (mm)\n",field.penalty_line_from_spot_dist());

    //                int calib_n = geom.calib_size();
    //                for (int i=0; i< calib_n; i++) {
    //                    const SSL_GeometryCameraCalibration & calib = geom.calib(i);
    //                    printf("Camera Geometry for Camera ID %d:\n", calib.camera_id());
    //                    printf("  -focal_length=%.2f\n",calib.focal_length());
    //                    printf("  -principal_point_x=%.2f\n",calib.principal_point_x());
    //                    printf("  -principal_point_y=%.2f\n",calib.principal_point_y());
    //                    printf("  -distortion=%.2f\n",calib.distortion());
    //                    printf("  -q0=%.2f\n",calib.q0());
    //                    printf("  -q1=%.2f\n",calib.q1());
    //                    printf("  -q2=%.2f\n",calib.q2());
    //                    printf("  -q3=%.2f\n",calib.q3());
    //                    printf("  -tx=%.2f\n",calib.tx());
    //                    printf("  -ty=%.2f\n",calib.ty());
    //                    printf("  -tz=%.2f\n",calib.tz());

    //                    if (calib.has_derived_camera_world_tx() && calib.has_derived_camera_world_ty() && calib.has_derived_camera_world_tz()) {
    //                      printf("  -derived_camera_world_tx=%.f\n",calib.derived_camera_world_tx());
    //                      printf("  -derived_camera_world_ty=%.f\n",calib.derived_camera_world_ty());
    //                      printf("  -derived_camera_world_tz=%.f\n",calib.derived_camera_world_tz());
    //                    }

    //                }
    //            }


        }

//        cout << "--------------Received BT packed-------------------" << endl;
//        if(re=BtClient.Recv(buffer))
//        {
//            cout << "Recv message:" << buffer << endl;
//            if (re!=1)
//                printf("CORRUPTED BUFFER by Server\n");

//            if(buffer[0]==4)
//                cout << "GOOD!" << endl;
//            memcpy((char *) mxGetPr(seng.Back_Params), (char*) sdat.Back_Params, config.BACK_AMOUNT*sizeof(double));
//            engPutVariable(seng.ep, "Back_Params", seng.Back_Params);

//        }
//        else
//        {
//            cout << "not received BT pack" << endl;
//        }

        cout << "--------------Received Matlab packed. Send to BT Server ------------------" << endl;
        if(m_buffer[5]=='1'){
            //-------Calculation of Rules Parameters--------
            engEvalString(seng.ep, config.file_of_matlab);

            m_buffer[5]='0';

            engEvalString(seng.ep, "disp(1)");
            // get Rules Parameters from Matlab
            seng.Rule = engGetVariable(seng.ep, "Rules");

            if(seng.Rule!=NULL){
                memcpy((char *) sdat.Rule, (char *)mxGetPr(seng.Rule), config.RULE_LENGTH*config.RULE_AMOUNT*sizeof(double));
                for(int i=0; i<config.RULE_LENGTH*config.RULE_AMOUNT; ++i){
                    cout << sdat.Rule[i] << " ";
                }
                cout << "get rule" << endl;
            }
            else
                cerr << "Can't Copy Rules" << endl;


            //------Sending by BT--------
            if(m_buffer[5]=='1'){

                for(int i=0;i<config.RULE_LENGTH;++i){
                    if((int)sdat.Rule[i]){
                        switch((int)sdat.Rule[i]){
                        case 1:
                            cout << "send to bt server pack: " << endl;
                            for(int j=0; j<config.RULE_AMOUNT; ++j){
                                pack[j]=(char)sdat.Rule[i+config.RULE_LENGTH*j];
                                printf("%d ", (char)sdat.Rule[i+config.RULE_LENGTH*j]);
                            }
                            break;
                        case 2:
                            printf("Message to BT\n");
                            for(int j=0; j<config.RULE_AMOUNT; ++j){
                                pack[j]=(char)sdat.Rule[i+config.RULE_LENGTH*j];
                                printf("%d ", (char)sdat.Rule[i+config.RULE_LENGTH*j]);
                            }
                            break;
                        case 3:
                            printf("Message to client\n");
                            break;
                        }

                        sprintf (sendString, "Rules=zeros(%d, %d)", config.RULE_LENGTH, config.RULE_AMOUNT);
                        engEvalString(seng.ep, sendString);
                        engEvalString(seng.ep, "disp(1)");

                        printf("%s\n", pack);
                        if(!(BtClient.Send(pack,100))){
                            printf("Can't Send to BT-Modul\n");
                            BtClient.Connect();
                        }
                    }
                }
            }

        }

        Sleep(1);


    } // end while

//    HANDLE hEvents[2] = {hThreadBC, hThreadSC};
//    WaitForMultipleObjects(2, hEvents, TRUE, INFINITE);

//    CloseHandle(hThreadBC);
//    CloseHandle(hThreadSC);
//    CloseHandle(hMutexBC);
//    CloseHandle(hMutexSC);


    engEvalString(seng.ep, "exit");
}
