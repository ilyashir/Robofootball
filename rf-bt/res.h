#pragma once
//��������� - ���������
#define MESSAGE_SIZE 100
#define MAX_MESSAGE 1000
#define MAX_DEVICE 12
#define MAX_SAVE_LET 10
char * PIPE_NAME="BtModul";
#define NXT_MESS_SIZE 32


#include <stdio.h>
#include <iostream>
using namespace std;

#include "BTDeviceClass.h"
#include "NXTClass.h"
Device R[MAX_DEVICE+1]; //������ ��� NXT ������������
#include "PipeChannel.h"
#include "fileWork.h"
BTconnect My; //����� ��� BlueTooth ����������
//-------------------------------------


//-------------------------------