#include "RfThread.h"

//int initConfig(RCConfig *config){
//    ifstream configFile;
//    string line;

//    configFile.open("rf-client.cnf", ios::in);

//    if(!configFile.is_open()){
//        cerr << "Can't open configuration file" << endl;
//        return -1;
//    }

//    if(!config){
//            cerr << "Can't parse configuration file" << endl;
//            return -2;
//        }

//    while(!configFile.eof()){
//        getline(configFile, line);

//        if(line.empty()){
//            continue;
//        }
//        if(line.find("rfclient.name") != string::npos){
//            config->name = line.substr(line.find('=') + 1);

//            if(config->name.size() > 16){
//                cerr << "[WARNING] rfclient.name value is too long." << endl << "It will be truncated up to 16 characters." << endl;
//                config->name.resize(16);
//            }
//        }

//        if(line.find("rfclient.file_of_matlab") != string::npos){
//            string fom = line.substr(line.find('=') + 1);

//            if(fom.size() > 16){
//                cerr << "[WARNING] rfclient.file_of_matlab value is too long." << endl << "It will be truncated up to 16 characters." << endl;
//                fom.resize(16);
//            }
//            char *str = new char[ fom.length() + 1 ];
//            strcpy(str,  fom.c_str());
//            config->file_of_matlab = str;
//        }

//        if(line.find("rfclient.RULE_AMOUNT") != string::npos){
//            config->RULE_AMOUNT = atoi(line.substr(line.find('=') + 1).c_str());
//        }

//        if(line.find("rfclient.RULE_LENGTH") != string::npos){
//            config->RULE_LENGTH = atoi(line.substr(line.find('=') + 1).c_str());
//        }

//        if(line.find("rfclient.BACK_AMOUNT") != string::npos){
//            config->BACK_AMOUNT = atoi(line.substr(line.find('=') + 1).c_str());
//        }

//        if(line.find("rfclient.BACK_LENGTH") != string::npos){
//            config->BACK_LENGTH = atoi(line.substr(line.find('=') + 1).c_str());
//        }
//    }

//    configFile.close();

//    return 0;
//}

RfThread::RfThread(RfData* rfdata_)
{
//    RCConfig rcconfig;

//    cout << "Initialization config file..." << endl;

//    if(!initConfig(&rcconfig)){

//        cout << rcconfig.file_of_matlab << endl;
//        cout << rcconfig.name << endl;
//        cout << rcconfig.BACK_AMOUNT << endl;
//        cout << rcconfig.BACK_LENGTH << endl;
//        cout << rcconfig.RULE_AMOUNT << endl;
//        cout << rcconfig.RULE_LENGTH << endl;

//        cout << "...successful" << endl;
//    }
//    else
//    {
//        cerr << "...bad" << endl;
//        char *str = new char[16];
//        str = "Robofootball";
//        rcconfig.name = str;
//        str = "main";
//        rcconfig.file_of_matlab=str;
//        rcconfig.RULE_AMOUNT=5;
//        rcconfig.RULE_LENGTH=5;
//        rcconfig.BACK_AMOUNT=10;
//        rcconfig.BACK_LENGTH=8;

//    }

//    RfData data(rcconfig);
    currentRfData=rfdata_;

//    currentRfData = data;

    m_buffer[255]='\0';
    stopped = false;
    fmtlab = false;
    flog = false;
    fclient = false;

}

bool RfThread::is_run()
{
    if (!fmtlab)
    {
        cout << "Matlab is not running" << endl;
        return false;
    }
    if (!fclient)
    {
        cout << "SSl client is not open" << endl;
    }
    return true;
}

void RfThread::go(){
 if(is_run())
     start();
}

//void printRobotInfo(const SSL_DetectionRobot & robot) {
//    printf("CONF=%4.2f ", robot.confidence());
//    if (robot.has_robot_id()) {
//        printf("ID=%3d ",robot.robot_id());
//    } else {
//        printf("ID=N/A ");
//    }
//    printf(" HEIGHT=%6.2f POS=<%9.2f,%9.2f> ",robot.height(),robot.x(),robot.y());
//    if (robot.has_orientation()) {
//        printf("ANGLE=%6.3f ",robot.orientation());
//    } else {
//        printf("ANGLE=N/A    ");
//    }
//    printf("RAW=<%8.2f,%8.2f>\n",robot.pixel_x(),robot.pixel_y());
//}


void RfThread::run()
{
    cout << "run ok!" << endl;

    //------GraphWindow Connect------
        printf("Start to Connect to GraphWindow\n");
        Pipe_Client myClient((char*)"GraphClient", 100);
        myClient.DispLog=false;
        myClient.Connect();
        char p[100]={0};

    while(true){


        mutex.lock();
        if (currentRfData->client.receive(currentRfData->packet)) {
            printf("-----Received Wrapper Packet---------------------------------------------\n");
            //see if the packet contains a robot detection frame:
            if (currentRfData->packet.has_detection()) {
                SSL_DetectionFrame detection = currentRfData->packet.detection();
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

                //cout << "Received SSL Packet" << endl;

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
                    currentRfData->sdat.Ball[0]=idCam;
                    SSL_DetectionBall ball = detection.balls(0);
                    currentRfData->sdat.Ball[1]=ball.x();
                    currentRfData->sdat.Ball[2]=ball.y();
                }
                else
                    if(currentRfData->sdat.Ball[0]==idCam)
                        currentRfData->sdat.Ball[0]=0;

                memcpy((char *) mxGetPr(currentRfData->seng.Ball), (char *) currentRfData->sdat.Ball, BALL_COUNT_d);
                engPutVariable(currentRfData->seng.ep, "Balls", currentRfData->seng.Ball);

                for (int j = 0; j < TEAM_COUNT/4; j++)
                    if(currentRfData->sdat.Blue[j]==idCam)
                        currentRfData->sdat.Blue[j]=0;
                for (int i = 0; i < robots_blue_n; i++) {
                    SSL_DetectionRobot robot = detection.robots_blue(i);
                    currentRfData->sdat.Blue[robot.robot_id()]=idCam;
                    currentRfData->sdat.Blue[robot.robot_id()+12]=robot.x();
                    currentRfData->sdat.Blue[robot.robot_id()+24]=robot.y();
                    currentRfData->sdat.Blue[robot.robot_id()+36]=robot.orientation();
                }

                memcpy((char *) mxGetPr(currentRfData->seng.Blue), (char *) currentRfData->sdat.Blue, TEAM_COUNT_d);
                engPutVariable(currentRfData->seng.ep, "Blues", currentRfData->seng.Blue);

                for (int j = 0; j < TEAM_COUNT/4; j++)
                    if(currentRfData->sdat.Yellow[j]==idCam)
                        currentRfData->sdat.Yellow[j]=0;
                for (int i = 0; i < robots_yellow_n; i++) {
                    SSL_DetectionRobot robot = detection.robots_yellow(i);
                    currentRfData->sdat.Yellow[robot.robot_id()]=idCam;
                    currentRfData->sdat.Yellow[robot.robot_id()+12]=robot.x();
                    currentRfData->sdat.Yellow[robot.robot_id()+24]=robot.y();
                    currentRfData->sdat.Yellow[robot.robot_id()+36]=robot.orientation();
                }

                memcpy((char *) mxGetPr(currentRfData->seng.Yellow), (char *) currentRfData->sdat.Yellow, TEAM_COUNT_d);
                engPutVariable(currentRfData->seng.ep, "Yellows", currentRfData->seng.Yellow);

                if (flog) {
                    for (int i = 0; i < robots_blue_n*4; i++){
                        logfile << currentRfData->sdat.Yellow[i] << " ";
                    }
                    logfile << endl;
                }


            }
            //see if packet contains geometry data:
            {
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

        }

//        cout << "--------------------------Received BT packed--------------------------" << endl;
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

        //cout << "---------------Received Matlab packed. Send to BT Server ------------------" << endl;
        if(m_buffer[5]=='1'){
            //-------Calculation of Rules Parameters--------
            engEvalString(currentRfData->seng.ep, currentRfData->config.file_of_matlab);

            m_buffer[5]='0';

            engEvalString(currentRfData->seng.ep, "disp(1)");
            // get Rules Parameters from Matlab
            currentRfData->seng.Rule = engGetVariable(currentRfData->seng.ep, "Rules");

            if(currentRfData->seng.Rule!=NULL){
                memcpy((char *) currentRfData->sdat.Rule, (char *)mxGetPr(currentRfData->seng.Rule),currentRfData->config.RULE_LENGTH*currentRfData->config.RULE_AMOUNT*sizeof(double));
                for(int i=0; i<currentRfData->config.RULE_LENGTH*currentRfData->config.RULE_AMOUNT; ++i){
                    cout << currentRfData->sdat.Rule[i] << " ";
                }
                cout << "get rule" << endl;
            }
            else
                cerr << "Can't Copy Rules" << endl;


            //----------------Sending by BT------------------
            if(m_buffer[5]=='1'){

                for(int i=0;i<currentRfData->config.RULE_LENGTH;++i){
                    if((int)currentRfData->sdat.Rule[i]){
                        switch((int)currentRfData->sdat.Rule[i]){
                        case 1:
                            cout << "send to bt server pack: " << endl;
                            for(int j=0; j<currentRfData->config.RULE_AMOUNT; ++j){
                                pack[j]=(char)currentRfData->sdat.Rule[i+currentRfData->config.RULE_LENGTH*j];
                                printf("%d ", (char)currentRfData->sdat.Rule[i+currentRfData->config.RULE_LENGTH*j]);
                            }
                            break;
                        case 2:
                            printf("Message to BT\n");
                            for(int j=0; j<currentRfData->config.RULE_AMOUNT; ++j){
                                pack[j]=(char)currentRfData->sdat.Rule[i+currentRfData->config.RULE_LENGTH*j];
                                printf("%d ", (char)currentRfData->sdat.Rule[i+currentRfData->config.RULE_LENGTH*j]);
                            }
                            break;
                        case 3:
                            printf("Message to client\n");
                            break;
                        }

                        sprintf (sendString, "Rules=zeros(%d, %d)", currentRfData->config.RULE_LENGTH, currentRfData->config.RULE_AMOUNT);
                        engEvalString(currentRfData->seng.ep, sendString);
                        engEvalString(currentRfData->seng.ep, "disp(1)");

                        printf("%s\n", pack);
                        if(!(currentRfData->BtClient.Send(pack,100))){
                            printf("Can't Send to BT-Modul\n");
                            currentRfData->BtClient.Connect();
                        }
                    }
                }
            }

        }

        if(timerSend<(clock()-CLOCKS_PER_SEC/FRAPS))
         {
             timerSendControl=clock();

             timerSend=clock();

 //            cout << "--------------Received Matlab packed. Send to BT Server ------------------" << endl;
             if(m_buffer[5]=='1'){
                 //-------Calculation of Rules Parameters--------
                 engEvalString(currentRfData->seng.ep, currentRfData->config.file_of_matlab);

                 m_buffer[5]='0';

                 engEvalString(currentRfData->seng.ep, "disp(1)");
                 // get Rules Parameters from Matlab
                 currentRfData->seng.Rule = engGetVariable(currentRfData->seng.ep, "Rules");

                 if(currentRfData->seng.Rule!=NULL)
                 {
                     memcpy((char *) currentRfData->sdat.Rule, (char *)mxGetPr(currentRfData->seng.Rule), currentRfData->config.RULE_LENGTH*currentRfData->config.RULE_AMOUNT*sizeof(double));
                     for(int i=0; i<currentRfData->config.RULE_LENGTH*currentRfData->config.RULE_AMOUNT; ++i)
                     {
                             cout << currentRfData->sdat.Rule[i] << " ";
                     }
                     cout << "get rule" << endl;
                 }
                 else
                     cerr << "Can't Copy Rules" << endl;


                 //cout<<"send BT"<<endl;                //------Sending by BT--------
                 {

                     for(int i=0;i<currentRfData->config.RULE_LENGTH;++i)
                     {
                         if((int)currentRfData->sdat.Rule[i]){
                             switch((int)currentRfData->sdat.Rule[i])
                             {
                             case 1:
                                 cout << "send to bt server pack: " << endl;
                                 for(int j=0; j<currentRfData->config.RULE_AMOUNT; ++j)
                                 {
                                     pack[j]=(char)currentRfData->sdat.Rule[i+currentRfData->config.RULE_LENGTH*j];
                                     printf("%d ", (char)currentRfData->sdat.Rule[i+currentRfData->config.RULE_LENGTH*j]);
                                 }
                                 break;
                             case 2:
                                 printf("Message to BT\n");
                                 for(int j=0; j<currentRfData->config.RULE_AMOUNT; ++j)
                                 {
                                     pack[j]=(char)currentRfData->sdat.Rule[i+currentRfData->config.RULE_LENGTH*j];
                                     printf("%d ", (char)currentRfData->sdat.Rule[i+currentRfData->config.RULE_LENGTH*j]);
                                 }
                                 break;
                             case 3:
                                 printf("Message to client\n");
                                 break;
                             }

                             sprintf (sendString, "Rules=zeros(%d, %d)", currentRfData->config.RULE_LENGTH, currentRfData->config.RULE_AMOUNT);
                             engEvalString(currentRfData->seng.ep, sendString);
                             engEvalString(currentRfData->seng.ep, "disp(1)");

                             printf("%s\n", pack);
                             if(!(currentRfData->BtClient.Send(pack,100)))
                             {
                                 printf("Can't Send to BT-Modul\n");
                                 currentRfData->BtClient.Connect();
                             }
                         }
                     }
                 }

             }
//             {//GRAPH WINDOW
//   //              if (sdat.Ball[0])
//                 //cout<<sdat.Ball[0]<<" "<<sdat.Ball[1]<<" "<<sdat.Ball[2]<<endl;
//                 sprintf(p, "%d %c %f %f %f ", 1, 'r', currentRfData->sdat.Ball[0], currentRfData->sdat.Ball[1], currentRfData->sdat.Ball[2]);
//                 if(!(myClient.Send(p,100)))
//                 {
//                     printf("Can't Send to Graph Window\n");
//                     myClient.Connect();
//                 }
//                 else{
//                     for(int i=0;i<12;++i){
//     //                    if (sdat.Blue[i])
//                         {
//                             sprintf(p, "%d %c %f %f %f %f", i, 'b', currentRfData->sdat.Blue[i], currentRfData->sdat.Blue[i+12], currentRfData->sdat.Blue[i+24], currentRfData->sdat.Blue[i+36]);
//                             myClient.Send(p,100);
//                         }//printf("ow.. ");
//                     }
//                     for(int i=0;i<12;++i){
//       //                  if (sdat.Yellow[i])
//                         {
//                             sprintf(p, "%d %c %f %f %f %f", i, 'y', currentRfData->sdat.Yellow[i], currentRfData->sdat.Yellow[i+12], currentRfData->sdat.Yellow[i+24], currentRfData->sdat.Yellow[i+36]);
//                             myClient.Send(p,100);
//                         }//printf("ow.. ");
//                     }
//                 }
//             }

             // Sleep(1);
             timecontrolS++;
             timeS+=clock()-timerSendControl;

             if (timecontrolS>FRAPS)
             {
                 timecontrolS=0;

                 for (int i=0; i<10; i++)
                         cout<<"!!!!!!!   "<<timeS<<endl;
                 timeS=0;

             }
         }
        mutex.unlock();
        Sleep(1);


    } // end while
}

//RfThread::setTargetWidget(QWidget *widget)
//{
//    targetWidget = widget;
//}

int RfThread::run_matlab()
{
    cout << "Work is started" << endl;
    mutex.lock();
    if (!(currentRfData->seng.ep = engOpen(NULL))) {
            cerr << "Can't to Open Matlab Engine" << endl;
            fmtlab = false;
            return -2;
        }

    // matlab buffer
    engOutputBuffer(currentRfData->seng.ep, m_buffer, 255);

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
    engEvalString(currentRfData->seng.ep, StringWithPath);
    printf("We are on %s\n",StringWithPath);

    //-----create Rules-----

    sprintf (sendString, "Rules=zeros(%d, %d)", currentRfData->config.RULE_LENGTH, currentRfData->config.RULE_AMOUNT);
    engEvalString(currentRfData->seng.ep, sendString);
    engEvalString(currentRfData->seng.ep, "disp(1)");

    mutex.unlock();

    fmtlab = true;

    return 0;
}

void RfThread::stop_matlab()
{
    mutex.lock();
    engEvalString(currentRfData->seng.ep, "exit");
    fmtlab = false;
    mutex.unlock();
}

void RfThread::client_open()
{
    mutex.lock();
    currentRfData->client.open(true);
    cout << "SSl client is open" << endl; // !!! посылать сообщением виджету
    fclient = true;
    mutex.unlock();
}


// create and open log
bool RfThread::openlog()
{
    logfile.open(QDir::currentPath().append("\\").append(QDate::currentDate().toString("yyyy_MM_dd")).append(".log").toAscii(), ios::app);
    if (!logfile.is_open())
    {
        flog = false;
        return false;
    }
    logfile << QTime::currentTime().toString().toAscii().data() << "Check" << "\n";
    flog = true;
    return true;
}

// close log
bool RfThread::closelog()
{
    logfile.close();
    flog = false;
    return true;
}
