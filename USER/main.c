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
#include "bsp_command.h"

int main(void)
{
    int i = 0;
    uint8_t count;
    g_spiLen = 20;                        /* SPI传输长度 */
    HAL_Init();                           /* 初始化HAL库 */
    HAL_DeInit();                         /* 初始化延时函数 */
    SystemClock_Config();                 /* 设置时钟,100Mhz */
    uart_init(1, 115200);                 /* 串口初始化 */
    bsp_AdcInit();                        /* ADC初始化 */
    bsp_GPIO_Init();                      /* LED和按键IO初始化 */
    bsp_InitSPIBus();                     /* 配置SPI总线 */
    bsp_InitTimer();                      /* 初始化滴答定时器 */
    bsp_KeyInit();                        /* 按键初始化 */
    bsp_IwdgInit(IWDG_PRESCALER_64, 625); /* 分频数为64,重载值为625,溢出时间为1s */
    printf("OLP System OK\r\n");
    while (1) {
		ReceivePolling(); /* 串口指令轮询接收 */
        bsp_Idle(); /* 空闲任务 喂狗 */
        BSP_IO_T(LED1);
        BSP_IO_T(LED2);
        BSP_IO_T(LED3);
        for (i = 0; i < 20; i++) {
            g_spiTxBuf[i] = count++;
            bsp_spiTransfer();
        }
        count = 0;
        bsp_DelayMS(100);
    }
}