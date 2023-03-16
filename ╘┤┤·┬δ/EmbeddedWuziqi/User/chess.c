#include "chess.h"
#include "lcd.h"
#include "touch.h"
#include "usart.h"
#include "sys.h"

struct ChessPos chesspos[5][5],chessscreenpos[5][5];
u16 GlobalData[8][5][5]; 

extern uint16_t x_data,y_data;
extern u8 height, width;

/*****************************my code*****************/
stCheckerBoardPos_t CheckerBoardPos[10][10];


//ToDo: the work of eliminate shake(average x_data and y_data)
void ClickToPutChess(int *flag){
	for(u8 i = 0; i < 8; i++){
		for(u8 j = 0; j < 8; j++){
			if(!CheckerBoardPos[i][j].status)
			if(x_data >= (CheckerBoardPos[i][j].screen_x-50) && x_data <= (CheckerBoardPos[i][j].screen_x+50))
				if(y_data >= (CheckerBoardPos[i][j].screen_y-50) && y_data <= (CheckerBoardPos[i][j].screen_y+50)){
					LCD_DrawChess(CheckerBoardPos[i][j].chess_x, CheckerBoardPos[i][j].chess_y, BLACK);


					LCD_DrawLine(0, 320-30, 40, 320-10, YELLOW);//erase old char
					LCD_ShowChar(10, 10, i+48, 24, BLACK);//48 correspond to '0' in ascci table
					LCD_ShowChar(30, 10, j+48, 24, BLACK);


					CheckerBoardPos[i][j].status = 1;

					char temp[2];
					temp[0] = (char)(i+48);
					temp[1] = (char)(j+48);
					usart_send(USART1, (u8 *)temp, 2);

					//USART_SendData(USART1, i+48);
					//the bug comes from the first char have not complete to send when I send the second char? See function uart_send
					//USART_SendData(USART1, j+48);

					//reset
					x_data = 0;
					y_data = 0;

					*flag = 1;
					return;
				}	
		}
	}
}

void TouchChess(void)
{
  int  flag=0;
	for(int i=0;i<5&&flag==0&&iWinner==0;i++)                                                                                                                                                                                                                                                                                                                               		
	 for(int i=0;i<5;i++)
		   for(int j=0;j<5&&flag==0&&iWinner==0;j++)
			   {
					 //如果触摸位置在某个棋盘位置的范围内
					 if(x_data>=(chessscreenpos[i][j].x-200) && x_data<=(chessscreenpos[i][j].x+200))
						 if(y_data>=(chessscreenpos[i][j].y-100) && y_data<=(chessscreenpos[i][j].y+100))
						 {
							 //if the position of board is chess
							 if(iChessPos[i][j]==1||iChessPos[i][j]==2)
							 {
								 if(Judge_Rule1(i,j)==0)
								 {
									 flag=1;
									 break;
								 }
								 //draw a rectangle around the chess to imply the chess is chosen
								 LCD_DrawRectangle(chesspos[i][j].x,chesspos[i][j].y,BLACK);
								 if(click_x!=-1&&click_y!=-1)//所选棋子发生变化choose another chess
								 {
									 RepairChessboard(click_x,click_y);
									 if(iChessPos[click_x][click_y]==1)
									   LCD_DrawChess(chesspos[click_x][click_y].x,chesspos[click_x][click_y].y,BLACK);
								   else if(iChessPos[click_x][click_y]==2)
										 LCD_DrawChess(chesspos[click_x][click_y].x,chesspos[click_x][click_y].y,WHITE);
								 }
								 //the position of the chosen chess
								 click_x=i;
								 click_y=j;
							 }
							 //if the position of board is not a chess
							 else
							 {
								 //if the position(i, j) the chosen chess(click_x, clisck_y) will go is the four direction around the chess
								 if(((i-click_x)==1&&(j-click_y)==0)||((i-click_x)==-1&&(j-click_y)==0)||((i-click_x)==0&&(j-click_y)==1)||((i-click_x)==0&&(j-click_y)==-1))
								 {
									 RepairChessboard(click_x,click_y);
									 if(iChessPos[click_x][click_y]==1)
									   LCD_DrawChess(chesspos[i][j].x,chesspos[i][j].y,BLACK);
								   else if(iChessPos[click_x][click_y]==2)
										 LCD_DrawChess(chesspos[i][j].x,chesspos[i][j].y,WHITE);
									 iChessPos[i][j]=iChessPos[click_x][click_y];
									 iChessPos[click_x][click_y]=0;
								 }								 
								 x_last=i;
								 y_last=j;
								 Judge_Rule2();
								 Winner();
							 }
						 }
				 }
}

//基本规则判断（黑先行，黑白各一步）
int Judge_Rule1(int x,int y)
{
	if(x_last==-1&&y_last==-1) //若x_last,y_last为初值，则为黑子行
	{
		if(iChessPos[x][y]==1)
			return 1;
		else
			return 0;
	}
	else//若不为初值，则根据x_last,y_last即上次移动的子的颜色判断黑子行或白子行
	{
		if(iChessPos[x][y]!=iChessPos[x_last][y_last])
			return 1;
		else
			return 0;
	}
}

//二吃一规则判定
void Judge_Rule2_Computer()
{
	for(int i=0;i<5;i++)
		for(int j=0;j<4;j++)
		{
			if(j==0)
			{
				if(iChessPos[i][j]==1&&iChessPos[i][j+1]==1&&iChessPos[i][j+2]==2)
				{
					if(iChessPos[i][j+3]!=2&&(x_Computer_last!=i)&&(y_Computer_last!=j+2)&&((x_last==i&&y_last==j)||(x_last==i&&y_last==j+1)))
					{
						RepairChessboard(i,j+2);
						iChessPos[i][j+2]=0;
					}
				}
			}
			else if(j==1)
			{
				if(iChessPos[i][j]==1&&iChessPos[i][j+1]==1&&iChessPos[i][j+2]==2)
				{
					if(iChessPos[i][j+3]!=2&&(x_Computer_last!=i)&&(y_Computer_last!=j+2)&&((x_last==i&&y_last==j)||(x_last==i&&y_last==j+1)))
					{
						RepairChessboard(i,j+2);
						iChessPos[i][j+2]=0;
					}
				}
				if(iChessPos[i][j]==1&&iChessPos[i][j+1]==1&&iChessPos[i][j-1]==2)
				{
					if((x_Computer_last!=i)&&(y_Computer_last!=j-1)&&((x_last==i&&y_last==j)||(x_last==i&&y_last==j+1)))
					{
						RepairChessboard(i,j-1);
						iChessPos[i][j-1]=0;
					}
				}
			}
			else if(j==2)
			{
				if(iChessPos[i][j]==1&&iChessPos[i][j+1]==1&&iChessPos[i][j-1]==2)
				{
					if(iChessPos[i][j-2]!=2&&(x_Computer_last!=i)&&(y_Computer_last!=j-1)&&((x_last==i&&y_last==j)||(x_last==i&&y_last==j+1)))
					{
						RepairChessboard(i,j-1);
						iChessPos[i][j-1]=0;
					}
				}
				if(iChessPos[i][j]==1&&iChessPos[i][j+1]==1&&iChessPos[i][j+2]==2)
				{
					if((x_Computer_last!=i)&&(y_Computer_last!=j+1)&&((x_last==i&&y_last==j)||(x_last==i&&y_last==j+1)))
						RepairChessboard(i,j+2);
						iChessPos[i][j+2]=0;
				}
			}
			else if(j==3)
			{
				if(iChessPos[i][j]==1&&iChessPos[i][j+1]==1&&iChessPos[i][j-1]==2)
				{
					if(iChessPos[i][j-2]!=2&&(x_Computer_last!=i)&&(y_Computer_last!=j-1)&&((x_last==i&&y_last==j)||(x_last==i&&y_last==j+1)))
					{
						RepairChessboard(i,j-1);
						iChessPos[i][j-1]=0;
					}
				}
			}
//			if(j!=0)
//			{
//				if(iChessPos[i][j-1]==1&&iChessPos[i][j]==2&&iChessPos[i][j+1]==1)
//				{
//						RepairChessboard(i,j);
//						iChessPos[i][j]=0;
//				}

//			}
		}
	for(int j=0;j<5;j++)
		for(int i=0;i<4;i++)
		{
			if(i==0)
			{
				if(iChessPos[i][j]==1&&iChessPos[i+1][j]==1&&iChessPos[i+2][j]==2)
				{
					if(iChessPos[i+3][j]!=2&&(x_Computer_last!=i+2)&&(y_Computer_last!=j)&&((x_last==i&&y_last==j)||(x_last==i+1&&y_last==j)))
					{
						RepairChessboard(i+2,j);
						iChessPos[i+2][j]=0;
					}
				}
			}
			else if(i==1)
			{
				if(iChessPos[i][j]==1&&iChessPos[i+1][j]==1&&iChessPos[i+2][j]==2)
				{
					if(iChessPos[i+3][j]!=2&&(x_Computer_last!=i+2)&&(y_Computer_last!=j)&&((x_last==i&&y_last==j)||(x_last==i+1&&y_last==j)))
					{
						RepairChessboard(i+2,j);
						iChessPos[i+2][j]=0;
					}
				}
				if(iChessPos[i][j]==1&&iChessPos[i+1][j]==1&&iChessPos[i-1][j]==2)
				{
					if((x_Computer_last!=i-1)&&(y_Computer_last!=j)&&((x_last==i&&y_last==j)||(x_last==i+1&&y_last==j)))
						RepairChessboard(i-1,j);
						iChessPos[i-1][j]=0;
				}
			}
			else if(i==2)
			{
				if(iChessPos[i][j]==1&&iChessPos[i+1][j]==1&&iChessPos[i-1][j]==2)
				{
					if(iChessPos[i-2][j]!=2&&(x_Computer_last!=i-1)&&(y_Computer_last!=j)&&((x_last==i&&y_last==j)||(x_last==i+1&&y_last==j)))
					{
						RepairChessboard(i-1,j);
						iChessPos[i-1][j]=0;
					}
				}
				if(iChessPos[i][j]==1&&iChessPos[i+1][j]==1&&iChessPos[i+2][j]==2)
				{
					if((x_Computer_last!=i+2)&&(y_Computer_last!=j)&&((x_last==i&&y_last==j)||(x_last==i+1&&y_last==j)))
						RepairChessboard(i+2,j);
						iChessPos[i+2][j]=0;
				}
			}
			else if(i==3)
			{
				if(iChessPos[i][j]==1&&iChessPos[i+1][j]==1&&iChessPos[i-1][j]==2)
				{
					if(iChessPos[i-2][j]!=2&&(x_Computer_last!=i-1)&&(y_Computer_last!=j)&&((x_last==i&&y_last==j)||(x_last==i+1&&y_last==j)))
					{
						RepairChessboard(i-1,j);
						iChessPos[i-1][j]=0;
					}
				}
			}
//			if(i!=0)
//			{
//				if(iChessPos[i-1][j]==1&&iChessPos[i][j]==2&&iChessPos[i+1][j]==1)
//				{
//						RepairChessboard(i,j);
//						iChessPos[i][j]=0;
//				}
//			}
		}
}

void TouchChess_VS_Computer(void)
{
  int  flag=0;
	for(int i=0;i<5&&flag==0&&iWinner==0;i++)                                                                                                                                                                                                                                                                                                                               		
	 for(int i=0;i<5;i++)
		   for(int j=0;j<5&&flag==0&&iWinner==0;j++)
			   {
					 if(x_data>=(chessscreenpos[i][j].x-200) && x_data<=(chessscreenpos[i][j].x+200))
						 if(y_data>=(chessscreenpos[i][j].y-100) && y_data<=(chessscreenpos[i][j].y+100))
						 {
							 if(iChessPos[i][j]==1)
							 { 
								 LCD_DrawRectangle(chesspos[i][j].x,chesspos[i][j].y,BLACK);
								 if(click_x!=-1&&click_y!=-1)//所选棋子发生变化
								 {
									 RepairChessboard(click_x,click_y);
									 if(iChessPos[click_x][click_y]==1)
									   LCD_DrawChess(chesspos[click_x][click_y].x,chesspos[click_x][click_y].y,BLACK);
								   else if(iChessPos[click_x][click_y]==2)
										 LCD_DrawChess(chesspos[click_x][click_y].x,chesspos[click_x][click_y].y,WHITE);
								 }
								 click_x=i;
								 click_y=j;
							 }
							 else
							 {
								 if(((i-click_x)==1&&(j-click_y)==0)||((i-click_x)==-1&&(j-click_y)==0)||((i-click_x)==0&&(j-click_y)==1)||((i-click_x)==0&&(j-click_y)==-1))
								 {
									 RepairChessboard(click_x,click_y);
									 if(iChessPos[click_x][click_y]==1)
									   LCD_DrawChess(chesspos[i][j].x,chesspos[i][j].y,BLACK);
								   else if(iChessPos[click_x][click_y]==2)
										 LCD_DrawChess(chesspos[i][j].x,chesspos[i][j].y,WHITE);
									 iChessPos[i][j]=iChessPos[click_x][click_y];
									 Info[i][j]=Info[click_x][click_y];
									 iChessPos[click_x][click_y]=0;
									 Info[click_x][click_y]=0;
								 }								 
								 x_last=i;
								 y_last=j;
								 x_Computer_last=-1;
								 y_Computer_last=-1;
								 //Judge_Rule2_Computer();
								 Winner();
							 }
						 }
				 }
}

void VSComputer(void)
{
	TouchChess_VS_Computer();
	if(usart_1_rvfinish==1)//如果有信号发过来
	{
				//如果是开始信号
				if(usart_1_rx_data[0]=='S')
				{
						TouchChess_VS_Computer();
					  usart_sendArray(USART1,iChessPos,5,5);
				}
				//如果是电脑走棋的信号
				else
				{
					// 更新Info信息（当发送来的信息更新的时候）
					for(int j=0;j<25;)
					{
						for(int i=0;i<usart_1_rxlen;i++)
						{
							if(i%3==2)
							{
									if(Info_new[j/5][j%5]!=(char)usart_1_rx_data[i]-48)
									{
										Info_new[j/5][j%5]=(char)usart_1_rx_data[i]-48;
										Info[j/5][j%5]=(char)usart_1_rx_data[i]-48;
									}

								  j++;
							}
						}
					}
					
					
					// 判断是哪个棋子改变了位置
					for(int i=0;i<5;i++)
					{
						for(int j=0;j<5;j++)
						{
							if(iChessPos[i][j]-Info[i][j]!=0) //位置改变了
							{
								iChessPos[i][j]=Info[i][j];
								if(iChessPos[i][j]==0)
								{
									x_Computer_last=i;
								  y_Computer_last=j;
									RepairChessboard(i,j);
								}
								if(iChessPos[i][j]==1)
								{
									x_Computer_last=i;
								  y_Computer_last=j;
									LCD_DrawChess(chesspos[i][j].x,chesspos[i][j].y,BLACK);
								}
								else if(iChessPos[i][j]==2)
								{
									LCD_DrawChess(chesspos[i][j].x,chesspos[i][j].y,WHITE);
									x_Computer_last=i;
								  y_Computer_last=j;
									Judge_Rule2_Computer();
								}
							}
						}
					}
					TouchChess_VS_Computer();
					Judge_Rule2_Computer();

					usart_sendArray(USART1,iChessPos,5,5);
					
				}
    		usart_1_rvfinish=0;
			  // Delay_ms (200);

		    usart_1_rxlen = 0;
	  }
		
}

//二吃一规则判定
void Judge_Rule2()
{
	for(int i=0;i<5;i++)
		for(int j=0;j<4;j++)
		{
			if(j==0)
			{
				if(iChessPos[i][j]==1&&iChessPos[i][j+1]==1&&iChessPos[i][j+2]==2)
				{
					if(iChessPos[i][j+3]!=2&&!(x_last==i&&y_last==j+2)&&((x_last==i&&y_last==j)||(x_last==i&&y_last==j+1)))
					{
						RepairChessboard(i,j+2);
						iChessPos[i][j+2]=0;
					}
				}
				else if(iChessPos[i][j]==2&&iChessPos[i][j+1]==2&&iChessPos[i][j+2]==1)
				{
					if(iChessPos[i][j+3]!=1&&!(x_last==i&&y_last==j+2)&&((x_last==i&&y_last==j)||(x_last==i&&y_last==j+1)))
					{
						RepairChessboard(i,j+2);
						iChessPos[i][j+2]=0;
					}
				}
			}
			else if(j==1)
			{
				if(iChessPos[i][j]==1&&iChessPos[i][j+1]==1&&iChessPos[i][j+2]==2)
				{
					if(iChessPos[i][j+3]!=2&&!(x_last==i&&y_last==j+2)&&((x_last==i&&y_last==j)||(x_last==i&&y_last==j+1)))
					{
						RepairChessboard(i,j+2);
						iChessPos[i][j+2]=0;
					}
				}
				else if(iChessPos[i][j]==2&&iChessPos[i][j+1]==2&&iChessPos[i][j+2]==1)
				{
					if(iChessPos[i][j+3]!=1&&!(x_last==i&&y_last==j+2)&&((x_last==i&&y_last==j)||(x_last==i&&y_last==j+1)))
					{
						RepairChessboard(i,j+2);
						iChessPos[i][j+2]=0;
					}
				}
				if(iChessPos[i][j]==1&&iChessPos[i][j+1]==1&&iChessPos[i][j-1]==2)
				{
					if(!(x_last==i&&y_last==j-1)&&((x_last==i&&y_last==j)||(x_last==i&&y_last==j+1)))
					{
						RepairChessboard(i,j-1);
						iChessPos[i][j-1]=0;
					}
				}
				else if(iChessPos[i][j]==2&&iChessPos[i][j+1]==2&&iChessPos[i][j-1]==1)
				{
					if(!(x_last==i&&y_last==j-1)&&((x_last==i&&y_last==j)||(x_last==i&&y_last==j+1)))
					{
						RepairChessboard(i,j-1);
						iChessPos[i][j-1]=0;
					}
				}
			}
			else if(j==2)
			{
				if(iChessPos[i][j]==1&&iChessPos[i][j+1]==1&&iChessPos[i][j-1]==2)
				{
					if(iChessPos[i][j-2]!=2&&!(x_last==i&&y_last==j-1)&&((x_last==i&&y_last==j)||(x_last==i&&y_last==j+1)))
					{
						RepairChessboard(i,j-1);
						iChessPos[i][j-1]=0;
					}
				}
				else if(iChessPos[i][j]==2&&iChessPos[i][j+1]==2&&iChessPos[i][j-1]==1)
				{
					if(iChessPos[i][j-2]!=1&&!(x_last==i&&y_last==j-1)&&((x_last==i&&y_last==j)||(x_last==i&&y_last==j+1)))
					{
						RepairChessboard(i,j-1);
						iChessPos[i][j-1]=0;
					}
				}
				if(iChessPos[i][j]==1&&iChessPos[i][j+1]==1&&iChessPos[i][j+2]==2)
				{
					if(!(x_last==i&&y_last==j+2)&&((x_last==i&&y_last==j)||(x_last==i&&y_last==j+1)))
					{
						RepairChessboard(i,j+2);
						iChessPos[i][j+2]=0;
					}
				}
				else if(iChessPos[i][j]==2&&iChessPos[i][j+1]==2&&iChessPos[i][j+2]==1)
				{
					if(!(x_last==i&&y_last==j+2)&&((x_last==i&&y_last==j)||(x_last==i&&y_last==j+1)))
					{
						RepairChessboard(i,j+2);
						iChessPos[i][j+2]=0;
					}
				}
			}
			else if(j==3)
			{
				if(iChessPos[i][j]==1&&iChessPos[i][j+1]==1&&iChessPos[i][j-1]==2)
				{
					if(iChessPos[i][j-2]!=2&&!(x_last==i&&y_last==j-1)&&((x_last==i&&y_last==j)||(x_last==i&&y_last==j+1)))
					{
						RepairChessboard(i,j-1);
						iChessPos[i][j-1]=0;
					}
				}
				else if(iChessPos[i][j]==2&&iChessPos[i][j+1]==2&&iChessPos[i][j-1]==1)
				{
					if(iChessPos[i][j-2]!=1&&!(x_last==i&&y_last==j-1)&&((x_last==i&&y_last==j)||(x_last==i&&y_last==j+1)))
					{
						RepairChessboard(i,j-1);
						iChessPos[i][j-1]=0;
					}
				}
			}
			if(j!=0)
			{
				if(iChessPos[i][j-1]==1&&iChessPos[i][j]==2&&iChessPos[i][j+1]==1)
				{
					if((x_last==i&&y_last==j-1)||(x_last==i&&y_last==j+1))
					{
						RepairChessboard(i,j);
						iChessPos[i][j]=0;
					}
				}
				else if(iChessPos[i][j-1]==2&&iChessPos[i][j]==1&&iChessPos[i][j+1]==2)
				{
					if((x_last==i&&y_last==j-1)||(x_last==i&&y_last==j+1))
					{
						RepairChessboard(i,j);
						iChessPos[i][j]=0;
					}
				}
			}
		}
	for(int j=0;j<5;j++)
		for(int i=0;i<4;i++)
		{
			if(i==0)
			{
				if(iChessPos[i][j]==1&&iChessPos[i+1][j]==1&&iChessPos[i+2][j]==2)
				{
					if(iChessPos[i+3][j]!=2&&!(x_last==i+2&&y_last==j)&&((x_last==i&&y_last==j)||(x_last==i+1&&y_last==j)))
					{
						RepairChessboard(i+2,j);
						iChessPos[i+2][j]=0;
					}
				}
				else if(iChessPos[i][j]==2&&iChessPos[i+1][j]==2&&iChessPos[i+2][j]==1)
				{
					if(iChessPos[i+3][j]!=1&&!(x_last==i+2&&y_last==j)&&((x_last==i&&y_last==j)||(x_last==i+1&&y_last==j)))
					{
						RepairChessboard(i+2,j);
						iChessPos[i+2][j]=0;
					}
				}
			}
			else if(i==1)
			{
				if(iChessPos[i][j]==1&&iChessPos[i+1][j]==1&&iChessPos[i+2][j]==2)
				{
					if(iChessPos[i+3][j]!=2&&!(x_last==i+2&&y_last==j)&&((x_last==i&&y_last==j)||(x_last==i+1&&y_last==j)))
					{
						RepairChessboard(i+2,j);
						iChessPos[i+2][j]=0;
					}
				}
				else if(iChessPos[i][j]==2&&iChessPos[i+1][j]==2&&iChessPos[i+2][j]==1)
				{
					if(iChessPos[i+3][j]!=1&&!(x_last==i+2&&y_last==j)&&((x_last==i&&y_last==j)||(x_last==i+1&&y_last==j)))
					{
						RepairChessboard(i+2,j);
						iChessPos[i+2][j]=0;
					}
				}
				if(iChessPos[i][j]==1&&iChessPos[i+1][j]==1&&iChessPos[i-1][j]==2)
				{
					if(!(x_last==i-1&&y_last==j)&&((x_last==i&&y_last==j)||(x_last==i+1&&y_last==j)))
					{
						RepairChessboard(i-1,j);
						iChessPos[i-1][j]=0;
					}
				}
				else if(iChessPos[i][j]==2&&iChessPos[i+1][j]==2&&iChessPos[i-1][j]==1)
				{
					if(!(x_last==i-1&&y_last==j)&&((x_last==i&&y_last==j)||(x_last==i+1&&y_last==j)))
					{
						RepairChessboard(i-1,j);
						iChessPos[i-1][j]=0;
					}
				}
			}
			else if(i==2)
			{
				if(iChessPos[i][j]==1&&iChessPos[i+1][j]==1&&iChessPos[i-1][j]==2)
				{
					if(iChessPos[i-2][j]!=2&&!(x_last==i-1&&y_last==j)&&((x_last==i&&y_last==j)||(x_last==i+1&&y_last==j)))
					{
						RepairChessboard(i-1,j);
						iChessPos[i-1][j]=0;
					}
				}
				else if(iChessPos[i][j]==2&&iChessPos[i+1][j]==2&&iChessPos[i-1][j]==1)
				{
					if(iChessPos[i-2][j]!=1&&!(x_last==i-1&&y_last==j)&&((x_last==i&&y_last==j)||(x_last==i+1&&y_last==j)))
					{
						RepairChessboard(i-1,j);
						iChessPos[i-1][j]=0;
					}
				}
				if(iChessPos[i][j]==1&&iChessPos[i+1][j]==1&&iChessPos[i+2][j]==2)
				{
					if(!(x_last==i+2&&y_last==j)&&((x_last==i&&y_last==j)||(x_last==i+1&&y_last==j)))
					{
						RepairChessboard(i+2,j);
						iChessPos[i+2][j]=0;
					}
				}
				else if(iChessPos[i][j]==2&&iChessPos[i+1][j]==2&&iChessPos[i+2][j]==1)
				{
					if(!(x_last==i+2&&y_last==j)&&((x_last==i&&y_last==j)||(x_last==i+1&&y_last==j)))
					{
						RepairChessboard(i+2,j);
						iChessPos[i+2][j]=0;
					}
				}
			}
			else if(i==3)
			{
				if(iChessPos[i][j]==1&&iChessPos[i+1][j]==1&&iChessPos[i-1][j]==2)
				{
					if(iChessPos[i-2][j]!=2&&!(x_last==i-1&&y_last==j)&&((x_last==i&&y_last==j)||(x_last==i+1&&y_last==j)))
					{
						RepairChessboard(i-1,j);
						iChessPos[i-1][j]=0;
					}
				}
				else if(iChessPos[i][j]==2&&iChessPos[i+1][j]==2&&iChessPos[i-1][j]==1)
				{
					if(iChessPos[i-2][j]!=1&&!(x_last==i-1&&y_last==j)&&((x_last==i&&y_last==j)||(x_last==i+1&&y_last==j)))
					{
						RepairChessboard(i-1,j);
						iChessPos[i-1][j]=0;
					}
				}
			}
			if(i!=0)
			{
				if(iChessPos[i-1][j]==1&&iChessPos[i][j]==2&&iChessPos[i+1][j]==1)
				{
					if((x_last==i-1&&y_last==j)||(x_last==i+1&&y_last==j))
					{
						RepairChessboard(i,j);
						iChessPos[i][j]=0;
					}
				}
				else if(iChessPos[i-1][j]==2&&iChessPos[i][j]==1&&iChessPos[i+1][j]==2)
				{
					if((x_last==i-1&&y_last==j)||(x_last==i+1&&y_last==j))
					{
						RepairChessboard(i,j);
						iChessPos[i][j]=0;
					}
				}
			}
		}
}

//胜负判定
void Winner()
{
	int black_num=0;
	int white_num=0;
	for(int i=0;i<5;i++)
	  for(int j=0;j<5;j++)
			{
				if(iChessPos[i][j]==1)
					black_num++;
				else if(iChessPos[i][j]==2)
					white_num++;
			}
	if(black_num==1)
	{
		Print_Win(WHITE);
		iWinner=1;
	}
	else if(white_num==1)
	{
		Print_Win(BLACK);
		iWinner=2;
	}
}

//胜方
void Print_Win(uint16_t GRB)
{
	LCD_DrawChess(120,50,GRB);

}
//修复棋盘
void RepairChessboard(int x,int y)
{
	LCD_DrawRectangle(chesspos[x][y].x,chesspos[x][y].y,YELLOW);
	LCD_DrawChess(chesspos[x][y].x,chesspos[x][y].y,YELLOW);
	LCD_DrawLine(chesspos[x][y].x-10,chesspos[x][y].y,chesspos[x][y].x+10,chesspos[x][y].y+1,BLACK);
	LCD_DrawLine(chesspos[x][y].x,chesspos[x][y].y-10,chesspos[x][y].x+1,chesspos[x][y].y+10,BLACK);
	if(x==0)
	{
		LCD_DrawLine(chesspos[x][y].x-10,chesspos[x][y].y,chesspos[x][y].x-1,chesspos[x][y].y+1,YELLOW);
	}
	else if(x==4)
	{
		LCD_DrawLine(chesspos[x][y].x+2,chesspos[x][y].y,chesspos[x][y].x+10,chesspos[x][y].y+1,YELLOW);
	}
	if(y==0)
	{
		LCD_DrawLine(chesspos[x][y].x,chesspos[x][y].y-10,chesspos[x][y].x+1,chesspos[x][y].y-1,YELLOW);
	}
	else if(y==4)
	{
		LCD_DrawLine(chesspos[x][y].x,chesspos[x][y].y+2,chesspos[x][y].x+1,chesspos[x][y].y+10,YELLOW);
	}
}	

/***********************************中断函数*************************************/
void EXTI2_IRQHandler (void)
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
  {
     x_data=SPI_X();
		 y_data=SPI_Y();
     
     EXTI_ClearITPendingBit(EXTI_Line2);
  }
}

