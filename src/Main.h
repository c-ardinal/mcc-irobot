/*
 * ------------------------------------------------------ *
 * @file	: Communicate.c
 * @brief	: 通信関連の処理を行う
 * ------------------------------------------------------ *
 */
#ifndef _MAIN_H_
#define _MAIN_H_


/*
 * ------------------------------------------------------ *
 * @brief	: インクルードファイル
 * ------------------------------------------------------ *
 */
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "oi.h"
#include "Constant.h"
#include "Sensor.h"
#include "Communicate.h"
#include "Drive.h"


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
