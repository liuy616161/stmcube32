#include "motor.h"
#include  "bsp_can.h"

Motor motor_yaw_info;
Motor m6020[2];
uint8_t rxData[8];

void Motor_init(){
		pid_init(&m6020[0].cascade_pid.inner, 20, 1, 20, 0, 10000);
		pid_init(&m6020[0].cascade_pid.outer, 10, 1, 20, 0, 1000);
		pid_init(&m6020[1].cascade_pid.inner, 20, 1, 0, 0, 10000);
		pid_init(&m6020[1].cascade_pid.outer, 5, 1, 5, 0, 1000);
}

void Motor_SpedSend(Motor *motor) //单级速度环
{
	  motor[0].target_speed = RC.joyStick[0]*320.0/783; //yaw轴，将摇杆的值转换为速度值
    motor[1].target_speed = RC.joyStick[2]*320.0/783; //pitch轴
    pid_calc(&(motor[0].speed_pid), motor[0].target_speed, motor[0].rotor_speed);  
    pid_calc(&(motor[1].speed_pid), motor[1].target_speed, motor[1].rotor_speed);
    Can1_transmit(motor[0].speed_pid.output, motor[1].speed_pid.output);
}

void Motor_CascadeSend(Motor *motor) //串级角度环
{
	 //yaw轴
    if(-10 < RC.joyStick[0] && RC.joyStick[0] < 10)
        motor[0].target_angle = 190; //回到中心位置
    else motor[0].target_angle = 190 + RC.joyStick[0] * 60 / 783;
		if(-10 < RC.joyStick[2] && RC.joyStick[2] < 10)
        motor[1].target_angle = 150; //回到中心位置
    else if(RC.joyStick >0) motor[1].target_angle = 150 + RC.joyStick[2] * 60 / 783;
		else  motor[1].target_angle = 150 + RC.joyStick[2] * 45 / 783;
		
		pid_cascadesend(&(motor[0].cascade_pid),motor[0].target_angle,motor[0].real_angle,motor[0].rotor_speed);
		pid_cascadesend(&(motor[1].cascade_pid),motor[1].target_angle,motor[1].real_angle,motor[1].rotor_speed);
		Can1_transmit(motor[0].cascade_pid.output,motor[1].cascade_pid.output);
}
void Gimbal_Control()
{
    if (RC.sw[1] == 0) //判断拨杆状态
    {
          Can1_transmit(0, 0);
    }
    if (RC.sw[1] == 1)
    { 
        Motor_CascadeSend(m6020);
    }
}


