#include "bsp_beep.h"
#include "bsp_gpio.h"

/**
 * @brief �������ṹ��
 */
static struct  {
	uint16_t beep_flag:1;
    uint16_t beep_tim:15;
}beep_module;

/**
 * @brief ���÷�������ʱ��
 * @param  tim  �趨ʱ��            
 */
void SetBeepTim(uint16_t tim)
{  
    if(tim>30000)
    {//λ����� timֻ��16λ,��������ֻ��32766
        tim=30000;
    }
    
	if( beep_module.beep_tim<tim)
    {
        beep_module.beep_tim=tim;
    }		
}
/**
 * @brief Get the Beep Tim object
 * @return uint16_t ��ǰ��������ʵ��
 */
uint16_t GetBeepTim(void)
{
	return  beep_module.beep_tim;
}
/**
 * @brief ����������,�����Ե���
 */
void BeepActive(void)
{    
    if(beep_module.beep_tim)
    {          
        if(beep_module.beep_tim<10000)
        {//������� 10000ms  10s�ڿ��Լ����
            if(beep_module.beep_tim%ONE_BEEP_TIM==0)
            {
                beep_module.beep_flag=1-beep_module.beep_flag;
            }
        }
        else if((beep_module.beep_tim>10000+TICKS_BEEP*2))
        {//������  �˴�����2�����ڵļ��ʱ��,���ⳤ��������ֿ�ʼ�����
            beep_module.beep_flag=1;
        }
        else
        {
            beep_module.beep_tim=0;
            beep_module.beep_flag=0;
        }
        
        beep_module.beep_tim=beep_module.beep_tim>TICKS_BEEP?beep_module.beep_tim-TICKS_BEEP:0;
    }
    else
    {
        beep_module.beep_tim=0;
		beep_module.beep_flag=0;
    }

    if(beep_module.beep_flag==1)
    {
        BSP_IO_H(BEEP);
    }
    else
    {
        BSP_IO_L(BEEP);
    }       
}
