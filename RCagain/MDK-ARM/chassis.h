#ifndef __CHASSIS_H_
#define __CHASSIS_H_

#include "motor.h"
#include "rc.h"
#include "pid.h"
#include "bsp_can.h"

#define PI 3.1416f
#define ROTATE_RATIO 300.f //��ת����
#define WHEEL_RADIUS 76.f //���Ӱ뾶

typedef struct _chassis
{
	Motor motor[4];
	struct{
		float vx;
		float vy;
		float vw;
	}target;
}Chassis;


typedef struct
{
    float target; //Ŀ��ֵ
    float step; //����ֵ
    float value; //��ǰֵ
    float deadzone; //��������
}Slope;

extern Chassis chassis;

#endif