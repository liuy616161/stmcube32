#include "rc.h"

uint8_t data[25];
_rc RC;
uint16_t channel[18];

void Sbus_DataDecode(_rc *RC,uint8_t data[25],uint16_t channel[18])
{
    channel[0]  = (data[1]  >> 0  | data[2]  << 8) & 0x7ff;
    channel[1]  = (data[2]  >> 3  | data[3]  << 5) & 0x7ff;
    channel[2]  = (data[3]  >> 6  | data[4]  << 2  | data[5] << 10) & 0x7ff;
    channel[3]  = (data[5]  >> 1  | data[6]  << 7) & 0x7ff;
    channel[4]  = (data[6]  >> 4  | data[7]  << 4) & 0x7ff;
    channel[5]  = (data[7]  >> 7  | data[8]  << 1  | data[9] << 9) & 0x7ff;
    channel[6]  = (data[9]  >> 2  | data[10] << 6) & 0x7ff;
    channel[7]  = (data[10] >> 5  | data[11] <<  3) & 0x7ff;
    channel[8]  = (data[12] >> 0  | data[13] << 8) & 0x7ff;
    channel[9]  = (data[13] >> 3  | data[14] << 5) & 0x7ff;
    channel[10] = (data[14] >> 6  | data[15] << 2  | data[16] << 10) & 0x7ff;
    channel[11] = (data[16] >> 1  | data[17] << 7) & 0x7ff;
    channel[12] = (data[17] >> 4  | data[18] << 4) & 0x7ff;
    channel[13] = (data[18] >> 7  | data[19] << 1  | data[20] << 9) & 0x7ff;
    channel[14] = (data[20] >> 2  | data[21] << 6) & 0x7ff;
    channel[15] = (data[21] >> 5  | data[22] << 3) & 0x7ff;
    channel[16] = (data[23] >> 7) & 0x01;
    channel[17] = (data[23] >> 6) & 0x01;
    RC->fp  = (data[23] >> 3) & 0x01;
    RC->lof = (data[23] >> 2) & 0x01;
}

void Sbus_DataCheck(_rc *RC)
{
    if(RC->fp || RC->lof)
    {
        RC->joyStick[0] = 0;
        RC->joyStick[1] = 0;
        RC->joyStick[2] = 0;
        RC->joyStick[3] = 0;
        RC->sw[0] = 0;
        RC->sw[1] = 0;
        RC->sw[2] = 0;
        RC->sw[3] = 0;
        RC->knob[0] = 0;
        RC->knob[1] = 0;
    }
}

void Sbus_DataAnalyze(_rc *RC,uint16_t channel[18])
{
    RC->joyStick[0] = channel[0] - 1024;
    RC->joyStick[1] = channel[1] - 1024;
    RC->joyStick[2] = channel[2] - 1024;
    RC->joyStick[3] = channel[3] - 1024;
    RC->sw[0] = channel[4] / 904;
    RC->sw[1] = channel[5] / 904;                                         
    RC->sw[2] = channel[6] / 603;
    RC->sw[3] = channel[7] / 904;
    RC->knob[0] = channel[8] - 240;
    RC->knob[1] = channel[9] - 240;
}
