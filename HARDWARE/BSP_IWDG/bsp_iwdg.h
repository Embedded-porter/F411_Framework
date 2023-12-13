#ifndef _BSP_IDWG_H
#define _BSP_IDWG_H

#include "sys.h"

void bsp_IwdgInit(u8 prer,u16 rlr);//初始化IWDG，并使能IWDG
void bsp_IwdgFeed(void);//喂狗

#endif // !_BSP_IDWG_H


