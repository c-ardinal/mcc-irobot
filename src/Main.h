#ifndef _MAIN_H_
#define _MAIN_H_


#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "oi.h"
#include "Constant.h"
#include "Sensor.h"
#include "Communicate.h"



/* ロボット状態定義 */
typedef enum State{
	WAIT,
	RUNNING,
	SEARCH_WALL,
	SEARCH_HOME,
	GO_HOME,
	CHECK_HOME,
	CHARGING
} robotState_t;


/* 関数プロトタイプ宣言 */
void initRobot(void);
void powerOnRobot(void);
void delay10ms(unsigned int);
void defineSongs(void);
void driveTurn(int16_t, int16_t);
void driveDirect(int16_t, int16_t);
uint16_t culPid(uint8_t, uint8_t);
void drivePid(int);


#endif
