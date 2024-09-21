#ifndef __PID_H_
#define __PID_H_
 
#include "main.h"
#include "stm32f4xx.h"

typedef struct 
{
	float e;
	float last_e;
	
	
	float kp;
	float ki;
	float kd;
	
	float i_max;
	float total_max;
	
	float p_out;
	float i_out;
	float d_out;
	float total_out;
}pid_struct;


typedef struct {
	uint16_t encoder;
	uint8_t encoder_is_init;
	uint16_t last_encoder;
	uint16_t round_cnt;
	uint16_t offset;
	uint16_t total_encoder;
}_angle;

typedef struct {
	pid_struct inner;
	pid_struct outer;
	float output;
}Cadepid;


void pid_init(pid_struct *pid,float kp,float ki,float kd,float i_max,float total_max);

void pid_calc(pid_struct *pid,float target,float current);

void pid_cadecalc(Cadepid *pid,float outRef,float  outFdb,float inFdb);
#endif