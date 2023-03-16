#include "LED.h"

void LED_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;                 	      /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	                    /*����GPIO������ʱ��*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_12|GPIO_Pin_10|GPIO_Pin_8;	       /*ѡ��һ���Ƶ�GPIO����*/    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                          /*��������ģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                         /*������������Ϊ50MHz*/
	GPIO_Init(GPIOE,&GPIO_InitStructure);	                                   /*���ÿ⺯������ʼ��GPIOC*/
}

void PEOut(int pin)
{
	if(pin==14)
		GPIO_ResetBits(GPIOE,GPIO_Pin_14);
}

