#ifndef __BSP_CAN_H_
#define __BSP_CAN_H_

#include "motor.h"
#include "rc.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

void Can2_Init();
void Can1_Init();
void can_filter_init();
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void Can1_transmit(int16_t value1, int16_t value2);
void Can2_transmit(int16_t value1 , int16_t value2 , int16_t value3 , int16_t value4);

#endif
