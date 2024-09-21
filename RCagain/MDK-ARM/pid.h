#ifndef __PID_H_
#define __PID_H_

#include	"main.h"
#include  "stm32f4xx.h"

typedef struct _pid_struct
{
	float kp,ki,kd;
	float p_out,i_out,d_out,output;
	float i_max,out_max;
	float target,feedback,err[2];
}pid_struct;

typedef struct _CascadePID
{
    pid_struct inner;//�ڻ�
    pid_struct outer;//�⻷
    float output;//�������������inner.output
}cascadepid;

void pid_init(pid_struct *pid,float kp,float ki,float kd,float i_max,float out_max);
float pid_calc(pid_struct *pid,float target,float feedback);
float pid_poscalc(pid_struct *pid,float target,float feedback);
void pid_cascadesend(cascadepid *pid ,float targetout,float inget,float outget);
#endif
