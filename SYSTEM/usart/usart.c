#include "usart.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
// ���ʹ��os,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h" //os ʹ��
#endif

#if 1
#pragma import(__use_no_semihosting)
// ��׼����Ҫ��֧�ֺ���
struct __FILE
{
	int handle;
};

FILE __stdout;
// ����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}
// �ض���fputc����
int fputc(int ch, FILE *f)
{
	while ((USART1->SR & 0X40) == 0)
		; // ѭ������,ֱ���������
	USART1->DR = (u8)ch;
	return ch;
}

#endif

#if EN_USART1_RX // ���ʹ���˽���
// ����1�жϷ������
// ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
u8 USART_RX_BUF6[USART_REC_LEN]; // uart6���ջ���,���USART_REC_LEN���ֽ�.
u8 USART_RX_BUF1[USART_REC_LEN]; // uart1���ջ���,���USART_REC_LEN���ֽ�.
u8 USART_RX_BUF2[USART_REC_LEN]; // uart2���ջ���,���USART_REC_LEN���ֽ�.
// ����״̬
// bit15��	������ɱ�־
// bit14��	���յ�0x0d
// bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA6 = 0; // ����״̬���	uart6��
u16 USART_RX_STA1 = 0; // ����״̬���	uart1��
u16 USART_RX_STA2 = 0; // ����״̬���	uart2��

u8 aRxBuffer6[RXBUFFERSIZE];									// HAL��ʹ�õĴ��ڽ��ջ��� uart6
u8 aRxBuffer1[RXBUFFERSIZE];									// HAL��ʹ�õĴ��ڽ��ջ��� uart1
u8 aRxBuffer2[RXBUFFERSIZE];									// HAL��ʹ�õĴ��ڽ��ջ��� uart1
UART_HandleTypeDef UART1_Handler, UART6_Handler, UART2_Handler; // UART���

// ��ʼ��IO ����1
// bound:������
void uart_init(u8 uartx, u32 bound)
{
	// UART ��ʼ������
	if (uartx == 6)
	{
		UART6_Handler.Instance = USART6;					// USART6
		UART6_Handler.Init.BaudRate = bound;				// ������
		UART6_Handler.Init.WordLength = UART_WORDLENGTH_8B; // �ֳ�Ϊ8λ���ݸ�ʽ
		UART6_Handler.Init.StopBits = UART_STOPBITS_1;		// һ��ֹͣλ
		UART6_Handler.Init.Parity = UART_PARITY_NONE;		// ����żУ��λ
		UART6_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; // ��Ӳ������
		UART6_Handler.Init.Mode = UART_MODE_TX_RX;			// �շ�ģʽ
		HAL_UART_Init(&UART6_Handler);						// HAL_UART_Init()��ʹ��UART6

		HAL_UART_Receive_IT(&UART6_Handler, (u8 *)aRxBuffer6, RXBUFFERSIZE); // �ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
	}
	else if (uartx == 1)
	{
		UART1_Handler.Instance = USART1;					// USART1
		UART1_Handler.Init.BaudRate = bound;				// ������
		UART1_Handler.Init.WordLength = UART_WORDLENGTH_8B; // �ֳ�Ϊ8λ���ݸ�ʽ
		UART1_Handler.Init.StopBits = UART_STOPBITS_1;		// һ��ֹͣλ
		UART1_Handler.Init.Parity = UART_PARITY_NONE;		// ����żУ��λ
		UART1_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; // ��Ӳ������
		UART1_Handler.Init.Mode = UART_MODE_TX_RX;			// �շ�ģʽ
		HAL_UART_Init(&UART1_Handler);						// HAL_UART_Init()��ʹ��UART1

		HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer1, RXBUFFERSIZE); // �ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
	}
	else if (uartx == 2)
	{
		UART2_Handler.Instance = USART2;					// USART2
		UART2_Handler.Init.BaudRate = bound;				// ������
		UART2_Handler.Init.WordLength = UART_WORDLENGTH_8B; // �ֳ�Ϊ8λ���ݸ�ʽ
		UART2_Handler.Init.StopBits = UART_STOPBITS_1;		// һ��ֹͣλ
		UART2_Handler.Init.Parity = UART_PARITY_NONE;		// ����żУ��λ
		UART2_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; // ��Ӳ������
		UART2_Handler.Init.Mode = UART_MODE_TX_RX;			// �շ�ģʽ
		HAL_UART_Init(&UART2_Handler);						// HAL_UART_Init()��ʹ��UART1

		HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE); // �ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
	}
}

// UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
// �˺����ᱻHAL_UART_Init()����
// huart:���ھ��

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	// GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;

	if (huart->Instance == USART6) // ����Ǵ���6�����д���6 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();  // ʹ��GPIOAʱ��
		__HAL_RCC_USART6_CLK_ENABLE(); // ʹ��USART6ʱ��

		GPIO_Initure.Pin = GPIO_PIN_11;			  // PA11
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;	  // �����������
		GPIO_Initure.Pull = GPIO_PULLUP;		  // ����
		GPIO_Initure.Speed = GPIO_SPEED_FAST;	  // ����
		GPIO_Initure.Alternate = GPIO_AF8_USART6; // ����ΪUSART6
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);	  // ��ʼ��PA11

		GPIO_Initure.Pin = GPIO_PIN_12;		 // PA12
		HAL_GPIO_Init(GPIOA, &GPIO_Initure); // ��ʼ��PA12

#if EN_USART1_RX
		HAL_NVIC_EnableIRQ(USART6_IRQn);		 // ʹ��USART6�ж�ͨ��
		HAL_NVIC_SetPriority(USART6_IRQn, 3, 3); // ��ռ���ȼ�3�������ȼ�3
#endif
	}

	if (huart->Instance == USART1) // ����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();  // ʹ��GPIOBʱ��
		__HAL_RCC_USART1_CLK_ENABLE(); // ʹ��USART1ʱ��

		GPIO_Initure.Pin = GPIO_PIN_9;			  // PA9
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;	  // �����������
		GPIO_Initure.Pull = GPIO_PULLUP;		  // ����
		GPIO_Initure.Speed = GPIO_SPEED_FAST;	  // ����
		GPIO_Initure.Alternate = GPIO_AF7_USART1; // ����ΪUSART1
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);	  // ��ʼ��PA9

		GPIO_Initure.Pin = GPIO_PIN_10;		 // PA10
		HAL_GPIO_Init(GPIOA, &GPIO_Initure); // ��ʼ��PA10

#if EN_USART1_RX
		HAL_NVIC_EnableIRQ(USART1_IRQn);		 // ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn, 3, 2); // ��ռ���ȼ�3�������ȼ�3
#endif
	}

	if (huart->Instance == USART2) // ����Ǵ���2�����д���2 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();  // ʹ��GPIOAʱ��
		__HAL_RCC_USART2_CLK_ENABLE(); // ʹ��USART1ʱ��

		GPIO_Initure.Pin = GPIO_PIN_2;			  // PA2
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;	  // �����������
		GPIO_Initure.Pull = GPIO_PULLUP;		  // ����
		GPIO_Initure.Speed = GPIO_SPEED_FAST;	  // ����
		GPIO_Initure.Alternate = GPIO_AF7_USART2; // ����ΪUSART2
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);	  // ��ʼ��PA2

		GPIO_Initure.Pin = GPIO_PIN_3;		 // PA3
		HAL_GPIO_Init(GPIOA, &GPIO_Initure); // ��ʼ��PA3

#if EN_USART1_RX
		HAL_NVIC_EnableIRQ(USART2_IRQn);		 // ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART2_IRQn, 3, 1); // ��ռ���ȼ�3�������ȼ�3
#endif
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if (huart->Instance == USART6) // ����Ǵ���6
	{
		if ((USART_RX_STA6 & 0x8000) == 0) // ����δ���
		{

			if (USART_RX_STA6 & 0x4000) // ���յ���0x0d
			{
				if (aRxBuffer6[0] != 0x0a)
					USART_RX_STA6 = 0; // ���մ���,���¿�ʼ
				else
					USART_RX_STA6 |= 0x8000; // ���������
			}
			else // ��û�յ�0X0D
			{
				if (aRxBuffer6[0] == 0x0d)
					USART_RX_STA6 |= 0x4000;
				else
				{
					USART_RX_BUF6[USART_RX_STA6 & 0X3FFF] = aRxBuffer6[0];
					USART_RX_STA6++;
					/* ������� */
					if (USART_RX_STA6 > (USART_REC_LEN - 1))
						USART_RX_STA6 = 0; // �������ݴ���,���¿�ʼ����
				}
			}
		}
	}
	else if (huart->Instance == USART1) // ����Ǵ���1
	{
		// ���յ�֡ͷ
		if (aRxBuffer1[0] == '<')
		{
			USART_RX_STA1 = 0x4000;  //���ñ�־λ
		}
		// �洢����
		if ((USART_RX_STA1 & 0x8000) == 0) // ����δ���
		{
			if (USART_RX_STA1 & 0x4000) // ���յ��� '<'
			{
				// ���յ��� '>'���������
				if (aRxBuffer1[0] == '>')
				{
					USART_RX_STA1 |= 0x8000;   // ��������˳�
					USART_RX_BUF1[USART_RX_STA1 & 0X3FFF] = aRxBuffer1[0];
					USART_RX_STA1++;
				}
				// �洢����
				else
				{
					USART_RX_BUF1[USART_RX_STA1 & 0X3FFF] = aRxBuffer1[0];
					USART_RX_STA1++;
					/* ������� */
					if ((USART_RX_STA1 & 0X3FFF) > (USART_REC_LEN - 1))
						USART_RX_STA1 = 0; // �������ݴ���,���¿�ʼ����
				}
			}
		}
	}
	else if (huart->Instance == USART2) // ����Ǵ���2
	{
		if ((USART_RX_STA2 & 0x8000) == 0) // ����δ���
		{
			if (USART_RX_STA2 & 0x4000) // ���յ���0x0d
			{
				if (aRxBuffer2[0] != 0x0a)
					USART_RX_STA2 = 0; // ���մ���,���¿�ʼ
				else
					USART_RX_STA2 |= 0x8000; // ���������
			}
			else // ��û�յ�0X0D
			{
				if (aRxBuffer2[0] == 0x0d)
					USART_RX_STA2 |= 0x4000;
				else
				{
					USART_RX_BUF2[USART_RX_STA2 & 0X3FFF] = aRxBuffer2[0];
					USART_RX_STA2++;
					/* ������� */
					if (USART_RX_STA2 > (USART_REC_LEN - 1))
						USART_RX_STA2 = 0; // �������ݴ���,���¿�ʼ����
				}
			}
		}
	}
}

// ����1�жϷ������
void USART1_IRQHandler(void)
{
	u32 timeout = 0;
#if SYSTEM_SUPPORT_OS // ʹ��OS
	OSIntEnter();
#endif

	HAL_UART_IRQHandler(&UART1_Handler); // ����HAL���жϴ����ú���

	timeout = 0;
	while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY) // �ȴ�����
	{
		timeout++; ////��ʱ����
		if (timeout > HAL_MAX_DELAY)
			break;
	}
	timeout = 0;
	while (HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer1, RXBUFFERSIZE) != HAL_OK) // һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
		timeout++; // ��ʱ����
		if (timeout > HAL_MAX_DELAY)
			break;
	}
#if SYSTEM_SUPPORT_OS // ʹ��OS
	OSIntExit();
#endif
}

// ����2�жϷ������
void USART2_IRQHandler(void)
{
	u32 timeout = 0;
#if SYSTEM_SUPPORT_OS // ʹ��OS
	OSIntEnter();
#endif

	HAL_UART_IRQHandler(&UART2_Handler); // ����HAL���жϴ����ú���

	timeout = 0;
	while (HAL_UART_GetState(&UART2_Handler) != HAL_UART_STATE_READY) // �ȴ�����
	{
		timeout++; ////��ʱ����
		if (timeout > HAL_MAX_DELAY)
			break;
	}

	timeout = 0;
	while (HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE) != HAL_OK) // һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
		timeout++; // ��ʱ����
		if (timeout > HAL_MAX_DELAY)
			break;
	}
#if SYSTEM_SUPPORT_OS // ʹ��OS
	OSIntExit();
#endif
}

// ����6�жϷ������
void USART6_IRQHandler(void)
{
	u32 timeout = 0;
#if SYSTEM_SUPPORT_OS // ʹ��OS
	OSIntEnter();
#endif

	HAL_UART_IRQHandler(&UART6_Handler); // ����HAL���жϴ����ú���

	timeout = 0;
	while (HAL_UART_GetState(&UART6_Handler) != HAL_UART_STATE_READY) // �ȴ�����
	{
		timeout++; ////��ʱ����
		if (timeout > HAL_MAX_DELAY)
			break;
	}

	timeout = 0;
	while (HAL_UART_Receive_IT(&UART6_Handler, (u8 *)aRxBuffer6, RXBUFFERSIZE) != HAL_OK) // һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
		timeout++; // ��ʱ����
		if (timeout > HAL_MAX_DELAY)
			break;
	}
#if SYSTEM_SUPPORT_OS // ʹ��OS
	OSIntExit();
#endif
}
#endif
