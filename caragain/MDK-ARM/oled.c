//
// Created by 杨振阳2021 on 2023/7/27.
//
#include "stdarg.h" // 引入可变参数库
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

//CubeMX自动配置完成
void I2C_Configuration(void)
{
    //GPIO
    HAL_I2C_MspInit(&hi2c1);
    // 配置 I2C1
    MX_I2C1_Init();
}

//向OLED寄存器地址写一个byte的数据
void I2C_WriteByte(uint8_t addr, uint8_t data)
{
    uint8_t buffer[2] = {addr, data};

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, buffer, 2, 1000);

    if (status != HAL_OK)
    {
        // 如果传输失败，您可以在此处添加错误处理代码
    }
}

//写指令
void WriteCmd(unsigned char I2C_Command)
{
    I2C_WriteByte(0x00,I2C_Command);
}

//写数据
void WriteData(unsigned char I2C_Data)
{
    I2C_WriteByte(0x40,I2C_Data);
}

/**
 * @function: void OLED_WR_DATA(uint8_t data)
 * @description: 向设备写控制数据
 * @param {uint8_t} data 数据
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
    WriteCmd(0xff); //áá?èμ÷?ú 0x00~0xff
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

//设置光标起始坐标（x,y）
void OLED_SetPos(unsigned char x,unsigned char y)
{
    WriteCmd(0xb0+y);
    WriteCmd( (x & 0xf0) >> 4 | 0x10 );
    WriteCmd( (x & 0x0f) | 0x01 );
}

//填充整个屏幕
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

//清屏
void OLED_Clear(void)
{
    OLED_Fill(0x00);
}

//将OLED从休眠中唤醒
void OLED_ON(void)
{
    WriteCmd(0xAF);
    WriteCmd(0x8D);
    WriteCmd(0x14);
}

//让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
void OLED_OFF(void)
{
    WriteCmd(0xAE);
    WriteCmd(0x8D);
    WriteCmd(0x10);
}
void OLED_refresh_line(uint8_t start_page, uint8_t end_page) {

    // 设置列地址范围
    WriteCmd(0x21); // 列地址设置命令
    WriteCmd(0);    // 起始列地址
    WriteCmd(127);  // 结束列地址

    // 设置页面地址范围
    WriteCmd(0x22);    // 页面地址设置命令
    WriteCmd(start_page); // 起始页面地址
    WriteCmd(end_page);   // 结束页面地址

    // 发送数据以刷新指定行
    for (uint16_t i = 0; i < (128 * (end_page - start_page + 1)); ++i) {
        WriteData(buffer[i]);
    }
}

void OLED_ClearLine(uint8_t line) {
    uint8_t x;

    // 检查行号是否在范围内 (0~3)
    if (line > 8) return;

    // 设置 OLED 光标位置到目标行的起始位置
    OLED_SetPos(0, line );

    // 使用空格字符覆盖目标行
    for (x = 0; x < 128 / 6; x++) {
        OLED_ShowStr(x * 6, line ," ", 1 );
    }
}

/**
 * @function: unsigned int oled_pow(uint8_t m,uint8_t n)
 * @description: m^n函数
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
 * @description: 在OLED12864特定位置开始显示一个字符
 * @param {uint8_t} x字符开始显示的横坐标
 * @param {uint8_t} y字符开始显示的纵坐标
 * @param {uint8_t} chr待显示的字符
 * @param {uint8_t} Char_Size待显示字符的字体大小,选择字体 16/12
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */

void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{
	unsigned char c=0,i=0;
		c=chr-' ';//得到偏移后的值
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
 * @brief 输出字符串
 * @param x 横坐标，上限127
 * @param y 纵坐标，上限7
 * @param ch 字符串内容 表示为"xxx"
 * @param TextSize 字体大小 可选为1/2
 */
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择

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
 * @description: 显示正负浮点数
 * @param {uint8_t} x待显示的数字起始横坐标,x:0~126
 * @param {uint8_t} y待显示的数字起始纵坐标, y:0~7，若选择字体大小为16，则两行数字之间需要间隔2，若选择字体大小为12，间隔1
 * @param {float} num:输入的浮点型数据
 * @param {uint8_t } z_ len:整数部分的位数
 * @param {uint8_t } f_len: 小数部分的位数
 * @param {uint8_t} size2:输入的数据大小，选择 16/12，16为8X16，12为6x8
 * @param {uint8_t} Color_Turn是否反相显示(1反相、0不反相)
 * @return {*}
 */
void OLED_Showdecimal(uint8_t x,uint8_t y,float num,uint8_t z_len,uint8_t f_len,uint8_t size2)
{
	uint8_t t,temp,i=0;//i为负数标志位
	uint8_t enshow;
	int z_temp,f_temp;
	if(num<0)
	{
		z_len+=1;
		i=1;
		num=-num;
	}
	z_temp=(int)num;
	//整数部分
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
	//小数点
	OLED_ShowChar(x+(size2/2)*(z_len),y,'.',size2);

	f_temp=(int)((num-z_temp)*(oled_pow(10,f_len)));
  //小数部分
	for(t=0;t<f_len;t++)
	{
		temp=(f_temp/oled_pow(10,f_len-t-1))%10;
		OLED_ShowChar(x+(size2/2)*(t+z_len)+5,y,temp+'0',size2);
	}
	if(i==1)//如果为负，就将最前的一位赋值‘-’
	{
		OLED_ShowChar(x,y,'-',size2);
		i=0;
	}
}
