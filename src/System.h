/*
 * ------------------------------------------------------ *
 * @file	: System.h
 * @brief	: System.c用のヘッダファイル
 * ------------------------------------------------------ *
 */
#ifndef _SYSTEM_H_
#define _SYSTEM_H_


/*
 * ------------------------------------------------------ *
 * @brief	: インクルードファイル
 * ------------------------------------------------------ *
 */
#include <util/delay.h>
#include "oi.h"
#include "Constant.h"
#include "Sensor.h"
#include "Communicate.h"



/*
 * ------------------------------------------------------ *
 * @brief	: ロボットの状態
 * ------------------------------------------------------ *
 */
typedef enum State{
	WAIT,
	RUNNING,
	SEARCH_HOME,
	GO_HOME,
	CHECK_HOME
} robotState_t;


/*
 * ------------------------------------------------------ *
 * @brief	: プロトタイプ宣言
 * ------------------------------------------------------ *
 */
void initRobot(void);
void powerOnRobot(void);
void delay10ms(unsigned int);
void defineSongs(void);


#endif
