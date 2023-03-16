#include "touch.h"
#include "lcd.h"

void TouchScreenInit()
{
	  GPIO_InitTypeDef GPIO_InitStructure; 
	  NVIC_InitTypeDef NVIC_InitStructure;
	  SPI_InitTypeDef  SPI_InitStructure;
	  EXTI_InitTypeDef EXTI_InitStructure;
	/*********************************内部向量中断初始化**************************************/
	
    /* Set the Vector Table base address at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
    /* Configure the Priority Group to 2 bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	/********************************************************************************/
	/*********************************外部中断初始化**************************************/
     
     /* Enable GPIOB, GPIOC and AFIO clock */
     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);  
     /* LEDs pins configuration */
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
     //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
     GPIO_Init(GPIOC, &GPIO_InitStructure);
	   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG , ENABLE);
    /* Connect Button EXTI Line to Button GPIO Pin */
     SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource2);
    /* Configure Button EXTI line */
     EXTI_InitStructure.EXTI_Line = EXTI_Line2;
     EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
     EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
     EXTI_InitStructure.EXTI_LineCmd = ENABLE;
     EXTI_Init(&EXTI_InitStructure);
	   EXTI_ClearITPendingBit(EXTI_Line2);
	/********************************************************************************/
	/*********************************触摸屏SPI初始化**************************************/
    
  /* Enable GPIOB, GPIOC and AFIO clock */
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB , ENABLE);  //RCC_APB2Periph_AFIO
     GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_SPI1); //nss  
     GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1 ); //sck
     GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1 ); //miso
     GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1 ); //mosi 
  /* LEDs pins configuration */
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 ;
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
     GPIO_Init(GPIOA, &GPIO_InitStructure);
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // also 100Mhz
     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
     GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; // GPIO_PuPd_DOWN
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5 ;
     GPIO_Init(GPIOA, &GPIO_InitStructure);// SCK,MISO   
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // also 100Mhz
     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
     GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; // GPIO_PuPd_DOWN
     GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
     GPIO_Init(GPIOB, &GPIO_InitStructure);// MOSI
   	 SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
     SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
     SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
     SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
     SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
     SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
     SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
     SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; /* Initialize the SPI_FirstBit member */
     SPI_InitStructure.SPI_CRCPolynomial=7;
     SPI_Init(SPI1, &SPI_InitStructure);
     SPI_Cmd(SPI1, ENABLE);
}

/***********************************获取横向AD函数*************************************/
u32 SPI_X(void) 
{
  u16		i,x;
  GPIO_ResetBits(GPIOA,GPIO_Pin_15);
  SPI_I2S_SendData(SPI1, 0xD0);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 
	SPI_I2S_SendData(SPI1, 0);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 
	SPI_I2S_SendData(SPI1, 0);
	i=SPI_I2S_ReceiveData(SPI1);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 
  x=SPI_I2S_ReceiveData(SPI1);
  x<<=8;	
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	x|=SPI_I2S_ReceiveData(SPI1);
  x>>=3;
  GPIO_SetBits(GPIOA,GPIO_Pin_15); 
	return  x;
}

/***********************************获取纵向AD函数*************************************/
u32 SPI_Y(void) 
{ 

  u16 i,y;
  GPIO_ResetBits(GPIOA,GPIO_Pin_15);
	SPI_I2S_SendData(SPI1, 0x90);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 
	SPI_I2S_SendData(SPI1, 0);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 
	SPI_I2S_SendData(SPI1, 0);
	i=SPI_I2S_ReceiveData(SPI1);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 
  y=SPI_I2S_ReceiveData(SPI1);
	y<<=8;	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	y|=SPI_I2S_ReceiveData(SPI1);
  y>>=3;
  GPIO_SetBits(GPIOA,GPIO_Pin_15); 
	return y;
}

