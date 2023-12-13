#include "bsp_key.h"
#include "bsp_button.h"
#include "bsp_gpio.h"
#include "bsp_config.h"

static Button btn1; // 定义按键1
static Button btn2; // 定义按键2
static Button btn3; // 定义按键3
static Button btn4; // 定义按键4

// 按键驱动
static uint8_t Read_Button1_GPIO(void)
{
	return !GPIO_R(KEY1);
}

static uint8_t Read_Button2_GPIO(void)
{
	return !GPIO_R(KEY2);
}

static uint8_t Read_Button3_GPIO(void)
{
	return !GPIO_R(KEY3);
}
static uint8_t Read_Button4_GPIO(void)
{
	return !GPIO_R(KEY4);
}

static void BTN1_PRESS_DOWN_Handler(void *btn, uint8_t event)
{
	if (event & PRESS_DOWN)
	{
		/*添加处理函数*/
		DBG_PRINTF("按键1被按下\r\n");
	}
}

static void BTN2_PRESS_DOWN_Handler(void *btn, uint8_t event)
{
	if (event & PRESS_DOWN)
	{
		/*添加处理函数*/
		DBG_PRINTF("按键2被按下\r\n");
	}
}
static void BTN3_PRESS_DOWN_Handler(void *btn, uint8_t event)
{
	if (event & PRESS_DOWN)
	{
		/*添加处理函数*/
		DBG_PRINTF("按键3被按下\r\n");
	}
}
static void BTN4_PRESS_DOWN_Handler(void *btn, uint8_t event)
{
	if (event & PRESS_DOWN)
	{
		/*添加处理函数*/
		DBG_PRINTF("按键4被按下\r\n");
	}
}
void bsp_KeyInit(void)
{
	button_init();
	/* 注册按键 */
	button_attach(&btn1, Read_Button1_GPIO, BTN1_PRESS_DOWN_Handler, PRESS_DOWN);

	button_attach(&btn2, Read_Button2_GPIO, BTN2_PRESS_DOWN_Handler, PRESS_DOWN);

	button_attach(&btn3, Read_Button3_GPIO, BTN3_PRESS_DOWN_Handler, PRESS_DOWN);

	button_attach(&btn4, Read_Button4_GPIO, BTN4_PRESS_DOWN_Handler, PRESS_DOWN);
}
