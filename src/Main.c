#include "Main.h"


/* グローバル変数の宣言 */
int integral=0;
robotState_t robotState = WAIT;


/* main関数 */
int main (void) {
	sensor_t sensor;
	int count=0;
	//char buff[64]; 
	
	
	initRobot();
	 
	byteTx(CmdStart);
	  
	setBaud(Baud28800, Ubrr28800);
	  
	defineSongs();
	
	byteTx(CmdFull);
	flushRx();

	while(1){
		// センサ情報更新
		updateAllSensors(sensor);
		
		// プレイボタン押下時
		if(isPressed(PLAY_BUTTON, sensor)==1
			&& !sensor->chargingSourcesAvailable){
			robotState = 
				(robotState == WAIT)? 
				RUNNING: 
				WAIT;
		}
		
		// アドバンスボタン押下時
		if(isPressed(ADVANCE_BUTTON, sensor)==1
			&& !sensor->chargingSourcesAvailable){
			robotState = 
				(robotState == WAIT)? 
				SEARCH_HOME: 
				WAIT;
		}
		
		// モジュールボタン押下時
		if(isPressed(MODULE_BUTTON, sensor)==1){
			//sprintf(buff, "%u\r\\n", ((unsigned int)(sensor->wallSignal&0x0f00>>4)*16)+(unsigned int)(sensor->wallSignal&0x00ff));
			//sprintf(buff, "%u\r\\n", sensor->wallSignal);
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
						integral = 0;
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
						integral = 0;
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
						integral = 0;
					}
					else{
						drivePid(culPid(120, sensor->wallSignal));
						delay10ms(1);
					}
				break;
				case SEARCH_WALL:
					if(sensor->wallSignal>0){
						//driveDirect(0xFF90, 0x0070);
						//delay10ms(50);
						driveDirect(0x0000, 0x0000);
						delay10ms(50);
						integral = 0;
						robotState = RUNNING;
					}
					else{
						driveDirect(0x0070, 0x0070);
						delay10ms(5);
						driveDirect(0x0070, 0xFF90);
						delay10ms(5);
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
				case CHARGING:
				break;
				default:
				break;
			}
		}
		else{
			robotState = WAIT;
			driveDirect(0x0000, 0x0000);
		}
		
		/*
		if(sensor->batteryCharge<=300){
			byteTx(CmdPlay);
			byteTx(DEBUG_SONG);
			delay10ms(32);
		}*/
		
	}//while::end
}//main::end




void initRobot(void){
	// Turn off interrupts
	cli();

	// Configure the I/O pins
	DDRB = 0x10;
	PORTB = 0xCF;
	DDRC = 0x02;
	PORTC = 0xFF;
	DDRD = 0xE6;
	PORTD = 0x7D;

	// Set up the serial port for 57600 baud
	UBRR0 = Ubrr57600;
	UCSR0B = (_BV(TXEN0) | _BV(RXEN0));
	UCSR0C = (_BV(UCSZ00) | _BV(UCSZ01));
	  
	setSerial(getSerialDestination());
	powerOnRobot();
}



/* ロボットの電源投入関数 */
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


/* 10msの待ち関数 */
void delay10ms(unsigned int delay_10ms){
	while(delay_10ms-- > 0)
		_delay_loop_2(46080);
}


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


/* 蛇行走行関数 */
void driveTurn(int16_t velocity, int16_t radius){
  byteTx(CmdDrive);
  byteTx((uint8_t)((velocity >> 8) & 0x00FF));
  byteTx((uint8_t)(velocity & 0x00FF));
  byteTx((uint8_t)((radius >> 8) & 0x00FF));
  byteTx((uint8_t)(radius & 0x00FF));
}


/* 直進走行関数 */
void driveDirect(int16_t rightVelocity, int16_t leftVelocity){
  byteTx(CmdDriveWheels);
  byteTx((uint8_t)((rightVelocity >> 8) & 0x00FF));
  byteTx((uint8_t)(rightVelocity & 0x00FF));
  byteTx((uint8_t)((leftVelocity >> 8) & 0x00FF));
  byteTx((uint8_t)(leftVelocity & 0x00FF));
}


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


void drivePid(int pid){
	uint16_t rightPower=0, leftPower=0;
	rightPower = 0x00F0-(pid/2);
	leftPower  = 0x00F0+pid;
	driveDirect(rightPower, leftPower);
}
