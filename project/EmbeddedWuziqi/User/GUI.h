#ifndef __GUI_H
#define __GUI_H

#include "stm32f2xx.h"
#include "lcd.h"

#define COLOR_BLOCK_WIDTH   140
#define COLOR_BLOCK_HEIGHT  50

#define BUTTON_NUM 2
#define WINDOWS_START_Y   0
#define WINDOWS_END_Y     LCD_Y_LENGTH

//��ť�������
#define BUTTON_START_X      50
#define PALETTE_START_X   (COLOR_BLOCK_WIDTH*2+1)
#define PALETTE_END_X     LCD_X_LENGTH

/*
	LCD ��ɫ���룬CL_��Color�ļ�д
	16Bit�ɸ�λ����λ�� RRRR RGGG GGGB BBBB

	�����RGB �꽫24λ��RGBֵת��Ϊ16λ��ʽ��
	����windows�Ļ��ʳ��򣬵���༭��ɫ��ѡ���Զ�����ɫ�����Ի�õ�RGBֵ��

	�Ƽ�ʹ������ȡɫ����������㿴���Ľ�����ɫ��
*/
#if LCD_RGB_888
/*RGB888��ɫת��*/
#define RGB(R,G,B)	( (R<< 16) | (G << 8) | (B))	/* ��8λR,G,Bת��Ϊ 24λRGB888��ʽ */

#else 
/*RGB565 ��ɫת��*/
#define RGB(R,G,B)	(((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3))	/* ��8λR,G,Bת��Ϊ 16λRGB565��ʽ */
#define RGB565_R(x)  ((x >> 8) & 0xF8)
#define RGB565_G(x)  ((x >> 3) & 0xFC)
#define RGB565_B(x)  ((x << 3) & 0xF8)

#endif


enum
{
	CL_WHITE    = RGB(255,255,255),	/* ��ɫ */
	CL_BLACK    = RGB(  0,  0,  0),	/* ��ɫ */
	CL_RED      = RGB(255,	0,  0),	/* ��ɫ */
	CL_GREEN    = RGB(  0,255,  0),	/* ��ɫ */
	CL_BLUE     = RGB(  0,	0,255),	/* ��ɫ */
	CL_YELLOW   = RGB(255,255,  0),	/* ��ɫ */

	CL_GREY    = RGB( 98, 98, 98), 	/* ���ɫ */
	CL_GREY1		= RGB( 150, 150, 150), 	/* ǳ��ɫ */
	CL_GREY2		= RGB( 180, 180, 180), 	/* ǳ��ɫ */
	CL_GREY3		= RGB( 200, 200, 200), 	/* ��ǳ��ɫ */
	CL_GREY4		= RGB( 230, 230, 230), 	/* ��ǳ��ɫ */

	CL_BUTTON_GREY	= RGB( 195, 195, 195), /* WINDOWS ��ť�����ɫ */

	CL_MAGENTA      = RGB(255, 0, 255),	/* ����ɫ�����ɫ */
	CL_CYAN         = RGB( 0, 255, 255),	/* ����ɫ����ɫ */

	CL_BLUE1        = RGB(  0,  0, 240),		/* ����ɫ */
	CL_BLUE2        = RGB(  0,  0, 128),		/* ����ɫ */
	CL_BLUE3        = RGB(  68, 68, 255),		/* ǳ��ɫ1 */
	CL_BLUE4        = RGB(  0, 64, 128),		/* ǳ��ɫ1 */

	/* UI ���� Windows�ؼ�����ɫ */
	CL_BTN_FACE		  = RGB(236, 233, 216),	/* ��ť������ɫ(��) */
	CL_BOX_BORDER1	= RGB(172, 168,153),	/* �����������ɫ */
	CL_BOX_BORDER2	= RGB(255, 255,255),	/* �������Ӱ����ɫ */

	CL_MASK			    = 0x7FFF	/* RGB565��ɫ���룬�������ֱ���͸�� */
};


typedef struct 
{
  uint16_t start_x;   //������x��ʼ����  
  uint16_t start_y;   //������y��ʼ����
  uint16_t end_x;     //������x�������� 
  uint16_t end_y;     //������y��������
  uint8_t touch_flag; //�������µı�־
    
  void (*draw_btn)(void * btn);     //������溯��
  void (*btn_command)(void * btn);  //��������ִ�к���
 
}Touch_Button;


  
void Palette_Init(uint8_t LCD_Mode);
void Touch_Button_Init(void);
void Touch_Button_Down(uint16_t x,uint16_t y);
void Touch_Button_Up(void);

/****************************************/

void Welcome(void);	//��ӭ����
void Entry(void);   //������Ϸ����
void Menu(void);    //���˵�
void Show_Time(void); //��ʾ����ʱ
void Show_Result(void);  //��ʾ���
void Goto_XY(void);  //���

#endif
