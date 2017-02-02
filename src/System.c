/*
 * ------------------------------------------------------ *
 * @file	: System.c
 * @brief	: ロボットの部分的な制御を行う
 * ------------------------------------------------------ *
 */
#include "System.h"


/*
 * ------------------------------------------------------ *
 * @function: ロボットを初期化する
 * @param	: void
 * @return	: void
 * ------------------------------------------------------ *
 */
void initRobot(void){
	cli();
	DDRB = 0x10;
	PORTB = 0xCF;
	DDRC = 0x02;
	PORTC = 0xFF;
	DDRD = 0xE6;
	PORTD = 0x7D;
	UBRR0 = Ubrr57600;
	UCSR0B = (_BV(TXEN0) | _BV(RXEN0));
	UCSR0C = (_BV(UCSZ00) | _BV(UCSZ01));
	setSerial(getSerialDestination());
	powerOnRobot();
}



/*
 * ------------------------------------------------------ *
 * @function: ロボットの電源を投入する
 * @param	: void
 * @return	: void
 * ------------------------------------------------------ *
 */
void powerOnRobot(void)
{
	if(!RobotIsOn){
		while(!RobotIsOn){
			RobotPwrToggleLow;
			delay10ms(50);
			RobotPwrToggleHigh;
			delay10ms(10);
			RobotPwrToggleLow;
		}
		delay10ms(350);
	}
}


/*
 * ------------------------------------------------------ *
 * @function: 10msのウェイトを行う
 * @param	: 待機時間
 * @return	: void
 * ------------------------------------------------------ *
 */
void delay10ms(unsigned int delay_10ms){
	while(delay_10ms-- > 0)
		_delay_loop_2(46080);
}


/*
 * ------------------------------------------------------ *
 * @function: サウンドを設定する
 * @param	: void
 * @return	: void
 * ------------------------------------------------------ *
 */
void defineSongs(void){
	// Create song
	byteTx(CmdSong);
	byteTx(CREATE_SONG);
	byteTx(5);
	byteTx(65);
	byteTx(24);
	byteTx(69);
	byteTx(24);
	byteTx(67);
	byteTx(48);
	byteTx(76);
	byteTx(24);
	byteTx(77);
	byteTx(48);

	// Module song
	byteTx(CmdSong);
	byteTx(MODULE_SONG);
	byteTx(6);
	byteTx(71);
	byteTx(24);
	byteTx(71);
	byteTx(24);
	byteTx(71);
	byteTx(24);
	byteTx(72);
	byteTx(24);
	byteTx(77);
	byteTx(48);
	byteTx(76);
	byteTx(48);

	// User-added hardware song
	byteTx(CmdSong);
	byteTx(USER_HW_SONG);
	byteTx(5);
	byteTx(62);
	byteTx(48);
	byteTx(65);
	byteTx(24);
	byteTx(67);
	byteTx(24);
	byteTx(62);
	byteTx(24);
	byteTx(69);
	byteTx(48);

	byteTx(CmdSong);
	byteTx(DEBUG_SONG);
	byteTx(1);
	byteTx(62);
	byteTx(16);

	byteTx(CmdSong);
	byteTx(CHARGE_START_SONG);
	byteTx(1);
	byteTx(86);
	byteTx(16);
}
