#ifndef __TOUCH_H
#define __TOUCH_H
#include "stm32f2xx.h"

#define u32 uint32_t 

void TouchScreenInit(void);
u32 SPI_Y(void);
u32 SPI_X(void);

#endif

