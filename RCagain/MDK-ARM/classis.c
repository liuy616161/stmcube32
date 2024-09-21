#include "chassis.h"
#include "rc.h"
#include "pid.h"
#include "bsp_can.h"


chassis chassis;

float ABS(float tmp)
{
	if(tmp<0) return -tmp;
	return tmp;
}

void chassis_init()
{
	pid_init(&chassis.motor[0].speed_pid, 10, 1, 0, 1000, 16384);
	pid_init(&chassis.motor[1].speed_pid, 10, 1, 0, 1000, 16384);
	pid_init(&chassis.motor[2].speed_pid, 10, 1, 0, 1000, 16384);
	pid_init(&chassis.motor[3].speed_pid, 10, 1, 0, 1000, 16384);
}