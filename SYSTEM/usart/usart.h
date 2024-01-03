#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK NANO STM32F4������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2019/3/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2019-2029
//All rights reserved
//********************************************************************************
//////////////////////////////////////////////////////////////////////////////////

#define USART_REC_LEN  			1024  	//�����������ֽ��� 1024
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART6_RX 			1		//ʹ�ܣ�1��/��ֹ��0������6����
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������6����
	  	
extern u8  USART_RX_BUF6[USART_REC_LEN]; //uart6���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART_RX_BUF1[USART_REC_LEN]; //uart1���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART_RX_BUF2[USART_REC_LEN]; //uart2���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA6;         		//����״̬���	
extern u16 USART_RX_STA1;         		//����״̬���	
extern u16 USART_RX_STA2;         		//����״̬���	
extern UART_HandleTypeDef UART1_Handler, UART6_Handler, UART2_Handler; //UART���

#define RXBUFFERSIZE   1 //�����С
extern u8 aRxBuffer6[RXBUFFERSIZE];//HAL��USART6����Buffer
extern u8 aRxBuffer1[RXBUFFERSIZE];//HAL��USART1����Buffer
extern u8 aRxBuffer2[RXBUFFERSIZE];//HAL��USART1����Buffer
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u8 uartx, u32 bound);
#endif


