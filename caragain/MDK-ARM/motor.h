#ifndef MOTOR_H
#define MOTOR_H




#include <main.h>
#include "stm32f4xx_hal.h" //HAL�ļ�����
#include <main.h>
#define MOTOR_GO HAL_GPIO_WritePin(GPIOE, AIN1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOE, AIN2_Pin, GPIO_PIN_SET)
extern TIM_HandleTypeDef htim4;//������
extern TIM_HandleTypeDef htim6;//��ʱ��
extern TIM_HandleTypeDef htim1;//��ʱ��

void Motor_Init(void);
/*//����ṹ��
typedef struct _Motor
{
	int32_t lastAngle;        //��10msת���ĽǶ�
	int32_t totalAngle;       //�ܵĽǶ�
	int16_t loopNum;          //�����������ֵ
	float speed;              //��������Ŀǰת��,��λΪRPM
}Motor;
 Motor motor;

void Motor_Init();
void GET_NUM();
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#define RR 30u    //������ٱ�
#define RELOADVALUE __HAL_TIM_GetAutoreload(&htim2)    //��ȡ�Զ�װ��ֵ,������Ϊ20000
#define COUNTERNUM __HAL_TIM_GetCounter(&htim2)        //��ȡ��������ʱ���еļ���ֵ*/



#endif