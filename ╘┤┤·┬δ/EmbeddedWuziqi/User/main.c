#include "stm32f2xx.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "touch.h"
#include "sys.h"
#include "chess.h"
#include "GUI.h"

u8 height = 8;
u8 width = 8;
extern struct ChessPos chesspos[5][5],chessscreenpos[5][5]; 
extern u8 usart_1_rvfinish;
extern u8 usart_1_rx_data[40];
extern u8 usart_1_rxlen;

/*********************************全局变量声明************************************/
	
	uint16_t x_data = 0,y_data = 0;
	char iChessPos[5][5]={1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2};//黑1，白2
	char Info[5][5]={1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2};
	char Info_new[5][5]={1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2};
	int x_last=-1,y_last=-1;
	int x_Computer_last=-1,y_Computer_last=-1;
	int click_x=-1,click_y=-1;
	int del_click_x=-1,del_click_y=-1;
	int iWinner=0;

/***************************************主函数**************************************/

/*int main()
{ 

	SystemCKInit();
	usart_Init();
	LCDInit();
	TouchScreenInit();
	
	
	//棋子位置(x,y)结构体赋值
	for(int i=0;i<5;i++)
		for(int j=0;j<5;j++)
			{
				chesspos[i][j].x=(u16)(15+51*i);
				chesspos[i][j].y=(u16)(100+50*j);
			}
	for(int i=0;i<5;i++)
		for(int j=0;j<5;j++)
			{
				chessscreenpos[i][j].x=0x0E40-i*0x0300;
				chessscreenpos[i][j].y=0x0550+j*0x0250;
			}
	Welcome();
			
  while(1)
	{
		Touch_Button_Up();
	}
}*/



extern stCheckerBoardPos_t CheckerBoardPos[10][10];
//the coefficient of screen is computed using the last.
void InitCheckerBoardCoordinate(){
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			CheckerBoardPos[i][j].chess_x = 12 + j*24;
			CheckerBoardPos[i][j].chess_y = 12 + i*24;
			CheckerBoardPos[i][j].screen_x = (uint16_t)(-15.06*CheckerBoardPos[i][j].chess_x + 3873.88);
			CheckerBoardPos[i][j].screen_y = (uint16_t)(11.84*CheckerBoardPos[i][j].chess_y + 176);
			CheckerBoardPos[i][j].status = 0;
		}
	}
}

//the total x is 240, and y is 320. There are 11 between x and 20 between y
void PrintMesgToLCD(const char *Mesg, u16 begin_x, u16 begin_y, u8 size){
	//the current x and the current y
	u16 curr_x = begin_x, curr_y = begin_y;
	for(u8 i = 0; i < my_strlen(Mesg); i++){
		//new line
		if(curr_x + 11 > 240){
			curr_x = 10;
			curr_y += 20;
		}
		LCD_ShowChar(curr_x, curr_y, (u8)Mesg[i], size, BLACK);
		curr_x += 11;
	}
}

//calibrate physical coordinate and pixel coordinate
//LCD_x = x_fac * P_x + x_off
//LCD_y = y_fac * P_x + y_off
// useless function
int x_fac, x_off, y_fac, y_off;
void CalibtateScreen(){
	u16 pos_temp[4][2];
	LCD_ColourFillScreen(GBLUE);
	LCD_DrawChess(20, 20, BLACK);
	LCD_DrawChess(240 - 20, 20, WHITE);
	LCD_DrawChess(240-20, 320 -20, YELLOW);
	LCD_DrawChess(20, 320-20, GREEN);
	for(int i = 0; i < 4; i++){
		while(x_data == 0 && y_data == 0);//blocked, waiting for click
		pos_temp[i][0] = x_data;
		pos_temp[i][i] = y_data;
		//reset
		x_data = 0;
		y_data = 0;
	}
}

//stm32 wait for PC ready
void Status_1(){
	const char *Mesg = "blocked, waiting for PC ready!";
	PrintMesgToLCD(Mesg, 10, 10, 24);
}

void Status_2(){
	LCD_InitCheckerBoard();
	const char *Mesg = "blocked, waiting for player action!";
	PrintMesgToLCD(Mesg, 10, 10, 24);
}

int main(){
	SystemCKInit();
	usart_Init();
	LCDInit();
	TouchScreenInit();

	//CalibtateScreen();	
	
	InitCheckerBoardCoordinate();
	LCD_InitCheckerBoard();
	while(1){
		//blocked, waiting for player action
		while(1){
			int flag = 0;
			ClickToPutChess(&flag);
			if(flag){
				break;
			}
		}

		//blocked, waiting for computer action
		while(1){
			if(IsCompleteReceive()){
				u16 len = USART_RX_STA & 0x3fff;//得到此次接收到的数据长度
				USART_RX_STA = 0;//reset

				if(len != 2){
					const char *Mesg = "Receive Data Length Error!";
					PrintMesgToLCD(Mesg, 10, 30, 24);
				}CheckerBoardPos[USART_RX_BUF[0]][USART_RX_BUF[1]].status = 2;
				LCD_DrawChess(CheckerBoardPos[USART_RX_BUF[0]][USART_RX_BUF[1]].chess_x,CheckerBoardPos[USART_RX_BUF[0]][USART_RX_BUF[1]].chess_y, WHITE);
				break;
			}
		}

	}

	//LCD_ColourFillScreen(GBLUE);
	//Status_1();	
	//LCD_DrawChess(10, 100, BLACK);
	/*while(1){
	if(IsCompleteReceive())
		{					   
			u16 len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			u16 a = 230;
			//LCD_ColourFillScreen(GBLUE);

			LCD_ShowChar(10,250,len+33,24,BLACK);//to see the number of len
			for(u16 t=0;t<len;t++)
			{
					
				LCD_ShowChar(a,210,USART_RX_BUF[t],24,BLACK);
				u16 b = 1<<15-1;
				while (b)
				{
					b--;
				}
				
				a -= 11;
			}
			USART_RX_STA=0;
			//if break here ,there has not content on the screen
			//break;

			const char *return_message = "STM32 received PC's message!\r\n";
			usart_send(USART1, (u8 *)return_message, my_strlen(return_message));
			break;
		}
	}*/
}
