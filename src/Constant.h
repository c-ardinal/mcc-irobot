#ifndef _CONSTANT_H_
#define _CONSTANT_H_


/* サウンド名の定義 */
#define CREATE_SONG  0
#define MODULE_SONG  1
#define USER_HW_SONG 2
#define DEBUG_SONG   3
#define CHARGE_START_SONG 4


/* シリアル通信先の設定 */
#define USB 1
#define CRB 2


/* ビット操作マクロの定義 */
#define	_BV(bit) (1	<<	(bit))	
#define	 BV(bit) _BV(bit)


/* 壁沿い走行用PIDパラメータ */
#define KP  0.6
#define KI  2.4
#define KD  0.15
#define TIME 0.10


#endif