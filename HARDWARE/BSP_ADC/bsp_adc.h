#ifndef _BSP_ADC_H
#define _BSP_ADC_H
#include "sys.h"

extern ADC_HandleTypeDef ADC1_Handler;//ADC句柄

void bsp_AdcInit(void);				//ADC通道初始化
u16 Get_Adc_Average(u32 ch,u8 times);//得到某个通道给定次数采样的平均值
#endif /*_BSP_ADC_H*/
