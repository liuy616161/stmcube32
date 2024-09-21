#include "bsp_can.h"
#include  "chassis.h"

void Can1_Init()
{
    CAN_FilterTypeDef Can1_Filter = {0};
    Can1_Filter.FilterActivation = ENABLE;
    Can1_Filter.FilterMode = CAN_FILTERMODE_IDMASK;
    Can1_Filter.FilterScale = CAN_FILTERSCALE_32BIT;
    Can1_Filter.FilterIdHigh = 0x0000;
    Can1_Filter.FilterIdLow = 0x0000;
    Can1_Filter.FilterMaskIdHigh = 0x0000;
    Can1_Filter.FilterMaskIdLow = 0x0000;
    Can1_Filter.FilterBank = 0;
    Can1_Filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    HAL_CAN_ConfigFilter(&hcan1, &Can1_Filter);
    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}
void Can2_Init()
{
    CAN_FilterTypeDef Can2_Filter = {0};
    Can2_Filter.FilterActivation = ENABLE;
    Can2_Filter.FilterMode = CAN_FILTERMODE_IDMASK;
    Can2_Filter.FilterScale = CAN_FILTERSCALE_32BIT;
    Can2_Filter.FilterIdHigh = 0x0000;
    Can2_Filter.FilterIdLow = 0x0000;
    Can2_Filter.FilterMaskIdHigh = 0x0000;
    Can2_Filter.FilterMaskIdLow = 0x0000;
    Can2_Filter.FilterBank = 0;
    Can2_Filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    HAL_CAN_ConfigFilter(&hcan2, &Can2_Filter);
    HAL_CAN_Start(&hcan2);
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
}
void can_filter_init(){
	Can2_Init(); Can1_Init();
}

void Motor_GetData(Motor* motor, uint8_t rxData[8])
{
    motor->rotor_angle = (uint16_t)(rxData[0] << 8 | rxData[1]);//机械角度0~8191
   	motor->real_angle = motor->rotor_angle * 360.0 / 8191.0; //将机械角度转化成实时角度0~360
    motor->rotor_speed = (int16_t)(rxData[2]  << 8 | rxData[3]);
    motor->torque_current = (int16_t)(rxData[4]  << 8 | rxData[5]);
    motor->temp = rxData[6];
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef rxHeader;
	uint8_t 						rxData[8];
	HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&rxHeader,rxData);
	if(hcan == &hcan1)
        switch(rxHeader.StdId)
        {   //6020反馈报文标识符
            case 0x205: 
                Motor_GetData(&m6020[0], rxData);break;
            case 0x206:
                Motor_GetData(&m6020[1], rxData);break;
            default: 
                break;
        }
    if(hcan == &hcan2)
        switch(rxHeader.StdId)
        {   //3508反馈报文标识符
            case 0x201:
                Motor_GetData(&chassis.motor[0], rxData);break;
            case 0x202:
                Motor_GetData(&chassis.motor[1], rxData);break;
            case 0x203:
                Motor_GetData(&chassis.motor[2], rxData);break;
            case 0x204:
                Motor_GetData(&chassis.motor[3], rxData);break;
            default: 
                break;
        }
}

void Can1_transmit(int16_t value1, int16_t value2)
{
	CAN_TxHeaderTypeDef tx_header;
	uint8_t 						txData[8] = {0};
	
		txData[0] = (value1 >> 8) & 0xff;
    txData[1] = (value1) & 0xff;
    txData[2] = (value2 >> 8) & 0xff;
    txData[3] = (value2) & 0xff;
	
	tx_header.StdId=0x1FF;//控制6020电机1.2.3.4
	tx_header.RTR=CAN_RTR_DATA;
	tx_header.IDE=CAN_ID_STD;
	tx_header.DLC=8;
	
	HAL_CAN_AddTxMessage(&hcan1,&tx_header,txData,(uint32_t*)CAN_TX_MAILBOX0);
}


void Can2_transmit(int16_t value1 , int16_t value2 , int16_t value3 , int16_t value4)
{
	CAN_TxHeaderTypeDef tx_header;
	uint8_t 						txData[8]={0};
	
	tx_header.StdId=0x200;//控制3508电机1.2.3.4
	tx_header.RTR=CAN_RTR_DATA;
	tx_header.IDE=CAN_ID_STD;
	tx_header.DLC=8;
	
    txData[0] = (value1 >> 8) & 0xff;
    txData[1] = (value1) & 0xff;
    txData[2] = (value2 >> 8) & 0xff;
    txData[3] = (value2) & 0xff;
    txData[4] = (value3 >> 8) & 0xff;
    txData[5] = (value3) & 0xff;
    txData[6] = (value4 >> 8) & 0xff;
    txData[7] = (value4) & 0xff;
	
	
	HAL_CAN_AddTxMessage(&hcan2,&tx_header,txData,(uint32_t*)CAN_TX_MAILBOX0);
}