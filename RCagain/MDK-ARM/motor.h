#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "stm32f4xx_hal.h"
#include "rc.h"
#include "pid.h"


typedef struct {
	uint16_t canID;
	int16_t  set_v;
	
	uint16_t rotor_angle;
	uint32_t real_angle;	
	int32_t target_angle;
	
	int16_t target_speed;
	int16_t rotor_speed;
	
	int16_t  torque_current;
	uint8_t temp;
	pid_struct speed_pid;
	cascadepid cascade_pid;
}Motor;
 
extern Motor motor_yaw_info;
extern Motor m6020[2];

extern uint8_t data[25];
extern _rc RC;
extern uint16_t channel[18];


//void Motor_SpedSend(Motor *motor) ;
//void Motor_CascadeSend(Motor *motor) ;
#endif