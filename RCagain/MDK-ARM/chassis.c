#include "chassis.h"	

Chassis chassis;

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


void Chassis_control(void)
{
    chassis.target.vx = RC.joyStick[3];
    chassis.target.vy = RC.joyStick[1];
    chassis.target.vw = RC.knob[0] * 0.2;

    chassis.motor[0].target_speed = -(-chassis.target.vx + chassis.target.vy + chassis.target.vw / 360.f * 2 * PI * ROTATE_RATIO) / (2 * PI * WHEEL_RADIUS);
    chassis.motor[1].target_speed = (chassis.target.vx + chassis.target.vy - chassis.target.vw / 360.f * 2 * PI * ROTATE_RATIO) / (2 * PI * WHEEL_RADIUS);
    chassis.motor[2].target_speed = (-chassis.target.vx + chassis.target.vy - chassis.target.vw / 360.f * 2 * PI * ROTATE_RATIO) / (2 * PI * WHEEL_RADIUS);
    chassis.motor[3].target_speed = -(chassis.target.vx + chassis.target.vy + chassis.target.vw / 360.f * 2 * PI * ROTATE_RATIO) / (2 * PI * WHEEL_RADIUS);
   
    //19 * 60 * moto rtarget_speed[0]，减速比19，是将m/s转换为rpm
    pid_calc(&chassis.motor[0].speed_pid, 19 * 60 * chassis.motor[0].target_speed, chassis.motor[0].rotor_speed);
    pid_calc(&chassis.motor[1].speed_pid, 19 * 60 * chassis.motor[1].target_speed, chassis.motor[1].rotor_speed);
    pid_calc(&chassis.motor[2].speed_pid, 19 * 60 * chassis.motor[2].target_speed, chassis.motor[2].rotor_speed);
    pid_calc(&chassis.motor[3].speed_pid, 19 * 60 * chassis.motor[3].target_speed, chassis.motor[3].rotor_speed);

    if(RC.sw[0] == 0) //急停制动
        Can2_transmit(0, 0, 0, 0);
    else
        Can2_transmit(chassis.motor[0].speed_pid.output, chassis.motor[1].speed_pid.output, chassis.motor[2].speed_pid.output, chassis.motor[3].speed_pid.output);
}

//计算下一个斜坡值，更新slope->value并返回该值
float Slope_NextVal(Slope *slope)
{
    float error = slope->value - slope->target;
    if(ABS(error) < slope->deadzone)
        return slope->value;
    if(ABS(error) < slope->step)
        slope->value = slope->target;
    else if(error < 0)
        slope->value += slope->step;
    else if(error > 0)
        slope->value -= slope->step;
    return slope->value;
}
