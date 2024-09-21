#ifndef __BSP_CAN_H_
#define __BSP_CAN_H_

#include "main.h"
#include "can.h"
#include "stm32f4xx.h"

typedef struct{
	uint16_t canID;
	int16_t  set_v;
	uint16_t rotor_angle;
	int16_t rotor_speed;
	int16_t  torque_current;
	uint8_t temp;
}moto_info_t;


void can_filter_init(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void Set_angle(CAN_HandleTypeDef *hcan,int16_t target_angle);
#endif