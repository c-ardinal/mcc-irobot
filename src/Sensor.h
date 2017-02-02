/*
 * ------------------------------------------------------ *
 * @file	: Sensor.h
 * @brief	: Sensor.c用のヘッダファイル
 * ------------------------------------------------------ *
 */
#ifndef _SENSOR_H_
#define _SENSOR_H_


/*
 * ------------------------------------------------------ *
 * @brief	: インクルードファイル
 * ------------------------------------------------------ *
 */
#include "oi.h"
#include "Constant.h"
#include "Communicate.h"


/*
 * ------------------------------------------------------ *
 * @brief	: ボタン番号
 * ------------------------------------------------------ *
 */
typedef enum{
	PLAY_BUTTON,
	ADVANCE_BUTTON,
	MODULE_BUTTON,
	RESET_BUTTON
} buttonId_t;


/*
 * ------------------------------------------------------ *
 * @brief	: センサ値管理曜構造体
 * ------------------------------------------------------ *
 */typedef struct{
	//Packet:0, 1, 6
	uint8_t bumpAndWheelDrops;			//1, 0~31
	uint8_t wall;						//1, 0~1
	uint8_t cliffLeft;					//1, 0~1
	uint8_t cliffFrontLeft;				//1, 0~1
	uint8_t cliffFrontRight;			//1, 0~1
	uint8_t cliffRight;
	uint8_t VirtualWall;				//1, 0~1
	uint8_t overcurrents;				//1, 0~31
	//Packet:0, 2, 6
	uint8_t irByte;						//1, 0~255
	uint8_t buttons;					//1, 0~15
	int distance;						//2, -32768~32767
	int angle;							//2, -32768~32767
	//Packet:0, 3, 6
	uint8_t chargingState;				//1, 0~5
	unsigned int voltage;				//2, 0~65535
	int current;						//2, -32768~32767
	uint8_t batteryTemperature;			//1, -128~-127
	unsigned int batteryCharge;			//2, 0~65535
	unsigned int batteryCapacity;		//2, 0~65535
	//Packet:4, 6
	unsigned int wallSignal;			//2, 0~4095
	unsigned int cliffLeftSignal;		//2, 0~4095
	unsigned int cliffFrontLeftSignal;	//2, 0~4095
	unsigned int cliffFrontRightSignal;	//2, 0~4095
	unsigned int cliffRightSignal;		//2, 0~4095
	uint8_t userDigitalInputs;			//1, 0~31
	unsigned int userAnalogInput;		//2, 0~1023
	uint8_t chargingSourcesAvailable;	//1, 0~3
	//Packet:5, 6
	uint8_t oiMode;						//1, 0~3
	uint8_t songNumber;					//1, 0~15
	uint8_t songPlaying;				//1, 0~1
	uint8_t numberOfStreamPacket;		//1, 0~42
	int velocity;					//2, -500~500
	int radius;					//2, -32768~32767
	int rightVelocity;				//2, -500~500
	int leftVelocity;				//2, -500~500
} *sensor_t;


/*
 * ------------------------------------------------------ *
 * @brief	: プロトタイプ宣言
 * ------------------------------------------------------ *
 */
uint8_t getSensor(uint8_t);
void updateAllSensors(sensor_t);
int isPressed(buttonId_t, sensor_t);


#endif
