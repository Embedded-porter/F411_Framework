#ifndef _BSP_BEEP_H
#define _BSP_BEEP_H

#include "sys.h"

#define TICKS_BEEP         (20)	//ms
#define ONE_BEEP_TIM       (140) //ms  必须是 TICKS_BEEP 整数倍

//间隔声音响  x->响的次数
#define BEEP_DIMMDIMM(x) SetBeepTim((x*2-1)*ONE_BEEP_TIM)
//持续响 x->持续响多少ms
#define BEEP_DIDIDIDI(x) SetBeepTim(x+10000+TICKS_BEEP*2) 

void SetBeepTim(uint16_t tim);
uint16_t GetBeepTim(void);
void BeepActive(void);

#endif
