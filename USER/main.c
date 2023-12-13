#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "bsp_config.h"
#include "bsp_adc.h"
#include "bsp_spi.h"
#include "bsp_timer.h"
#include "bsp_gpio.h"
#include "bsp_key.h"
#include "bsp_iwdg.h"
#include "bsp_beep.h"

int main(void)
{
	int i = 0;
	uint8_t count;
	g_spiLen = 20;						  /*SPI���䳤��*/
	HAL_Init();							  /*��ʼ��HAL�� */
	HAL_DeInit();						  /*��ʼ����ʱ����*/
	SystemClock_Config();				  /*����ʱ��,100Mhz */
	uart_init(1, 115200);				  /*��ʼ������1,������115200 */
	bsp_AdcInit();						  /*ADC��ʼ��*/
	bsp_GPIO_Init();					  /*LED�Ͱ���IO��ʼ��*/
	bsp_InitSPIBus();					  /*����SPI����*/
	bsp_InitTimer();					  /*��ʼ���δ�ʱ��*/
	bsp_KeyInit();						  /*������ʼ��*/
	bsp_IwdgInit(IWDG_PRESCALER_64, 625); /*��Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ1s*/
	while (1)
	{
		bsp_Idle(); /*�������� ι��*/
		 BSP_IO_H(LED1);
		 BSP_IO_H(LED2);
		 BSP_IO_H(LED3);
		for (i = 0; i < 20; i++)
		{
			g_spiTxBuf[i] = count++;
			bsp_spiTransfer();
		}
		count = 0;
		bsp_DelayMS(100);
	}
}
