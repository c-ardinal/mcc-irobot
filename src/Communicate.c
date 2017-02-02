#include "Communicate.h"


/* �{�[���[�g�ݒ�֐� */
void setBaud(uint8_t baud, unsigned int ubrr){
	byteTx(CmdBaud);
	byteTx(baud);
	while(!(UCSR0A & _BV(TXC0)));
	UBRR0 = ubrr;
	delay10ms(10);
}


/* �o�C�g�f�[�^��M�֐� */
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


/* �o�C�g�f�[�^�]���֐� */
void byteTx(uint8_t value){
	while(!(UCSR0A & _BV(UDRE0)));
		UDR0 = value;
}


/* �V���A���ʐM��̐ݒ� */
void setSerial(uint8_t com){	
	if(com == USB)	
 		PORTB |= BV(4);
	else if(com == CRB)	
		PORTB &= ~BV(4);	
}



/* ���݂̃V���A���ʐM��̎擾 */
uint8_t	getSerialDestination(void){
	if(PORTB & 0x10)	
		return USB;
	else
		return CRB;
}


/* ������̑��M(������\r\\n�K�{) */
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
