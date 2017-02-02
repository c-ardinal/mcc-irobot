#ifndef _COMMUNICATE_H_
#define _COMMUNICATE_H_


#include <avr/interrupt.h>
#include "oi.h"
#include "Constant.h"


/* 関数プロトタイプ宣言 */
void setBaud(uint8_t, unsigned int);
uint8_t byteRx(void);
void flushRx(void);
void byteTx(uint8_t);
void setSerial(uint8_t);
uint8_t	getSerialDestination(void);
void sendString(char*);


#endif
