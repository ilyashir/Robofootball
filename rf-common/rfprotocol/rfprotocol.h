#ifndef RFPROTOCOL_H_
#define RFPROTOCOL_H_

#include <iostream>
#include <string.h>

#define MESSAGE_SIZE 100
#define MAX_MESSAGE 1000

//char* PIPE_NAME= "BtModul"; // BAD!
#define PIPE_NAME "BtModul" // BAD!

#define MAX_DEVICE 12 // max bluetooth devices
#define NXT_MESS_SIZE 32

// Должно быть так!||||||||||||||
//                 VVVVVVVVVVVVVV

//using namespace std;

////commands constants
//#define COMMAND_PING						0xFF
//#define COMMAND_READ_SENSOR_REQUEST			0x10	//should be used for fetching info from analog sensors
//#define COMMAND_READ_SENSOR_RESPONSE		0x11
//#define COMMAND_WRITE_SENSOR_REQUEST		0x20	//should be used for I2C sensors config
//#define COMMAND_WRITE_SENSOR_RESPONSE		0x21
//#define COMMAND_READ_MOTOR_REQUEST			0x30
//#define COMMAND_READ_MOTOR_RESPONSE			0x31
//#define COMMAND_WRITE_MOTOR_REQUEST			0x40
//#define COMMAND_WRITE_MOTOR_RESPONSE		0x41
//#define COMMAND_SYSTEM_REQUEST				0x50
//#define COMMAND_SYSTEM_RESPONSE				0x51
//#define COMMAND_DEVICE_ANNOUNCE				0x60	//should be used for announcing connected NXT devices to the server
//#define COMMAND_DEVICE_ANNOUNCE_RESPONSE	0x61
//#define COMMAND_INIT_SENSOR_REQUEST			0x70	//should be used for initializing sensors on the NXT device
//#define COMMAND_INIT_SENSOR_RESPONSE		0x71

////protocols constants
//#define PROTOCOL_SAME	0
//#define PROTOCOL_TCP	1
//#define PROTOCOL_UDP	2

////data formats
//#define FORMAT_RAW		0x10

//#pragma pack(1)
//typedef struct {
//	char deviceName[16];
//	uint8 port;
//	uint8 sensor;
//	uint8 dataFormat;
//	uint8 dataSize;
//	uint8 data[28];
//} Device;

//#pragma pack(1)
//typedef struct {
//	uint8	command;
//	uint16	repeatsNumber;
//	uint16	repeatDelay;		//ms
//	uint8	responseProtocol;
//	char 	dst[16];
//} Command;

//#pragma pack(1)
//typedef struct {
//	uint8	sourceIp[4];
//	char	sourceName[16];
//	uint8	sourceProtocol;
//	int64	timestampSec;
//	int64	timestampUsec;
//} Source;

//#pragma pack(1)
//typedef struct {
//	Command 		command;
//	Source 			source;
//	Device			deviceData;
//	struct timeval	receivedTimestamp;
//} ExchangePacket;

#endif /* RFPROTOCOL_H_ */
