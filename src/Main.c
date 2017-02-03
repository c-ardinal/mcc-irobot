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
	sensor = (sensor_t)malloc(sizeof(sensor_t));
	int count=0;
	
	initRobot();

	byteTx(CmdStart);

	setBaud(Baud28800, Ubrr28800);

	defineSongs();

	byteTx(CmdFull);
	flushRx();
	
	LEDBothOff;
	
	while(1){
		// センサ情報更新
		updateAllSensors(sensor);

		// プレイボタンが押された場合、壁沿い走行を開始
		if(isPressed(PLAY_BUTTON, sensor)==1
			&& !sensor->chargingSourcesAvailable){
			robotState =
				(robotState == WAIT)?
				RUNNING:
				WAIT;
		}

		// アドバンスボタンが押された場合、ホーム探索を開始
		if(isPressed(ADVANCE_BUTTON, sensor)==1
			&& !sensor->chargingSourcesAvailable){
			robotState =
				(robotState == WAIT)?
				SEARCH_HOME:
				WAIT;
		}

		// モジュールボタンが押された場合、"Hello, Create!!"を送信する
		if(isPressed(MODULE_BUTTON, sensor)==1){
			sendString("Hello, Create!!\r\\n");
		}

		// ロボットが地面に置かれている間、下記の処理を行う
		if(sensor->cliffLeft==0
			&& sensor->cliffFrontLeft==0
			&& sensor->cliffFrontRight==0
			&& sensor->cliffRight==0){
			switch(robotState){
				//待機時処理
				case WAIT:
					driveDirect(0x0000, 0x0000);
				break;
				//壁沿い走行
				case RUNNING:
					if(getIntegral()>=300){
						driveDirect(0x00A0, 0x00A0);
						if(sensor->bumpAndWheelDrops!=0){
							resetIntegral();
						}
					}
					else{
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
							LED1On;
							LED2Off;
							driveDirect(0xFF00, 0xFF00);
							delay10ms(10);
							driveDirect(0x0000, 0x0000);
							delay10ms(20);
							driveDirect(0x0070, 0xFF90);
							delay10ms(60);
							driveDirect(0x0000, 0x0000);
							delay10ms(20);
							resetIntegral();
						}
						else if(sensor->bumpAndWheelDrops==2){
							LED1Off;
							LED2On;
							driveDirect(0xFF00, 0xFF00);
							delay10ms(10);
							driveDirect(0x0000, 0x0000);
							delay10ms(20);
							driveDirect(0xFF90, 0x0070);
							delay10ms(60);
							driveDirect(0x0000, 0x0000);
							delay10ms(20);
							resetIntegral();
						}
						else if(sensor->bumpAndWheelDrops==3){
							LEDBothOn;
							driveDirect(0xFF00, 0xFF00);
							delay10ms(10);
							driveDirect(0x0000, 0x0000);
							delay10ms(20);
							driveDirect(0x0070, 0xFF90);
							delay10ms(190);
							driveDirect(0x0000, 0x0000);
							delay10ms(20);
							resetIntegral();
						}
						else{
							LEDBothOff;
							if(sensor->wallSignal>TARGET_LOW)
								drivePid(culPid(TARGET_HIGH, sensor->wallSignal));
							else
								drivePid(culPid(TARGET_LOW, sensor->wallSignal));
							delay10ms(1);
						}
					}
				break;
				//ホームベース探索
				case SEARCH_HOME:
					driveDirect(0x0070, 0x0070);
					if(sensor->irByte==252){
						driveTurn(0x00F0, 0x0001);
						delay10ms(50);
						robotState = GO_HOME;
					}
				break;
				//ホームベースへ向かう
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
				//ホームにドッキングしたか確認する
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
							byteTx(CmdPlay);
							byteTx(DEBUG_SONG);
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
			resetIntegral();
		}
	}//while::end
}//main::end
