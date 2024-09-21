#ifndef __BSP_CAN_H_
#define __BSP_CAN_H_

#include "main.h"
#include "can.h"
#include "stm32f4xx.h"
#include "pid.h"

typedef struct{
	uint16_t canID;
	int16_t  set_v;
	uint16_t rotor_angle;
	uint32_t real_angle;	
	uint32_t target_angle;
	int16_t rotor_speed;
	int16_t  torque_current;
	uint8_t temp;
	pid_struct angle_pid,speed_pid;
}moto_info_t;

void Motor_CascadeSend(moto_info_t *motor);
void can_filter_init(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void Can_transmit(CAN_HandleTypeDef *hcan,int16_t value);
#endif
