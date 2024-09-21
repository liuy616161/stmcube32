#include "pid.h"


void pid_init(pid_struct *pid,float kp,float ki,float kd,float i_max,float total_max)
{
	pid->kp =kp;
	pid->ki =ki;
	pid->kd =kd;
	pid->i_max =i_max;
	pid->total_max=total_max;
}


void pid_calc(pid_struct *pid,float target,float current)
{
	
	pid->e =target -current ;
	
	pid->p_out =(int32_t)(pid->kp*pid->e );
	pid->i_out +=(int32_t)(pid->ki*pid->e );
	pid->d_out =(int32_t)(pid->kd *(pid->e -pid->last_e ));
	
	//»ý·ÖÏÞ·ù
	if(pid->i_out >pid->i_max ) pid->i_out=pid->i_max; 
	
	pid->total_out =pid->p_out +pid->i_out +pid->d_out ;
	
	if(pid->total_out >pid->total_max ) pid->total_out=pid->total_max;
	
	pid->last_e =pid->e ;
}

void pid_cadecalc(Cadepid *pid,float outRef,float  outFdb,float inFdb)
{
	pid_calc (&pid->outer ,outRef,outFdb );
	pid_calc (&pid->inner ,pid->outer.total_out ,inFdb );
	pid->output =pid->inner .total_out ;
}