#ifndef MOTOR_H
#define MOTOR_H

#define RR 30u    //电机减速比
#define RELOADVALUE __HAL_TIM_GetAutoreload(&htim2)    //获取自动装载值,本例中为20000
#define COUNTERNUM __HAL_TIM_GetCounter(&htim2)        //获取编码器定时器中的计数值
#include <main.h>
#include "tim.h"
//电机结构体
typedef struct _Motor
{
	int32_t lastAngle;        //上10ms转过的角度
	int32_t totalAngle;       //总的角度
	int16_t loopNum;          //溢出次数计数值
	float speed;              //电机输出轴目前转速,单位为RPM
}Motor;
Motor motor;

void Motor_Init()
{
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);      //开启编码器定时器
    __HAL_TIM_ENABLE_IT(&htim2,TIM_IT_UPDATE);           //开启编码器定时器更新中断,防溢出处理
	HAL_TIM_Base_Start_IT(&htim6);                       //开启10ms定时器中断
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);            //开启PWM
	__HAL_TIM_SET_COUNTER(&htim1, 10000);                //编码器定时器初始值设定为10000
	motor.loopNum = 0;                                   //溢出计数
}

#endif