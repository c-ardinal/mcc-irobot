/*
 * ------------------------------------------------------ *
 * @file	: Drive.c
 * @brief	: ロボットの走行制御を行う
 * ------------------------------------------------------ *
 */
#include "Drive.h"


// 積分値保存用
static int integral = 0;


/*
 * ------------------------------------------------------ *
 * @function: 曲率を用いた走行を行う
 * @param	: 速度、曲率
 * @return	: void
 * ------------------------------------------------------ *
 */
void driveTurn(int16_t velocity, int16_t radius){
  byteTx(CmdDrive);
  byteTx((uint8_t)((velocity >> 8) & 0x00FF));
  byteTx((uint8_t)(velocity & 0x00FF));
  byteTx((uint8_t)((radius >> 8) & 0x00FF));
  byteTx((uint8_t)(radius & 0x00FF));
}


/*
 * ------------------------------------------------------ *
 * @function: 直線走行を行う
 * @param	: 右モータ速度、左モータ速度
 * @return	: void
 * ------------------------------------------------------ *
 */
void driveDirect(int16_t rightVelocity, int16_t leftVelocity){
  byteTx(CmdDriveWheels);
  byteTx((uint8_t)((rightVelocity >> 8) & 0x00FF));
  byteTx((uint8_t)(rightVelocity & 0x00FF));
  byteTx((uint8_t)((leftVelocity >> 8) & 0x00FF));
  byteTx((uint8_t)(leftVelocity & 0x00FF));
}


/*
 * ------------------------------------------------------ *
 * @function: PID制御量を算出する
 * @param	: 目標値、現在値
 * @return	: 制御量
 * ------------------------------------------------------ *
 */
uint16_t culPid(uint8_t target, uint8_t now){
	static int pastDiff = 0;
	int result = 0, nowDiff = 0;

	pastDiff = nowDiff;
	nowDiff = ((int)target-(int)now);
	integral += (nowDiff+pastDiff)/2*TIME;

	int tp = KP*nowDiff;
	int ti = KI*integral;
	int td = KD*(nowDiff-pastDiff)/TIME;

	result = tp+ti+td;
	return result;
}


/*
 * ------------------------------------------------------ *
 * @function: PID制御量を用いた走行を行う
 * @param	: PID制御量
 * @return	: void
 * ------------------------------------------------------ *
 */
void drivePid(int pid){
	uint16_t rightPower=0, leftPower=0;
	rightPower = 0x00F0-(pid/2);
	leftPower  = 0x00F0+pid;
	driveDirect(rightPower, leftPower);
}


/*
 * ------------------------------------------------------ *
 * @function: PID制御による積分値のリセットを行う
 * @param	: void
 * @return	: void
 * ------------------------------------------------------ *
 */
void resetIntegral(void){
	integral = 0;
}


/*
 * ------------------------------------------------------ *
 * @function: PID制御による積分値を渡す
 * @param	: void
 * @return	: 積分値
 * ------------------------------------------------------ *
 */
int getIntegral(void){
	return integral;
}
