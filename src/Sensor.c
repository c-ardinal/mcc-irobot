/*
 * ------------------------------------------------------ *
 * @file	: Sensor.c
 * @brief	: センサ関連の処理を行う
 * ------------------------------------------------------ *
 */
#include "Sensor.h"


/*
 * ------------------------------------------------------ *
 * @function: センサの値を取得する
 * @param	: センサ番号
 * @return	: センサ値
 * ------------------------------------------------------ *
 */
uint8_t getSensor(uint8_t sensorId){
	uint8_t result;
	byteTx(CmdSensors);
	byteTx(sensorId);
	result = byteRx();
	return result;
}


/*
 * ------------------------------------------------------ *
 * @function: センサを全取得する
 * @param	: 結果代入曜ポインタ
 * @return	: void
 * ------------------------------------------------------ *
 */
void updateAllSensors(sensor_t result){

	byteTx(CmdSensors);
	byteTx(6);

	result->bumpAndWheelDrops = byteRx();

	result->wall = byteRx();

	result->cliffLeft = byteRx();

	result->cliffFrontLeft = byteRx();

	result->cliffFrontRight = byteRx();

	result->cliffRight = byteRx();

	result->VirtualWall = byteRx();

	result->overcurrents = byteRx();

	byteRx();

	byteRx();

	result->irByte = byteRx();

	result->buttons = byteRx();

	result->distance = (((unsigned int)byteRx())<<4&0xff00) + ((unsigned int)byteRx()&0x00FF);

	result->angle  = (((unsigned int)byteRx())<<4&0xff00) + ((unsigned int)byteRx()&0x00FF);

	result->chargingState = byteRx();

	result->voltage  = (((unsigned int)byteRx())<<4&0xff00) + ((unsigned int)byteRx()&0x00FF);

	result->current  = (((unsigned int)byteRx())<<4&0xff00) + ((unsigned int)byteRx()&0x00FF);

	result->batteryTemperature = byteRx();

	result->batteryCharge  = (((unsigned int)byteRx())<<4&0xff00) + ((unsigned int)byteRx()&0x00FF);

	result->batteryCapacity  = (((unsigned int)byteRx())<<4&0xff00) + ((unsigned int)byteRx()&0x00FF);

	result->wallSignal  = (((unsigned int)byteRx())<<4&0xff00) + ((unsigned int)byteRx()&0x00FF);

	result->cliffLeftSignal  = (((unsigned int)byteRx())<<4&0xff00) + ((unsigned int)byteRx()&0x00FF);

	result->cliffFrontLeftSignal  = (((unsigned int)byteRx())<<4&0xff00) + ((unsigned int)byteRx()&0x00FF);

	result->cliffFrontRightSignal  = (((unsigned int)byteRx())<<4&0xff00) + ((unsigned int)byteRx()&0x00FF);

	result->cliffRightSignal  = (((unsigned int)byteRx())<<4&0xff00) + ((unsigned int)byteRx()&0x00FF);

	result->userDigitalInputs = byteRx();

	result->userAnalogInput  = (((unsigned int)byteRx())<<4&0xff00) + ((unsigned int)byteRx()&0x00FF);

	result->chargingSourcesAvailable = byteRx();

	result->oiMode = byteRx();

	result->songNumber = byteRx();

	result->songPlaying = byteRx();

	result->numberOfStreamPacket = byteRx();

	result->velocity  = (((unsigned int)byteRx())<<4&0xff00) + ((unsigned int)byteRx()&0x00FF);

	result->radius  = (((unsigned int)byteRx())<<4&0xff00) + ((unsigned int)byteRx()&0x00FF);

	result->rightVelocity  = (((unsigned int)byteRx())<<4&0xff00) + ((unsigned int)byteRx()&0x00FF);

	result->leftVelocity  = (((unsigned int)byteRx())<<4&0xff00) + ((unsigned int)byteRx()&0x00FF);
}


/*
 * ------------------------------------------------------ *
 * @function: ボタンの押下チェックを行う
 * @param	: ボタン番号、センサ値代入曜構造体のポインタ
 * @return	: 0:開放→開放、1:開放→押下、2:押下→押下、3:押下→開放
 * ------------------------------------------------------ *
 */
int isPressed(buttonId_t buttonId, sensor_t sensor){
	static uint8_t buttonState[3][2] = {{0, 0}, {0, 0}, {0, 0}};

	buttonState[buttonId][0] = buttonState[buttonId][1];

	switch(buttonId){
		case PLAY_BUTTON:
			buttonState[0][1] = sensor->buttons&0x01;
		break;
		case ADVANCE_BUTTON:
			buttonState[1][1] = (sensor->buttons&0x04)>>2;
		break;
		case MODULE_BUTTON:
			buttonState[2][1] = (!(PIND & 0x10));
		break;
		case RESET_BUTTON:
		break;
	}

	if(buttonState[buttonId][0]==0 && buttonState[buttonId][1]==0)
		return 0;
	else if(buttonState[buttonId][0]==0 && buttonState[buttonId][1]==1)
		return 1;
	else if(buttonState[buttonId][0]==1 && buttonState[buttonId][1]==1)
		return 2;
	else if(buttonState[buttonId][0]==1 && buttonState[buttonId][1]==0)
		return 3;
	else
		return -1;
}
