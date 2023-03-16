#include "lcd.h"
#include "sys.h"
#include "fonts.h"

void LCDInit()
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;
	/* Enable GPIOs clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOC, ENABLE);

	/* Enable FSMC clock */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PC0 推挽输出 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits (GPIOD,GPIO_Pin_12);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //PC0 推挽输出 背光
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits (GPIOC,GPIO_Pin_0);
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC); 

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 |
	GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* GPIOE configuration */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 |	GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 | GPIO_Pin_11|
		GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/*-- FSMC Configuration ------------------------------------------------------*/
	p.FSMC_AddressSetupTime = 6;
	p.FSMC_AddressHoldTime = 1;
	p.FSMC_DataSetupTime = 9;
	p.FSMC_BusTurnAroundDuration = 1;
	p.FSMC_CLKDivision = 0;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_A;

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;  
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

	/*!< Enable FSMC Bank1_SRAM1 Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
  Delay_ms(200);
  GPIO_ResetBits (GPIOD,GPIO_Pin_12);
  Delay_ms(500);
  GPIO_SetBits (GPIOD,GPIO_Pin_12);
  Delay_ms(500);
 	Delay_ms(50); 					// delay 50 ms 
 	LCD_IO_WriteReg(0xCF);  
	LCD_IO_WriteData(0x00); 
	LCD_IO_WriteData(0xC1); 
	LCD_IO_WriteData(0X30); 
	LCD_IO_WriteReg(0xED);  
	LCD_IO_WriteData(0x64); 
	LCD_IO_WriteData(0x03); 
	LCD_IO_WriteData(0X12); 
	LCD_IO_WriteData(0X81); 
	LCD_IO_WriteReg(0xE8);  
	LCD_IO_WriteData(0x85); 
	LCD_IO_WriteData(0x10); 
	LCD_IO_WriteData(0x7A); 
	LCD_IO_WriteReg(0xCB);  
	LCD_IO_WriteData(0x39); 
	LCD_IO_WriteData(0x2C); 
	LCD_IO_WriteData(0x00); 
	LCD_IO_WriteData(0x34); 
	LCD_IO_WriteData(0x02); 
	LCD_IO_WriteReg(0xF7);  
	LCD_IO_WriteData(0x20); 
	LCD_IO_WriteReg(0xEA);  
	LCD_IO_WriteData(0x00); 
	LCD_IO_WriteData(0x00); 
	LCD_IO_WriteReg(0xC0);    //Power control 
	LCD_IO_WriteData(0x1B);   //VRH[5:0] 
	LCD_IO_WriteReg(0xC1);    //Power control 
	LCD_IO_WriteData(0x01);   //SAP[2:0];BT[3:0] 
	LCD_IO_WriteReg(0xC5);    //VCM control 
	LCD_IO_WriteData(0x30); 	 //3F
	LCD_IO_WriteData(0x30); 	 //3C
	LCD_IO_WriteReg(0xC7);    //VCM control2 
	LCD_IO_WriteData(0XB7); 
	LCD_IO_WriteReg(0x36);    // Memory Access Control 
	LCD_IO_WriteData(0x48); 
	LCD_IO_WriteReg(0x3A);   
	LCD_IO_WriteData(0x55); 
	LCD_IO_WriteReg(0xB1);   
	LCD_IO_WriteData(0x00);   
	LCD_IO_WriteData(0x1A); 
	LCD_IO_WriteReg(0xB6);    // Display Function Control 
	LCD_IO_WriteData(0x0A); 
	LCD_IO_WriteData(0xA2); 
	LCD_IO_WriteReg(0xF2);    // 3Gamma Function Disable 
	LCD_IO_WriteData(0x00); 
	LCD_IO_WriteReg(0x26);    //Gamma curve selected 
	LCD_IO_WriteData(0x01); 
	LCD_IO_WriteReg(0xE0);    //Set Gamma 
	LCD_IO_WriteData(0x0F); 
	LCD_IO_WriteData(0x2A); 
	LCD_IO_WriteData(0x28); 
	LCD_IO_WriteData(0x08); 
	LCD_IO_WriteData(0x0E); 
	LCD_IO_WriteData(0x08); 
	LCD_IO_WriteData(0x54); 
	LCD_IO_WriteData(0XA9); 
	LCD_IO_WriteData(0x43); 
	LCD_IO_WriteData(0x0A); 
	LCD_IO_WriteData(0x0F); 
	LCD_IO_WriteData(0x00); 
	LCD_IO_WriteData(0x00); 
	LCD_IO_WriteData(0x00); 
	LCD_IO_WriteData(0x00); 		 
	LCD_IO_WriteReg(0XE1);    //Set Gamma 
	LCD_IO_WriteData(0x00); 
	LCD_IO_WriteData(0x15); 
	LCD_IO_WriteData(0x17); 
	LCD_IO_WriteData(0x07); 
	LCD_IO_WriteData(0x11); 
	LCD_IO_WriteData(0x06); 
	LCD_IO_WriteData(0x2B); 
	LCD_IO_WriteData(0x56); 
	LCD_IO_WriteData(0x3C); 
	LCD_IO_WriteData(0x05); 
	LCD_IO_WriteData(0x10); 
	LCD_IO_WriteData(0x0F); 
	LCD_IO_WriteData(0x3F); 
	LCD_IO_WriteData(0x3F); 
	LCD_IO_WriteData(0x0F); 
	LCD_IO_WriteReg(0x2B); 
	LCD_IO_WriteData(0x00);
	LCD_IO_WriteData(0x00);
	LCD_IO_WriteData(0x01);
	LCD_IO_WriteData(0x3f);
	LCD_IO_WriteReg(0x2A); 
	LCD_IO_WriteData(0x00);
	LCD_IO_WriteData(0x00);
	LCD_IO_WriteData(0x00);
	LCD_IO_WriteData(0xef);	 
	LCD_IO_WriteReg(0x11); //Exit Sleep
	Delay_ms(120);
	LCD_IO_WriteReg(0x29); //display on
}

/***********************************写指令函数*************************************/
void LCD_IO_WriteReg(uint8_t Reg) 
{
  /* Write 16-bit Index, then Write Reg */
   *(__IO uint16_t *)FSMC_LCD_REG = Reg;
}

/***********************************写数据函数*************************************/
void LCD_IO_WriteData(uint16_t Data) 
{
  /* Write 16-bit Reg */
  *(__IO uint16_t *)FSMC_LCD_DATA = Data;
}
/***********************************读数据函数*************************************/
uint16_t LCD_IO_ReadData(void)
{
	return ( * ( __IO uint16_t * ) ( FSMC_LCD_DATA ) );
}

/***********************************设置光标函数*************************************/
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	LCD_IO_WriteReg(0x2A);
	LCD_IO_WriteData(Xpos>>8);
	LCD_IO_WriteData(Xpos&0XFF);
	
	LCD_IO_WriteReg(0x2B);
	LCD_IO_WriteData(Ypos>>8);
	LCD_IO_WriteData(Ypos&0XFF);
}

/***********************************打点函数*************************************/
void LCD_DrawPoint(u16 x,u16 y,uint16_t GRB)
{
        LCD_SetCursor(x,y);
			  LCD_IO_WriteReg(0x2C);
			 	LCD_IO_WriteData(GRB);
}

/***********************************全屏变色函数*************************************/
void LCD_ColourFillScreen(uint16_t GRB)
{
	uint16_t height,width;
	uint16_t i,j;
	width=240; 			
	height=320;			
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(0,0+i);
   	
		LCD_IO_WriteReg(0x2C);
		for(j=0;j<width;j++)LCD_IO_WriteData(GRB);
	}	
}

/***********************************直线(矩形)函数*************************************/
void LCD_DrawLine(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t GRB)
{  
	uint16_t height,width;
	uint16_t i,j;
	width=ex-sx+1; 			
	height=ey-sy+1;			
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);
   	
		LCD_IO_WriteReg(0x2C);
		for(j=0;j<width;j++)LCD_IO_WriteData(GRB);
	}		  
}

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,uint16_t GRB)
{  							  
  u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		
 	num=num-' ';
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	
		else if(size==16)temp=asc2_1608[num][t];	
		else if(size==24)temp=asc2_2412[num][t];	
		else return;
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_DrawPoint(x,320-y,GRB);
			temp<<=1;
			y++;
			if(y>=320)return;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=240)return;
				break;
			}
		}  	 
	}  	    	   	 	  
}


//棋盘初始化

/*void LCD_InitCheckerBoard()
{	
	LCD_ColourFillScreen(YELLOW);
	
	

	//X轴
	LCD_DrawLine(15,100,16,300,BLACK);
	LCD_DrawLine(66,100,67,300,BLACK);
	LCD_DrawLine(117,100,118,300,BLACK);
	LCD_DrawLine(168,100,169,300,BLACK);
	LCD_DrawLine(219,100,220,300,BLACK);

	//Y轴
	LCD_DrawLine(15,100,220,101,BLACK);
	LCD_DrawLine(15,150,220,151,BLACK);
	LCD_DrawLine(15,200,220,201,BLACK);
	LCD_DrawLine(15,250,220,251,BLACK);
	LCD_DrawLine(15,300,220,301,BLACK);

	//黑子
	LCD_DrawChess(15,100,BLACK);
	LCD_DrawChess(15,150,BLACK);
	LCD_DrawChess(15,200,BLACK);
	LCD_DrawChess(15,250,BLACK);
	LCD_DrawChess(15,300,BLACK);

	//白子
	LCD_DrawChess(219,100,WHITE);
	LCD_DrawChess(219,150,WHITE);
	LCD_DrawChess(219,200,WHITE);
	LCD_DrawChess(219,250,WHITE);
	LCD_DrawChess(219,300,WHITE);

}*/



/**********************my code***********************/
//棋盘初始化
extern height;
extern width;
void LCD_InitCheckerBoard()
{	
	LCD_ColourFillScreen(YELLOW);

	//left-top to empty 12 pixels respectively, and the interval between two line is 24 pixels
	//| | | | | | ->
	//| | | | | | ->
	//| | | | | | ->
	//| | | | | | ->
	int begin_a = 12;
	for(int i = 0; i < 8; i++){	
		LCD_DrawLine(begin_a,12,begin_a+1,180,BLACK);
		begin_a += 24;
	}

	//________________
	//________________
	//________________
	//________________
	int begin_b = 12;
	for(int i = 0; i < 8; i++){
		LCD_DrawLine(12, begin_b, 180, begin_b+1, BLACK);
		begin_b += 24;
	}
}



//棋子
void LCD_DrawChess(uint16_t Xpos,uint16_t Ypos,uint16_t GRB)
{
	uint16_t Radius=10;//设置半径为10
	uint16_t x,y,r=Radius;
	for(y=Ypos - r;y<Ypos +r;y++)//画圆
	{
		for(x=Xpos - r;x<Xpos+r;x++)
		{
			if(((x-Xpos)*(x-Xpos)+(y-Ypos)*(y-Ypos)) <= r*r)		LCD_DrawPoint(x,y,GRB);
		}
	}
}

//画矩形边框
void LCD_DrawRectangle(uint16_t Xpos,uint16_t Ypos,uint16_t GRB)
{
	LCD_DrawLine(Xpos-10,Ypos-10,Xpos+10,Ypos-10,GRB);
	LCD_DrawLine(Xpos-10,Ypos+10,Xpos+10,Ypos+10,GRB);
	LCD_DrawLine(Xpos-10,Ypos-10,Xpos-10,Ypos+10,GRB);
	LCD_DrawLine(Xpos+10,Ypos-10,Xpos+10,Ypos+10,GRB);
}


