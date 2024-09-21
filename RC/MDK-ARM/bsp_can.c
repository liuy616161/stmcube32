#include "bsp_can.h"
#include "rc.h"

moto_info_t motor_yaw_info;
extern uint8_t data[25];
extern _rc RC;
extern uint16_t channel[18];
void Motor_CascadeSend(moto_info_t *motor) //串级角度环
{
    motor_yaw_info.target_angle = (RC.joyStick[1]+781)*360.0/781/2; //将摇杆的值转换为角度值
		pid_calc(&motor_yaw_info.angle_pid,motor_yaw_info.target_angle, motor_yaw_info.real_angle );//角度环
		pid_calc(&motor_yaw_info.speed_pid,motor_yaw_info.angle_pid.output, motor_yaw_info.rotor_speed);//速度环
    float output =  motor_yaw_info.speed_pid.output;
    Can_transmit(&hcan1,output);
}

void can_filter_init(void){
	CAN_FilterTypeDef can_filter_st;
	can_filter_st.FilterActivation=ENABLE;
	can_filter_st.FilterMode=CAN_FILTERMODE_IDMASK;
	can_filter_st.FilterScale=CAN_FILTERSCALE_32BIT;
	can_filter_st.FilterIdHigh=0x0000;
	can_filter_st.FilterIdLow=0x0000;
	can_filter_st.FilterMaskIdHigh=0x0000;
	can_filter_st.FilterMaskIdLow=0x0000;
	can_filter_st.FilterBank=0;
	can_filter_st.FilterFIFOAssignment=CAN_RX_FIFO0;
	HAL_CAN_ConfigFilter(&hcan1,&can_filter_st);
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);
	
	can_filter_st.SlaveStartFilterBank=14;
	can_filter_st.FilterBank=14;
	HAL_CAN_ConfigFilter(&hcan1,&can_filter_st);
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef rx_header;
	uint8_t 						rx_data[8];
	if(hcan->Instance==CAN1){
		HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&rx_header,rx_data);
		switch(rx_header.StdId){
			case 0x208:	
			{
				motor_yaw_info.rotor_angle=((rx_data[0]<<8)|rx_data[1]);
				motor_yaw_info.rotor_speed=((rx_data[2]<<8)|rx_data[3]);
				motor_yaw_info.torque_current=((rx_data[4]<<8)|rx_data[5]);
				motor_yaw_info.real_angle=(motor_yaw_info.rotor_angle)*360/8191;
				motor_yaw_info.temp=rx_data[6];
				break;
			}
		}
	}
}

void Can_transmit(CAN_HandleTypeDef *hcan,int16_t value)
{
	CAN_TxHeaderTypeDef tx_header;
	uint8_t 						tx_data[8]={0};
	
	tx_header.StdId=0x1FF;
	tx_header.RTR=CAN_RTR_DATA;
	tx_header.IDE=CAN_ID_STD;
	tx_header.DLC=8;
	
	tx_data[6]=(value>>8)&0xFF;
	tx_data[7]=(value)&0xFF;
	
	
	HAL_CAN_AddTxMessage(&hcan1,&tx_header,tx_data,(uint32_t*)CAN_TX_MAILBOX0);
}
