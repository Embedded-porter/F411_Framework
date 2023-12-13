#include "bsp_iwdg.h"

HAL_StatusTypeDef HAL_IWDG_Start(IWDG_HandleTypeDef *hiwdg)
{
	__HAL_IWDG_START(hiwdg);//开启 IWDG外设
	
	__HAL_IWDG_RELOAD_COUNTER(hiwdg);//用RLR寄存器中定义的值重新加载IWDG计数器
	
	 return HAL_OK;
}

IWDG_HandleTypeDef IWDG_Handler; //独立看门狗句柄

//初始化独立看门狗
//prer:分频数:IWDG_PRESCALER_4~IWDG_PRESCALER_256
//rlr:自动重装载值,0~0XFFF.
//时间计算(大概):Tout=((4*2^prer)*rlr)/32 (ms).
void bsp_IwdgInit(u8 prer,u16 rlr)
{
    IWDG_Handler.Instance=IWDG;
    IWDG_Handler.Init.Prescaler=prer;	//设置IWDG分频系数
    IWDG_Handler.Init.Reload=rlr;		//重装载值
    HAL_IWDG_Init(&IWDG_Handler);		//初始化IWDG,默认会开启独立看门狗	

	HAL_IWDG_Start(&IWDG_Handler);		//启动独立看门狗
}
   
//喂独立看门狗
void bsp_IwdgFeed(void)
{   
    HAL_IWDG_Refresh(&IWDG_Handler); 	//喂狗
}