#ifndef _BSP_ADC_H
#define _BSP_ADC_H
#include "sys.h"

extern ADC_HandleTypeDef ADC1_Handler;//ADC���

void bsp_AdcInit(void);				//ADCͨ����ʼ��
u16 Get_Adc_Average(u32 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ
#endif /*_BSP_ADC_H*/
