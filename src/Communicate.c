#include "Communicate.h"


/* ボーレート設定関数 */
void setBaud(uint8_t baud, unsigned int ubrr){
	byteTx(CmdBaud);
	byteTx(baud);
	while(!(UCSR0A & _BV(TXC0)));
	UBRR0 = ubrr;
	delay10ms(10);
}


/* バイトデータ受信関数 */
uint8_t byteRx(void){
	while(!(UCSR0A & _BV(RXC0))) ;
	return UDR0;
}


/*  */
void flushRx(void){
	uint8_t temp;
	while(UCSR0A & _BV(RXC0))
		temp = UDR0;
}


/* バイトデータ転送関数 */
void byteTx(uint8_t value){
	while(!(UCSR0A & _BV(UDRE0)));
		UDR0 = value;
}


/* シリアル通信先の設定 */
void setSerial(uint8_t com){	
	if(com == USB)	
 		PORTB |= BV(4);
	else if(com == CRB)	
		PORTB &= ~BV(4);	
}



/* 現在のシリアル通信先の取得 */
uint8_t	getSerialDestination(void){
	if(PORTB & 0x10)	
		return USB;
	else
		return CRB;
}


/* 文字列の送信(文末に\r\\n必須) */
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
