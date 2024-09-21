//
// Created by —Ó’Ò—Ù2021 on 2023/7/27.
//
#ifndef OLED_H_
#define OLED_H_

#include "stm32f4xx_hal.h"
#define OLED_ADDRESS 0x78

void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteData(unsigned char I2C_Data);
void OLED_WR_DATA(uint8_t data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x,unsigned char y);
void OLED_Fill(unsigned char Fill_Data);
void OLED_Clear(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_refresh_line(uint8_t start_page, uint8_t end_page);
void OLED_ClearLine(uint8_t line);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x,unsigned char y,unsigned char N);
void OLED_ShowBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void OLED_Showdecimal(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size2);
#endif 
