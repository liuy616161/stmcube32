#ifndef __CHASSIS_H_
#define __CHASSIS_H_

#include "motor.h"

typedef struct _chassis
{
	Motor motor[4];
	struct{
		float vx;
		float vy;
		float vw;
	}target;
}Chassis;


extern Chassis chassis;

#endif