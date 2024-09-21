#ifndef __RC_H_
#define __RC_H_

#include  "main.h"
#include  "stm32f4xx.h"
#include "usart.h"
#include "gpio.h"

typedef struct
{
	int16_t joyStick[4];//ҡ��
	int8_t sw[4];       //����
	int16_t knob[2];		//��ť
	uint8_t lof;				//֡��ʧ
	uint8_t fp;					//���ϱ���
}_rc;


extern uint8_t data[25];
extern uint16_t channel[18];
extern _rc RC;

void Sbus_DataDecode(_rc *RC,uint8_t data[25],uint16_t channel[18]);
void Sbus_DataCheck(_rc *RC);
void Sbus_DataAnalyze(_rc *RC,uint16_t channel[18]);

#endif
