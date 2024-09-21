#ifndef MOTOR_H
#define MOTOR_H




#include <main.h>
#include "stm32f4xx_hal.h" //HAL文件声明
#include <main.h>
#define MOTOR_GO HAL_GPIO_WritePin(GPIOE, AIN1_Pin, GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOE, AIN2_Pin, GPIO_PIN_SET)
extern TIM_HandleTypeDef htim4;//编码器
extern TIM_HandleTypeDef htim6;//定时器
extern TIM_HandleTypeDef htim1;//定时器

void Motor_Init(void);
/*//电机结构体
typedef struct _Motor
{
	int32_t lastAngle;        //上10ms转过的角度
	int32_t totalAngle;       //总的角度
	int16_t loopNum;          //溢出次数计数值
	float speed;              //电机输出轴目前转速,单位为RPM
}Motor;
 Motor motor;

void Motor_Init();
void GET_NUM();
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#define RR 30u    //电机减速比
#define RELOADVALUE __HAL_TIM_GetAutoreload(&htim2)    //获取自动装载值,本例中为20000
#define COUNTERNUM __HAL_TIM_GetCounter(&htim2)        //获取编码器定时器中的计数值*/



#endif