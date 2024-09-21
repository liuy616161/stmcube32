#include "motor.h"
#include  "oled.h"
#include  "main.h"


void Motor_Init()
{
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);      //������������ʱ��
    __HAL_TIM_ENABLE_IT(&htim4,TIM_IT_UPDATE);           //������������ʱ�������ж�,���������
	HAL_TIM_Base_Start_IT(&htim6);                       //����10ms��ʱ���ж�
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);          
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);              
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);               //����PWM
	__HAL_TIM_SET_COUNTER(&htim4, 0);                            //�������
}