#ifndef MOTOR_H
#define MOTOR_H

#define RR 30u    //������ٱ�
#define RELOADVALUE __HAL_TIM_GetAutoreload(&htim2)    //��ȡ�Զ�װ��ֵ,������Ϊ20000
#define COUNTERNUM __HAL_TIM_GetCounter(&htim2)        //��ȡ��������ʱ���еļ���ֵ
#include <main.h>
#include "tim.h"
//����ṹ��
typedef struct _Motor
{
	int32_t lastAngle;        //��10msת���ĽǶ�
	int32_t totalAngle;       //�ܵĽǶ�
	int16_t loopNum;          //�����������ֵ
	float speed;              //��������Ŀǰת��,��λΪRPM
}Motor;
Motor motor;

void Motor_Init()
{
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);      //������������ʱ��
    __HAL_TIM_ENABLE_IT(&htim2,TIM_IT_UPDATE);           //������������ʱ�������ж�,���������
	HAL_TIM_Base_Start_IT(&htim6);                       //����10ms��ʱ���ж�
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);            //����PWM
	__HAL_TIM_SET_COUNTER(&htim1, 10000);                //��������ʱ����ʼֵ�趨Ϊ10000
	motor.loopNum = 0;                                   //�������
}

#endif