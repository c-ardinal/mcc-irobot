/*
 * ------------------------------------------------------ *
 * @file	: Drive.h
 * @brief	: Drive.c用のヘッダファイル
 * ------------------------------------------------------ *
 */
#ifndef _DRIVE_H_
#define _DRIVE_H_


/*
 * ------------------------------------------------------ *
 * @brief	: インクルードファイル
 * ------------------------------------------------------ *
 */
#include "oi.h"
#include "Constant.h"
#include "Sensor.h"
#include "Communicate.h"


/*
 * ------------------------------------------------------ *
 * @brief	: プロトタイプ宣言
 * ------------------------------------------------------ *
 */
void driveTurn(int16_t, int16_t);
void driveDirect(int16_t, int16_t);
uint16_t culPid(uint8_t, uint8_t);
void drivePid(int);
void resetIntegral(void);
int getIntegral(void);


#endif
