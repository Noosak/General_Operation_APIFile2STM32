#ifndef __OLED_H
#define __OLED_H			  	 
#include "IIC_STM32.h"
#include "Character_bank.h"

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED IIC端口定义----------------  					   

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//SCL
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//SDA
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

u8 ReBytes(u8 inchar);
void OLED_FZShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_FZShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);

//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(u8 GPIOx,u32 OLEDSCL,u32 OLEDSDA);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,char *p,u8 size);
void OLED_FZShowString(u8 x,u8 y,const u8 *p,u8 size);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void fill_picture(unsigned char fill_Data);

#endif  
