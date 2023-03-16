#ifndef __LCD_H
#define __LCD_H

#include "stm32f2xx.h"

#define u16 uint16_t
#define u8 uint8_t

//»­±ÊÑÕÉ«
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	   0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //×ØÉ«
#define BRRED 			     0XFC07 //×ØºìÉ«
#define GRAY  			     0X8430 //»ÒÉ«

#define FSMC_LCD_DATA ((uint32_t)0x60020000)
#define FSMC_LCD_REG ((uint32_t)0x60000000)

void LCDInit(void);
void LCD_IO_WriteReg(uint8_t Reg) ;
void LCD_IO_WriteData(uint16_t Data) ;
uint16_t LCD_IO_ReadData(void);
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t GRB);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_ColourFillScreen(uint16_t GRB);
void LCD_DrawLine(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t GRB);
void LCD_DrawRectangle(uint16_t Xpos,uint16_t Ypos,uint16_t GRB);
void LCD_DrawChess(uint16_t Xpos,uint16_t Ypos,uint16_t GRB);
void LCD_InitCheckerBoard(void);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,uint16_t GRB);

#endif

