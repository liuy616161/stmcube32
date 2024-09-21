#include "stm32f4xx_hal.h"

typedef struct
{
	int16_t JoyStick[4];  //Ò¡¸Ë
	int8_t sw[4];    //²¦¸Ë
	int16_t knob[2];		//ÐýÅ¥
	uint8_t lof;		//Ö¡¶ªÊ§
	uint8_t fp;   	//¹ÊÕÏ±£»¤
}_rc;
