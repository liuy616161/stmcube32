//
// Created by ������2021 on 2023/7/27.
//
#include "stdarg.h" // ����ɱ������
#include <stdio.h>
#include "main.h"
//#include "SysTick.h"
#include "gpio.h"
#include "oled.h"
#include "oled_font.h"
#include "i2c.h"
//#include "test.h"

#define OLED_TEST

char buffer[128];

//CubeMX�Զ��������
void I2C_Configuration(void)
{
    //GPIO
    HAL_I2C_MspInit(&hi2c1);
    // ���� I2C1
    MX_I2C1_Init();
}

//��OLED�Ĵ�����ַдһ��byte������
void I2C_WriteByte(uint8_t addr, uint8_t data)
{
    uint8_t buffer[2] = {addr, data};

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, buffer, 2, 1000);

    if (status != HAL_OK)
    {
        // �������ʧ�ܣ��������ڴ˴���Ӵ��������
    }
}

//дָ��
void WriteCmd(unsigned char I2C_Command)
{
    I2C_WriteByte(0x00,I2C_Command);
}

//д����
void WriteData(unsigned char I2C_Data)
{
    I2C_WriteByte(0x40,I2C_Data);
}

/**
 * @function: void OLED_WR_DATA(uint8_t data)
 * @description: ���豸д��������
 * @param {uint8_t} data ����
 * @return {*}
 */
void OLED_WR_DATA(uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x40,I2C_MEMADD_SIZE_8BIT,&data,1,0x100);
}

void OLED_Init(void)
{
    HAL_Delay(100);
    WriteCmd(0xAE); //display off
    WriteCmd(0x20);	//Set Memory Addressing Mode
    WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
    WriteCmd(0xc8);	//Set COM Output Scan Direction
    WriteCmd(0x00); //---set low column address
    WriteCmd(0x10); //---set high column address
    WriteCmd(0x40); //--set start line address
    WriteCmd(0x81); //--set contrast control register
    WriteCmd(0xff); //����?���̡�?�� 0x00~0xff
    WriteCmd(0xa1); //--set segment re-map 0 to 127
    WriteCmd(0xa6); //--set normal display
    WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
    WriteCmd(0x3F); //
    WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    WriteCmd(0xd3); //-set display offset
    WriteCmd(0x00); //-not offset
    WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
    WriteCmd(0xf0); //--set divide ratio
    WriteCmd(0xd9); //--set pre-charge period
    WriteCmd(0x22); //
    WriteCmd(0xda); //--set com pins hardware configuration
    WriteCmd(0x12);
    WriteCmd(0xdb); //--set vcomh
    WriteCmd(0x20); //0x20,0.77xVcc
    WriteCmd(0x8d); //--set DC-DC enable
    WriteCmd(0x14); //
    WriteCmd(0xaf); //--turn on oled panel

}

//���ù����ʼ���꣨x,y��
void OLED_SetPos(unsigned char x,unsigned char y)
{
    WriteCmd(0xb0+y);
    WriteCmd( (x & 0xf0) >> 4 | 0x10 );
    WriteCmd( (x & 0x0f) | 0x01 );
}

//���������Ļ
void OLED_Fill(unsigned char Fill_Data)
{
    unsigned char m,n;

    for(m=0;m<8;m++)
    {
        WriteCmd(0xb0+m);
        WriteCmd(0x00);
        WriteCmd(0x10);

        for(n=0;n<128;n++)
        {
            WriteData(Fill_Data);
        }
    }
}

//����
void OLED_Clear(void)
{
    OLED_Fill(0x00);
}

//��OLED�������л���
void OLED_ON(void)
{
    WriteCmd(0xAF);
    WriteCmd(0x8D);
    WriteCmd(0x14);
}

//��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
void OLED_OFF(void)
{
    WriteCmd(0xAE);
    WriteCmd(0x8D);
    WriteCmd(0x10);
}
void OLED_refresh_line(uint8_t start_page, uint8_t end_page) {

    // �����е�ַ��Χ
    WriteCmd(0x21); // �е�ַ��������
    WriteCmd(0);    // ��ʼ�е�ַ
    WriteCmd(127);  // �����е�ַ

    // ����ҳ���ַ��Χ
    WriteCmd(0x22);    // ҳ���ַ��������
    WriteCmd(start_page); // ��ʼҳ���ַ
    WriteCmd(end_page);   // ����ҳ���ַ

    // ����������ˢ��ָ����
    for (uint16_t i = 0; i < (128 * (end_page - start_page + 1)); ++i) {
        WriteData(buffer[i]);
    }
}

void OLED_ClearLine(uint8_t line) {
    uint8_t x;

    // ����к��Ƿ��ڷ�Χ�� (0~3)
    if (line > 8) return;

    // ���� OLED ���λ�õ�Ŀ���е���ʼλ��
    OLED_SetPos(0, line );

    // ʹ�ÿո��ַ�����Ŀ����
    for (x = 0; x < 128 / 6; x++) {
        OLED_ShowStr(x * 6, line ," ", 1 );
    }
}

/**
 * @function: unsigned int oled_pow(uint8_t m,uint8_t n)
 * @description: m^n����
 * @param {uint8_t} m,n
 * @return {unsigned int} result
 */
unsigned int oled_pow(uint8_t m,uint8_t n)
{
	unsigned int result=1;
	while(n--)result*=m;
	return result;
}

/**
 * @function: void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size,uint8_t Color_Turn)
 * @description: ��OLED12864�ض�λ�ÿ�ʼ��ʾһ���ַ�
 * @param {uint8_t} x�ַ���ʼ��ʾ�ĺ�����
 * @param {uint8_t} y�ַ���ʼ��ʾ��������
 * @param {uint8_t} chr����ʾ���ַ�
 * @param {uint8_t} Char_Size����ʾ�ַ��������С,ѡ������ 16/12
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */

void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{
	unsigned char c=0,i=0;
		c=chr-' ';//�õ�ƫ�ƺ��ֵ
		if(x>128-1){x=0;y=y+2;}
		if(Char_Size ==16)
		{
			OLED_SetPos(x,y);
			for(i=0;i<8;i++)
				{
					  OLED_WR_DATA(F8X16[c*16+i]);
				}
			OLED_SetPos(x,y+1);
			for(i=0;i<8;i++)
			    {
					  OLED_WR_DATA(F8X16[c*16+i+8]);
			    }

			}
	     else
	     {
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
			    {
					  OLED_WR_DATA(F6x8[c][i]);
			    }
		  }
}

/*!
 * @brief ����ַ���
 * @param x �����꣬����127
 * @param y �����꣬����7
 * @param ch �ַ������� ��ʾΪ"xxx"
 * @param TextSize �����С ��ѡΪ1/2
 */
// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); ch[] -- Ҫ��ʾ���ַ���; TextSize -- �ַ���С(1:6*8 ; 2:8*16)
// Description    : ��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��

void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteData(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteData(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteData(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}

void OLED_ShowCN(unsigned char x,unsigned char y,unsigned char N)
{
    unsigned char i = 0;
    unsigned char addr = 32*N;

    OLED_SetPos(x,y);

    for(i=0;i<16;i++)
    {
        WriteData(F16X16[addr]);
        addr += 1;
    }

    OLED_SetPos(x,y+1);

    for(i=0;i<16;i++)
    {
        WriteData(F16X16[addr]);
        addr += 1;
    }
}

void OLED_ShowBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
    unsigned char x,y;
    unsigned int j = 0;

    if(y1 % 8 == 0)
    {
        y = y1 / 8;
    }
    else
    {
        y = y1 / 8+1;
    }

    for(y=y0;y<y1;y++)
    {
        OLED_SetPos(x0,y);

        for(x=x0;x<x1;x++)
        {
            WriteData(BMP1[j++]);
        }
    }
}
/**
 * @function: void OLED_Showdecimal(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size2, uint8_t Color_Turn)
 * @description: ��ʾ����������
 * @param {uint8_t} x����ʾ��������ʼ������,x:0~126
 * @param {uint8_t} y����ʾ��������ʼ������, y:0~7����ѡ�������СΪ16������������֮����Ҫ���2����ѡ�������СΪ12�����1
 * @param {float} num:����ĸ���������
 * @param {uint8_t } z_ len:�������ֵ�λ��
 * @param {uint8_t } f_len: С�����ֵ�λ��
 * @param {uint8_t} size2:��������ݴ�С��ѡ�� 16/12��16Ϊ8X16��12Ϊ6x8
 * @param {uint8_t} Color_Turn�Ƿ�����ʾ(1���ࡢ0������)
 * @return {*}
 */
void OLED_Showdecimal(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size2)
{
	uint8_t t,temp,i=0;//iΪ������־λ
	uint8_t enshow;
	int z_temp,f_temp;
	if(num<0)
	{
		z_len+=1;
		i=1;
		num=-num;
	}
	z_temp=(int)num;
	//��������
	for(t=0;t<z_len;t++)
	{
		temp=(z_temp/oled_pow(10,z_len-t-1))%10;
		if(enshow==0 && t<(z_len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}
			else
			enshow=1;
		}
		OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2);
	}
	//С����
	OLED_ShowChar(x+(size2/2)*(z_len),y,'.',size2);

	f_temp=(int)((num-z_temp)*(oled_pow(10,f_len)));
  //С������
	for(t=0;t<f_len;t++)
	{
		temp=(f_temp/oled_pow(10,f_len-t-1))%10;
		OLED_ShowChar(x+(size2/2)*(t+z_len)+5,y,temp+'0',size2);
	}
	if(i==1)//���Ϊ�����ͽ���ǰ��һλ��ֵ��-��
	{
		OLED_ShowChar(x,y,'-',size2);
		i=0;
	}
}
