#include "pid.h"

void pid_init(pid_struct *pid,float kp,float ki,float kd,float i_max,float out_max)
{
	pid->kp=kp;
	pid->ki=ki;
	pid->kd=kd;
	pid->i_max=i_max;
	pid->out_max=out_max;
}

float pid_calc(pid_struct *pid,float target,float feedback){
	pid->target=target;
	pid->feedback=feedback;
	pid->err[1]=pid->err[0];
	pid->err[0]=pid->target-pid->feedback;//Õı¸º£¿
	
	pid->p_out  =pid->kp*pid->err[0];
	pid->i_out +=pid->ki*pid->err[0];
	pid->d_out  =pid->kd*(pid->err[0]-pid->err[1]);
	
	LIMIT_MIN_MAX(pid->i_out,-pid->i_max,pid->i_max);
	
	pid->output=pid->p_out+pid->i_out+pid->d_out;
	LIMIT_MIN_MAX(pid->output,-pid->out_max,pid->output);
	
	return pid->output;
}

