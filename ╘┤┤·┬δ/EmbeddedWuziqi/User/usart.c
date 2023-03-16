#include "usart.h"

u8 testdata1[] = {0X55,0XAA,0X00,0X00};
u8 usart_1_rvfinish = 0;
u8 usart_1_rx_data[40]={0};
u8 usart_1_rxlen = 0;

void usart_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
  USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;   //tx
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //rx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_Init(USART1, &USART_InitStructure);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//空闲中断
  USART_Cmd(USART1, ENABLE);
	USART_ClearITPendingBit(USART1, USART_IT_TC);//清除中断TC位
		
}

void usart_send(USART_TypeDef* USARTx,u8 *data, u8 len)
{
	
	u8 i; 
	for(i = 0;i<len;i++) 
	{ 
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);  
		USART_SendData(USARTx,*data++); 	
		
	}
}

void usart_sendArray(USART_TypeDef* USARTx,char send_array[][5],int num1,int num2) //两个参数 一是数组(的数据) 二是数组长度1-255 
{
        //串口发送
        int i=0;  //定义一个局部变量  用来 发送字符串 ++运算
        int j=0;
				for(i=0;i<num1;i++)
					for(j=0;j<num2;j++)
          {                
                USART_SendData(USARTx,send_array[i][j]);    //通过库函数  发送数据
                while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);  
                //等待发送完成。   检测 USART_FLAG_TC 是否置1        
          }        
}

u8 usart_ReciveData(void)
{
	u8 Data;
		if(((0x20&USART1->SR)!=0)&&(( 0x10&USART1->CR1)!=0))
		{
			Data = (uint16_t)(USART1->DR & (uint16_t)0x01FF);
		}
	return Data;

}

/*
void USART1_IRQHandler(void)
{
	unsigned char clearidle = clearidle;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {      
    usart_1_rx_data[usart_1_rxlen++]=USART_ReceiveData(USART1);
  } 
	else if(USART_GetITStatus(USART1, USART_IT_IDLE)){
		
		clearidle = USART1->SR;
		clearidle = USART1->DR;
		usart_1_rvfinish = 1;
	}
}*/ 

u8 IsCompleteReceive(){
	if(USART_RX_STA&0x8000)
		return 1;
	return 0;
}

/**********************frome alientek***********************/

u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	unsigned char clearidle = clearidle;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据（每次只接受一个字节的数据吗？）
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//之前已经接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //之前还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;//这次接收到的是0x0d
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
	else if(USART_GetITStatus(USART1, USART_IT_IDLE)){	
		//read register to clear SR
		clearidle = USART1->SR;
		clearidle = USART1->DR;
	}
} 