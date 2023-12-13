#include "bsp_beep.h"
#include "bsp_gpio.h"

/**
 * @brief 蜂鸣器结构体
 */
static struct  {
	uint16_t beep_flag:1;
    uint16_t beep_tim:15;
}beep_module;

/**
 * @brief 设置蜂鸣器的时间
 * @param  tim  设定时间            
 */
void SetBeepTim(uint16_t tim)
{  
    if(tim>30000)
    {//位域操作 tim只有16位,最大的数据只有32766
        tim=30000;
    }
    
	if( beep_module.beep_tim<tim)
    {
        beep_module.beep_tim=tim;
    }		
}
/**
 * @brief Get the Beep Tim object
 * @return uint16_t 当前蜂鸣器的实际
 */
uint16_t GetBeepTim(void)
{
	return  beep_module.beep_tim;
}
/**
 * @brief 蜂鸣器动作,周期性调用
 */
void BeepActive(void)
{    
    if(beep_module.beep_tim)
    {          
        if(beep_module.beep_tim<10000)
        {//间隔声音 10000ms  10s内可以间隔响
            if(beep_module.beep_tim%ONE_BEEP_TIM==0)
            {
                beep_module.beep_flag=1-beep_module.beep_flag;
            }
        }
        else if((beep_module.beep_tim>10000+TICKS_BEEP*2))
        {//连续响  此处加了2个周期的间隔时间,避免长响结束后又开始间隔响
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
