#ifndef _BSP_BEEP_H
#define _BSP_BEEP_H

#include "sys.h"

#define TICKS_BEEP         (20)	//ms
#define ONE_BEEP_TIM       (140) //ms  ������ TICKS_BEEP ������

//���������  x->��Ĵ���
#define BEEP_DIMMDIMM(x) SetBeepTim((x*2-1)*ONE_BEEP_TIM)
//������ x->���������ms
#define BEEP_DIDIDIDI(x) SetBeepTim(x+10000+TICKS_BEEP*2) 

void SetBeepTim(uint16_t tim);
uint16_t GetBeepTim(void);
void BeepActive(void);

#endif
