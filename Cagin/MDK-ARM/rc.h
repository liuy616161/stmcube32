#include "stm32f4xx_hal.h"

typedef struct
{
	int16_t JoyStick[4];  //ҡ��
	int8_t sw[4];    //����
	int16_t knob[2];		//��ť
	uint8_t lof;		//֡��ʧ
	uint8_t fp;   	//���ϱ���
}_rc;
