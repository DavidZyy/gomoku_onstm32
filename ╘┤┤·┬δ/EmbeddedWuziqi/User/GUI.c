#include "GUI.h"
#include "chess.h"

extern uint16_t x_data,y_data;
extern int iWinner;
extern struct ChessPos chessscreenpos[5][5]; 

/*��ť�ṹ������*/
Touch_Button button[BUTTON_NUM];

static void Command_VS_Person(void *btn);
static void Command_VS_Computer(void *btn);

void GUI_DrawRectangle ( uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight, uint16_t GRB )
{
		LCD_DrawLine ( usX_Start, usY_Start, usX_Start + usWidth - 1, usY_Start, GRB);
		LCD_DrawLine ( usX_Start, usY_Start + usHeight - 1, usX_Start + usWidth - 1, usY_Start + usHeight - 1,GRB );
		LCD_DrawLine ( usX_Start, usY_Start, usX_Start, usY_Start + usHeight - 1,GRB );
		LCD_DrawLine ( usX_Start + usWidth - 1, usY_Start, usX_Start + usWidth - 1, usY_Start + usHeight - 1 ,GRB);		
}

static void Draw_Color_Button(void *btn)
{
  Touch_Button *ptr = (Touch_Button *)btn;
  
  /*�ͷŰ���*/
  if(ptr->touch_flag == 0)
  {
    GUI_DrawRectangle(	ptr->start_x,
															ptr->start_y,
															ptr->end_x - ptr->start_x,
															ptr->end_y - ptr->start_y,GRAY);
  }
  else  /*��������*/
  {
    /*��ɫ����*/
    GUI_DrawRectangle(	ptr->start_x,
															ptr->start_y,
															ptr->end_x - ptr->start_x,
															ptr->end_y - ptr->start_y,BLUE);
  } 

//    GUI_DrawRectangle(	ptr->start_x,
//															ptr->start_y,
//															ptr->end_x - ptr->start_x,
//															ptr->end_y - ptr->start_y,WHITE);
  
}

void Touch_Button_Init(void)
{
  /*ѡ������ģʽ or �˻�ģʽ*/
  button[0].start_x = BUTTON_START_X;
  button[0].start_y = 125;
  button[0].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH ;
  button[0].end_y = COLOR_BLOCK_HEIGHT+125;
  button[0].touch_flag = 0;  
  button[0].draw_btn = Draw_Color_Button ;
  button[0].btn_command = Command_VS_Person ;
  
  button[1].start_x = BUTTON_START_X;
  button[1].start_y = COLOR_BLOCK_HEIGHT+175;
  button[1].end_x = BUTTON_START_X+COLOR_BLOCK_WIDTH ;
  button[1].end_y = COLOR_BLOCK_HEIGHT*2+175;
  button[1].touch_flag = 0;  
  button[1].draw_btn = Draw_Color_Button ;
  button[1].btn_command = Command_VS_Computer ;
  
}

///**
//* @brief  Touch_Button_Down ����������ʱ���õĺ������ɴ���������
//* @param  x ����λ�õ�x����
//* @param  y ����λ�õ�y����
//* @retval ��
//*/
//void Touch_Button_Down(uint16_t x,uint16_t y)
//{
//  uint8_t i;
//  for(i=0;i<BUTTON_NUM;i++)
//  {
//    /* �������˰�ť */
//    if(x<=button[i].end_x && y<=button[i].end_y && y>=button[i].start_y && x>=button[i].start_x )
//    {
//      if(button[i].touch_flag == 0)     /*ԭ����״̬Ϊû�а��£������״̬*/
//      {
//      button[i].touch_flag = 1;         /* ��¼���±�־ */
//      
//      button[i].draw_btn(&button[i]);  /*�ػ水ť*/
//      }        
//      
//    }
//    else if(button[i].touch_flag == 1) /* �����Ƴ��˰����ķ�Χ��֮ǰ�а��°�ť */
//    {
//      button[0].touch_flag = 0;         /* ������±�־���ж�Ϊ�����*/
//      
//      button[0].draw_btn(&button[i]);   /*�ػ水ť*/
//    }

//  }

//}

/**
* @brief  Touch_Button_Up �������ͷ�ʱ���õĺ������ɴ���������
* @param  x ��������ͷ�ʱ��x����
* @param  y ��������ͷ�ʱ��y����
* @retval ��
*/
void Touch_Button_Up(void)
{
   uint8_t i; 
   for(i=0;i<BUTTON_NUM;i++)
   {
			if((x_data>=(chessscreenpos[1][1].x-200) && x_data<=(chessscreenpos[1][1].x+200)
				&&(y_data>=(chessscreenpos[1][1].y-100) && y_data<=(chessscreenpos[1][1].y+100))))
				{   
					button[0].btn_command(&button[0]);  /*ִ�а����Ĺ�������*/
					break;
				}	
			else if((x_data>=(chessscreenpos[1][3].x-200) && x_data<=(chessscreenpos[1][3].x+200)
				&&(y_data>=(chessscreenpos[1][3].y-100) && y_data<=(chessscreenpos[1][3].y+100))))
			{
					button[1].btn_command(&button[1]);  /*ִ�а����Ĺ�������*/
					break;
			}
    }  

}


void Welcome(void)	//��ӭ����
{
	  uint8_t i;
		LCD_ColourFillScreen(GBLUE);

	// "Computer"
	  LCD_ShowChar(80,320-260,67,24,BLACK);
	  LCD_ShowChar(91,320-260,111,24,BLACK);
	  LCD_ShowChar(102,320-260,109,24,BLACK);
	  LCD_ShowChar(113,320-260,112,24,BLACK);
	  LCD_ShowChar(124,320-260,117,24,BLACK);
	  LCD_ShowChar(135,320-260,116,24,BLACK);
	  LCD_ShowChar(146,320-260,101,24,BLACK);
	  LCD_ShowChar(157,320-260,114,24,BLACK);
		LCD_DrawChess(66,250,BLACK);
	
		// "Battle"
	  LCD_ShowChar(90,320-160,66,24,BLACK);
	  LCD_ShowChar(101,320-160,97,24,BLACK);
	  LCD_ShowChar(112,320-160,116,24,BLACK);
	  LCD_ShowChar(123,320-160,116,24,BLACK);
	  LCD_ShowChar(134,320-160,108,24,BLACK);
	  LCD_ShowChar(145,320-160,101,24,BLACK);
		LCD_DrawChess(66,150,WHITE);
		
//		// "Welcome!"
//	  LCD_ShowChar(90,320-160,66,24,BLACK);
//	  LCD_ShowChar(101,320-160,97,24,BLACK);
//	  LCD_ShowChar(112,320-160,116,24,BLACK);
//	  LCD_ShowChar(123,320-160,116,24,BLACK);
//	  LCD_ShowChar(134,320-160,108,24,BLACK);
//	  LCD_ShowChar(145,320-160,101,24,BLACK);
//		LCD_DrawChess(66,150,WHITE);
	
	  Touch_Button_Init();
	
	  /* ��水ť */
    for(i=0;i<BUTTON_NUM;i++)
    {
      button[i].draw_btn(&button[i]);
    }
		
//		Touch_Button_Up(x_data,y_data);
		
}


void Entry(void)   //������Ϸ����
{

}

void Menu(void)    //���˵�
{

}

void Show_Time(void) //��ʾ����ʱ
{

}


void Show_Result(void)  //��ʾ���
{

}

void Goto_XY(void)  //���
{

}

void Command_VS_Person(void *btn)
{
	LCD_InitCheckerBoard();
	while(1)
	{
		TouchChess();
		if(iWinner==1||iWinner==2)
			 break;
	}
	//LCD_ShowChar(66,320-80,89,24,RED);
}


void Command_VS_Computer(void *btn)
{
	LCD_InitCheckerBoard();
	while(1)
	{
		VSComputer();
		if(iWinner==1||iWinner==2)
			 break;
	}
	// LCD_ShowChar(66,320-80,89,24,RED);

}


