/*
 * ------------------------------------------------------ *
 * @file	: Communicate.c
 * @brief	: 通信関連の処理を行う
 * ------------------------------------------------------ *
 */
#include "Communicate.h"


/*
 * ------------------------------------------------------ *
 * @function: 通信レートを設定する
 * @param	: ボーレート、転送レート
 * @return	: void
 * ------------------------------------------------------ *
 */
void setBaud(uint8_t baud, unsigned int ubrr){
	byteTx(CmdBaud);
	byteTx(baud);
	while(!(UCSR0A & _BV(TXC0)));
	UBRR0 = ubrr;
	delay10ms(10);
}


/*
 * ------------------------------------------------------ *
 * @function: 8bitデータを受信する
 * @param	: void
 * @return	: 8bitのデータ
 * ------------------------------------------------------ *
 */
uint8_t byteRx(void){
	while(!(UCSR0A & _BV(RXC0))) ;
	return UDR0;
}


/*
 * ------------------------------------------------------ *
 * @function: 受信したデータを破棄する
 * @param	: void
 * @return	: void
 * ------------------------------------------------------ *
 */
void flushRx(void){
	uint8_t temp;
	while(UCSR0A & _BV(RXC0))
		temp = UDR0;
}


/*
 * ------------------------------------------------------ *
 * @function: 8bitのデータを送信する
 * @param	: 送信するデータ
 * @return	: void
 * ------------------------------------------------------ *
 */
void byteTx(uint8_t value){
	while(!(UCSR0A & _BV(UDRE0)));
		UDR0 = value;
}


/*
 * ------------------------------------------------------ *
 * @function: シリアル通信先を設定する
 * @param	: 通信先
 * @return	: void
 * ------------------------------------------------------ *
 */
void setSerial(uint8_t com){
	if(com == USB)
 		PORTB |= BV(4);
	else if(com == CRB)
		PORTB &= ~BV(4);
}



/*
 * ------------------------------------------------------ *
 * @function: シリアル通信先を取得する
 * @param	: void
 * @return	: シリアル通信先
 * ------------------------------------------------------ *
 */
uint8_t	getSerialDestination(void){
	if(PORTB & 0x10)
		return USB;
	else
		return CRB;
}


/*
 * ------------------------------------------------------ *
 * @function: 文字列を送信する
 * @param	: 送信文字列(終端に\r\\nを付ける必要あり)
 * @return	: void
 * ------------------------------------------------------ *
 */
void sendString(char *str){
	setBaud(Baud9600, Ubrr9600);
	setSerial(USB);
	delay10ms(20);
	while(*str)
		byteTx((uint8_t)*str++);
	setSerial(CRB);
	delay10ms(20);
	setBaud(Baud28800, Ubrr28800);
}
