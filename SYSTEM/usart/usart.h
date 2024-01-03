#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK NANO STM32F4开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2019/3/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2019-2029
//All rights reserved
//********************************************************************************
//////////////////////////////////////////////////////////////////////////////////

#define USART_REC_LEN  			1024  	//定义最大接收字节数 1024
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_USART6_RX 			1		//使能（1）/禁止（0）串口6接收
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口6接收
	  	
extern u8  USART_RX_BUF6[USART_REC_LEN]; //uart6接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART_RX_BUF1[USART_REC_LEN]; //uart1接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART_RX_BUF2[USART_REC_LEN]; //uart2接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA6;         		//接收状态标记	
extern u16 USART_RX_STA1;         		//接收状态标记	
extern u16 USART_RX_STA2;         		//接收状态标记	
extern UART_HandleTypeDef UART1_Handler, UART6_Handler, UART2_Handler; //UART句柄

#define RXBUFFERSIZE   1 //缓存大小
extern u8 aRxBuffer6[RXBUFFERSIZE];//HAL库USART6接收Buffer
extern u8 aRxBuffer1[RXBUFFERSIZE];//HAL库USART1接收Buffer
extern u8 aRxBuffer2[RXBUFFERSIZE];//HAL库USART1接收Buffer
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u8 uartx, u32 bound);
#endif


