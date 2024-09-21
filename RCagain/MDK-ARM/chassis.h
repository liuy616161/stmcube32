#ifndef __CHASSIS_H_
#define __CHASSIS_H_

#include "motor.h"
#include "rc.h"
#include "pid.h"
#include "bsp_can.h"

#define PI 3.1416f
#define ROTATE_RATIO 300.f //旋转比例
#define WHEEL_RADIUS 76.f //轮子半径

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
    float target; //目标值
    float step; //步进值
    float value; //当前值
    float deadzone; //死区增减
}Slope;

extern Chassis chassis;

#endif