/*
 * ------------------------------------------------------ *
 * @file	: Main.c
 * @brief	: ロボットの全体的な制御を行う
 * ------------------------------------------------------ *
 */
#include "Main.h"


/*
 * ------------------------------------------------------ *
 * @brief	: グローバル変数
 * ------------------------------------------------------ *
 */
robotState_t robotState = WAIT;


/*
 * ------------------------------------------------------ *
 * @function: メイン関数
 * @param	: void
 * @return	: void
 * ------------------------------------------------------ *
 */
int main (void) {
	sensor_t sensor;
	int count=0;
	uint16_t pastWallSignal = 0;
	char buff[64]; 
	
	
	initRobot();

	byteTx(CmdStart);

	setBaud(Baud28800, Ubrr28800);

	defineSongs();

	byteTx(CmdFull);
	flushRx();

	while(1){
		// ?Z???T?????X?V
		updateAllSensors(sensor);

		// ?v???C?{?^????????
		if(isPressed(PLAY_BUTTON, sensor)==1
			&& !sensor->chargingSourcesAvailable){
			robotState =
				(robotState == WAIT)?
				RUNNING:
				WAIT;
		}

		// ?A?h?o???X?{?^????????
		if(isPressed(ADVANCE_BUTTON, sensor)==1
			&& !sensor->chargingSourcesAvailable){
			robotState =
				(robotState == WAIT)?
				SEARCH_HOME:
				WAIT;
		}

		// ???W???[???{?^????????
		if(isPressed(MODULE_BUTTON, sensor)==1){
			sendString("Hello, Create!!\r\\n");
		}


		if(sensor->cliffLeft==0
			&& sensor->cliffFrontLeft==0
			&& sensor->cliffFrontRight==0
			&& sensor->cliffRight==0){
			switch(robotState){
				case WAIT:
					driveDirect(0x0000, 0x0000);
				break;
				case RUNNING:
					if(sensor->irByte==252)
						robotState = SEARCH_HOME;
					else if(sensor->irByte==242){
						driveDirect(0x0070, 0xFF90);
						delay10ms(130);
						driveDirect(0x00A0, 0x00A0);
						delay10ms(600);
						driveDirect(0xFF90, 0x0070);
						delay10ms(240);
						robotState = GO_HOME;
					}
					if(sensor->bumpAndWheelDrops==1){
						driveDirect(0xFF00, 0xFF00);
						delay10ms(10);
						driveDirect(0x0000, 0x0000);
						delay10ms(50);
						driveDirect(0x0070, 0xFF90);
						delay10ms(60);
						driveDirect(0x0000, 0x0000);
						delay10ms(20);
						resetIntegral();
					}
					else if(sensor->bumpAndWheelDrops==2){
						driveDirect(0xFF00, 0xFF00);
						delay10ms(10);
						driveDirect(0x0000, 0x0000);
						delay10ms(50);
						driveDirect(0xFF90, 0x0070);
						delay10ms(60);
						driveDirect(0x0000, 0x0000);
						delay10ms(20);
						resetIntegral();
					}
					else if(sensor->bumpAndWheelDrops==3){
						driveDirect(0xFF00, 0xFF00);
						delay10ms(10);
						driveDirect(0x0000, 0x0000);
						delay10ms(50);
						driveDirect(0x0070, 0xFF90);
						delay10ms(190);
						driveDirect(0x0000, 0x0000);
						delay10ms(20);
						resetIntegral();
					}
					else{
						if(sensor->wallSignal>50)
							drivePid(culPid(150, sensor->wallSignal));
						else
							drivePid(culPid(50, sensor->wallSignal));
						delay10ms(1);
					}
				break;
				case SEARCH_HOME:
					driveDirect(0x0070, 0x0070);
					if(sensor->irByte==252){
						driveTurn(0x00F0, 0x0001);
						delay10ms(50);
						robotState = GO_HOME;
					}
				break;
				case GO_HOME:
					updateAllSensors(sensor);
					switch(sensor->irByte){
						//ForceField
						case 242:
							//driveDirect(0x8070, 0x8070);
						break;
						//GreenBuoy
						case 244:
							driveDirect(0x0020, 0x0070);
						break;
						//RedBuoy
						case 248:
							driveDirect(0x0070, 0x0020);
						break;
						//R and G
						case 252:
							driveDirect(0x0070, 0x0070);
						break;
						//R and F
						case 250:
							driveDirect(0x0070, 0x0020);
						break;
						//G and F
						case 246:
							driveDirect(0x0020, 0x0070);
						break;
						//R and G and F
						case 254:
							driveDirect(0x0030, 0x0030);
						break;
						//Other
						default:
							//robotState = RUNNING;
						break;
					}
					if(sensor->bumpAndWheelDrops>0){
						driveDirect(0xFFCE, 0xFFCE);
						delay10ms(1);
						driveDirect(0x0000, 0x0000);
						robotState = CHECK_HOME;
					}
				break;
				case CHECK_HOME:
					delay10ms(1);
					if(count++>20){
						count=0;
						if(sensor->chargingSourcesAvailable!=0){
							byteTx(CmdPlay);
							byteTx(CHARGE_START_SONG);
							delay10ms(70);
							robotState = WAIT;
						}
						else{
							driveDirect(0xFF00, 0xFF00);
							delay10ms(50);
							robotState = GO_HOME;
						}
					}
				break;
				default:
				break;
			}
		}
		else{
			robotState = WAIT;
			driveDirect(0x0000, 0x0000);
		}
	}//while::end
}//main::end


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
