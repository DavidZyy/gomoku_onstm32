#ifndef _USART_H
#define _USART_H
#include "stm32f2xx.h"
#define u32 uint32_t
#define u8 uint8_t

/***********from alientek****************/
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
/***************************************/



u8 IsCompleteReceive();


void usart_Init(void);
void usart_send(USART_TypeDef* USARTx,u8 *data, u8 len);
void usart_sendArray(USART_TypeDef* USARTx,char send_array[][5],int num1,int num2);
u8 usart_ReciveData(void);

#endif

