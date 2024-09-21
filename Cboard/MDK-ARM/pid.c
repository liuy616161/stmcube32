#include "pid.h"
 
pid_struct_t gimbal_yaw_speed_pid;
pid_struct_t gimbal_yaw_angle_pid;
 
void pid_init(pid_struct_t *pid,
              float kp,
              float ki,
              float kd,
              float i_max,
              float out_max)//PID��ʼ������
{
  pid->kp      = kp;
  pid->ki      = ki;
  pid->kd      = kd;
  pid->i_max   = i_max;
  pid->out_max = out_max;
}
 
 
float pid_calc(pid_struct_t *pid, float ref, float fdb)//PID���㺯��
{
  pid->ref = ref;
  pid->fdb = fdb;
  pid->err[1] = pid->err[0];
  pid->err[0] = pid->ref - pid->fdb;
  
  pid->p_out  = pid->kp * pid->err[0];
  pid->i_out += pid->ki * pid->err[0];
  pid->d_out  = pid->kd * (pid->err[0] - pid->err[1]);
  LIMIT_MIN_MAX(pid->i_out, -pid->i_max, pid->i_max);
  
  pid->output = pid->p_out + pid->i_out + pid->d_out;
  LIMIT_MIN_MAX(pid->output, -pid->out_max, pid->out_max);
  return pid->output;
}
 
void gimbal_PID_init()//�ǶȻ����ٶȻ���PID��ʼ��,ֻ�ǳ�����������ݣ����廹��Ҫ����
{
	pid_init(&gimbal_yaw_speed_pid, 30, 0, 0, 30000, 30000);//P=30,I=0,D=0
	pid_init(&gimbal_yaw_angle_pid, 400, 0, 0, 0, 320);//P=400,I=0,D=0
}