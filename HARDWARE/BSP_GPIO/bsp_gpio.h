#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H

#include "sys.h"
/*LED 1*/
#define G_LED1_Pin GPIO_PIN_5
#define G_LED1_GPIO_Port GPIOC
/*LED 2*/
#define G_LED2_Pin GPIO_PIN_6
#define G_LED2_GPIO_Port GPIOC
/*LED 3*/
#define G_LED3_Pin GPIO_PIN_7
#define G_LED3_GPIO_Port GPIOC
/*kEY 1*/
#define G_KEY1_Pin GPIO_PIN_8
#define G_KEY1_GPIO_Port GPIOC
/*kEY 2*/
#define G_KEY2_Pin GPIO_PIN_9
#define G_KEY2_GPIO_Port GPIOC
/*kEY 3*/
#define G_KEY3_Pin GPIO_PIN_0
#define G_KEY3_GPIO_Port GPIOA
/*kEY 4*/
#define G_KEY4_Pin GPIO_PIN_2
#define G_KEY4_GPIO_Port GPIOD
/*BEEP*/
#define G_BEEP_Pin GPIO_PIN_8
#define G_BEEP_GPIO_Port GPIOB

typedef enum
{
	LED1 = 0,
	LED2,
	LED3,
	KEY1,
	KEY2,
	KEY3,
	KEY4,
	BEEP,
} IO_TypeDef;
uint8_t GPIO_R(IO_TypeDef IO); // 读取IO口
void BSP_IO_H(IO_TypeDef io);  // 输出高电平
void BSP_IO_L(IO_TypeDef io);  // 输出低电平
void BSP_IO_T(IO_TypeDef io);  // IO口翻转
void bsp_GPIO_Init(void);       // GPIO初始化
#endif
