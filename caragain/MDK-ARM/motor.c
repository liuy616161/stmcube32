#include "motor.h"
#include  "oled.h"
#include  "main.h"


void Motor_Init()
{
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);      //开启编码器定时器
    __HAL_TIM_ENABLE_IT(&htim4,TIM_IT_UPDATE);           //开启编码器定时器更新中断,防溢出处理
	HAL_TIM_Base_Start_IT(&htim6);                       //开启10ms定时器中断
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);          
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);              
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);               //开启PWM
	__HAL_TIM_SET_COUNTER(&htim4, 0);                            //溢出计数
}