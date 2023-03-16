#ifndef __CHESS_H
#define __CHESS_H

#include "stm32f2xx.h"

#define u8 uint8_t
#define u16 uint16_t

extern int x_last,y_last;
extern int x_Computer_last,y_Computer_last;
extern char iChessPos[5][5];
extern char Info[5][5];
extern char Info_new[5][5];
extern int click_x,click_y;
extern int iWinner;
extern u8 usart_1_rvfinish;
extern u8 usart_1_rx_data[40];
extern u8 usart_1_rxlen;


//Æå×ÓÎ»ÖÃ
struct ChessPos
{
	uint16_t x;
	uint16_t y;
};

void TouchChess(void);
void RepairChessboard(int x,int y);
int  Judge_Rule1(int x,int y);
void VSComputer(void);
void Judge_Rule2(void);
void Winner(void);
void Print_Win(uint16_t GRB);





typedef struct stCheckerBoardPos_t{
	uint16_t chess_x, chess_y;
	uint16_t screen_x, screen_y;
	//white chess = 2, empty = 0, black chess = 1
	u8 status;
}stCheckerBoardPos_t; 



void ClickToPutChess(int *flag);

#endif

