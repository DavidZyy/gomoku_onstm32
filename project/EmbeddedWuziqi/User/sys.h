#ifndef __SYS_H
#define __SYS_H

#include "stm32f2xx.h"

/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
#define PLL_M      25
#define PLL_N      240
/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P      2
/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
#define PLL_Q      5
#define u16 uint16_t
#define u8 uint8_t

void SystemCKInit(void);
u16 Delay_ms(u16 count);


u8 my_strlen(const char* string);

#endif

