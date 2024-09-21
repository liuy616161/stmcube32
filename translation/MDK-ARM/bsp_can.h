#ifndef __BSP_CAN_H_
#define __BSP_CAN_H_
 
#include "main.h"
#include "can.h"
#include "stm32f4xx.h"
 
typedef struct
{
    uint16_t can_id;//���ID
    int16_t  set_voltage;//�趨�ĵ�ѹֵ
    uint16_t rotor_angle;//��е�Ƕ�
    int16_t  rotor_speed;//ת��
    int16_t  torque_current;//Ť�ص���
    uint8_t  temp;//�¶�
}moto_info_t;
 
void can_filter_init(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void Set_Angle(CAN_HandleTypeDef *hcan,int16_t v1);
#endif
