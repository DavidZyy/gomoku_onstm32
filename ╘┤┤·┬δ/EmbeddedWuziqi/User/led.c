#include "LED.h"

void LED_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;                 	      /*定义一个GPIO_InitTypeDef类型的结构体*/
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	                    /*开启GPIO的外设时钟*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_12|GPIO_Pin_10|GPIO_Pin_8;	       /*选择一控制的GPIO引脚*/    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                          /*设置引脚模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                         /*设置引脚速率为50MHz*/
	GPIO_Init(GPIOE,&GPIO_InitStructure);	                                   /*调用库函数，初始化GPIOC*/
}

void PEOut(int pin)
{
	if(pin==14)
		GPIO_ResetBits(GPIOE,GPIO_Pin_14);
}

