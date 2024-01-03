#include "usart.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
// 如果使用os,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h" //os 使用
#endif

#if 1
#pragma import(__use_no_semihosting)
// 标准库需要的支持函数
struct __FILE
{
	int handle;
};

FILE __stdout;
// 定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
// 重定义fputc函数
int fputc(int ch, FILE *f)
{
	while ((USART1->SR & 0X40) == 0)
		; // 循环发送,直到发送完毕
	USART1->DR = (u8)ch;
	return ch;
}

#endif

#if EN_USART1_RX // 如果使能了接收
// 串口1中断服务程序
// 注意,读取USARTx->SR能避免莫名其妙的错误
u8 USART_RX_BUF6[USART_REC_LEN]; // uart6接收缓冲,最大USART_REC_LEN个字节.
u8 USART_RX_BUF1[USART_REC_LEN]; // uart1接收缓冲,最大USART_REC_LEN个字节.
u8 USART_RX_BUF2[USART_REC_LEN]; // uart2接收缓冲,最大USART_REC_LEN个字节.
// 接收状态
// bit15，	接收完成标志
// bit14，	接收到0x0d
// bit13~0，	接收到的有效字节数目
u16 USART_RX_STA6 = 0; // 接收状态标记	uart6用
u16 USART_RX_STA1 = 0; // 接收状态标记	uart1用
u16 USART_RX_STA2 = 0; // 接收状态标记	uart2用

u8 aRxBuffer6[RXBUFFERSIZE];									// HAL库使用的串口接收缓冲 uart6
u8 aRxBuffer1[RXBUFFERSIZE];									// HAL库使用的串口接收缓冲 uart1
u8 aRxBuffer2[RXBUFFERSIZE];									// HAL库使用的串口接收缓冲 uart1
UART_HandleTypeDef UART1_Handler, UART6_Handler, UART2_Handler; // UART句柄

// 初始化IO 串口1
// bound:波特率
void uart_init(u8 uartx, u32 bound)
{
	// UART 初始化设置
	if (uartx == 6)
	{
		UART6_Handler.Instance = USART6;					// USART6
		UART6_Handler.Init.BaudRate = bound;				// 波特率
		UART6_Handler.Init.WordLength = UART_WORDLENGTH_8B; // 字长为8位数据格式
		UART6_Handler.Init.StopBits = UART_STOPBITS_1;		// 一个停止位
		UART6_Handler.Init.Parity = UART_PARITY_NONE;		// 无奇偶校验位
		UART6_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; // 无硬件流控
		UART6_Handler.Init.Mode = UART_MODE_TX_RX;			// 收发模式
		HAL_UART_Init(&UART6_Handler);						// HAL_UART_Init()会使能UART6

		HAL_UART_Receive_IT(&UART6_Handler, (u8 *)aRxBuffer6, RXBUFFERSIZE); // 该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
	}
	else if (uartx == 1)
	{
		UART1_Handler.Instance = USART1;					// USART1
		UART1_Handler.Init.BaudRate = bound;				// 波特率
		UART1_Handler.Init.WordLength = UART_WORDLENGTH_8B; // 字长为8位数据格式
		UART1_Handler.Init.StopBits = UART_STOPBITS_1;		// 一个停止位
		UART1_Handler.Init.Parity = UART_PARITY_NONE;		// 无奇偶校验位
		UART1_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; // 无硬件流控
		UART1_Handler.Init.Mode = UART_MODE_TX_RX;			// 收发模式
		HAL_UART_Init(&UART1_Handler);						// HAL_UART_Init()会使能UART1

		HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer1, RXBUFFERSIZE); // 该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
	}
	else if (uartx == 2)
	{
		UART2_Handler.Instance = USART2;					// USART2
		UART2_Handler.Init.BaudRate = bound;				// 波特率
		UART2_Handler.Init.WordLength = UART_WORDLENGTH_8B; // 字长为8位数据格式
		UART2_Handler.Init.StopBits = UART_STOPBITS_1;		// 一个停止位
		UART2_Handler.Init.Parity = UART_PARITY_NONE;		// 无奇偶校验位
		UART2_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; // 无硬件流控
		UART2_Handler.Init.Mode = UART_MODE_TX_RX;			// 收发模式
		HAL_UART_Init(&UART2_Handler);						// HAL_UART_Init()会使能UART1

		HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE); // 该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
	}
}

// UART底层初始化，时钟使能，引脚配置，中断配置
// 此函数会被HAL_UART_Init()调用
// huart:串口句柄

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	// GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;

	if (huart->Instance == USART6) // 如果是串口6，进行串口6 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();  // 使能GPIOA时钟
		__HAL_RCC_USART6_CLK_ENABLE(); // 使能USART6时钟

		GPIO_Initure.Pin = GPIO_PIN_11;			  // PA11
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;	  // 复用推挽输出
		GPIO_Initure.Pull = GPIO_PULLUP;		  // 上拉
		GPIO_Initure.Speed = GPIO_SPEED_FAST;	  // 高速
		GPIO_Initure.Alternate = GPIO_AF8_USART6; // 复用为USART6
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);	  // 初始化PA11

		GPIO_Initure.Pin = GPIO_PIN_12;		 // PA12
		HAL_GPIO_Init(GPIOA, &GPIO_Initure); // 初始化PA12

#if EN_USART1_RX
		HAL_NVIC_EnableIRQ(USART6_IRQn);		 // 使能USART6中断通道
		HAL_NVIC_SetPriority(USART6_IRQn, 3, 3); // 抢占优先级3，子优先级3
#endif
	}

	if (huart->Instance == USART1) // 如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();  // 使能GPIOB时钟
		__HAL_RCC_USART1_CLK_ENABLE(); // 使能USART1时钟

		GPIO_Initure.Pin = GPIO_PIN_9;			  // PA9
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;	  // 复用推挽输出
		GPIO_Initure.Pull = GPIO_PULLUP;		  // 上拉
		GPIO_Initure.Speed = GPIO_SPEED_FAST;	  // 高速
		GPIO_Initure.Alternate = GPIO_AF7_USART1; // 复用为USART1
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);	  // 初始化PA9

		GPIO_Initure.Pin = GPIO_PIN_10;		 // PA10
		HAL_GPIO_Init(GPIOA, &GPIO_Initure); // 初始化PA10

#if EN_USART1_RX
		HAL_NVIC_EnableIRQ(USART1_IRQn);		 // 使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn, 3, 2); // 抢占优先级3，子优先级3
#endif
	}

	if (huart->Instance == USART2) // 如果是串口2，进行串口2 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();  // 使能GPIOA时钟
		__HAL_RCC_USART2_CLK_ENABLE(); // 使能USART1时钟

		GPIO_Initure.Pin = GPIO_PIN_2;			  // PA2
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;	  // 复用推挽输出
		GPIO_Initure.Pull = GPIO_PULLUP;		  // 上拉
		GPIO_Initure.Speed = GPIO_SPEED_FAST;	  // 高速
		GPIO_Initure.Alternate = GPIO_AF7_USART2; // 复用为USART2
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);	  // 初始化PA2

		GPIO_Initure.Pin = GPIO_PIN_3;		 // PA3
		HAL_GPIO_Init(GPIOA, &GPIO_Initure); // 初始化PA3

#if EN_USART1_RX
		HAL_NVIC_EnableIRQ(USART2_IRQn);		 // 使能USART1中断通道
		HAL_NVIC_SetPriority(USART2_IRQn, 3, 1); // 抢占优先级3，子优先级3
#endif
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if (huart->Instance == USART6) // 如果是串口6
	{
		if ((USART_RX_STA6 & 0x8000) == 0) // 接收未完成
		{

			if (USART_RX_STA6 & 0x4000) // 接收到了0x0d
			{
				if (aRxBuffer6[0] != 0x0a)
					USART_RX_STA6 = 0; // 接收错误,重新开始
				else
					USART_RX_STA6 |= 0x8000; // 接收完成了
			}
			else // 还没收到0X0D
			{
				if (aRxBuffer6[0] == 0x0d)
					USART_RX_STA6 |= 0x4000;
				else
				{
					USART_RX_BUF6[USART_RX_STA6 & 0X3FFF] = aRxBuffer6[0];
					USART_RX_STA6++;
					/* 数据溢出 */
					if (USART_RX_STA6 > (USART_REC_LEN - 1))
						USART_RX_STA6 = 0; // 接收数据错误,重新开始接收
				}
			}
		}
	}
	else if (huart->Instance == USART1) // 如果是串口1
	{
		// 接收到帧头
		if (aRxBuffer1[0] == '<')
		{
			USART_RX_STA1 = 0x4000;  //设置标志位
		}
		// 存储数据
		if ((USART_RX_STA1 & 0x8000) == 0) // 接收未完成
		{
			if (USART_RX_STA1 & 0x4000) // 接收到了 '<'
			{
				// 接收到了 '>'，接收完成
				if (aRxBuffer1[0] == '>')
				{
					USART_RX_STA1 |= 0x8000;   // 接收完成退出
					USART_RX_BUF1[USART_RX_STA1 & 0X3FFF] = aRxBuffer1[0];
					USART_RX_STA1++;
				}
				// 存储数据
				else
				{
					USART_RX_BUF1[USART_RX_STA1 & 0X3FFF] = aRxBuffer1[0];
					USART_RX_STA1++;
					/* 数据溢出 */
					if ((USART_RX_STA1 & 0X3FFF) > (USART_REC_LEN - 1))
						USART_RX_STA1 = 0; // 接收数据错误,重新开始接收
				}
			}
		}
	}
	else if (huart->Instance == USART2) // 如果是串口2
	{
		if ((USART_RX_STA2 & 0x8000) == 0) // 接收未完成
		{
			if (USART_RX_STA2 & 0x4000) // 接收到了0x0d
			{
				if (aRxBuffer2[0] != 0x0a)
					USART_RX_STA2 = 0; // 接收错误,重新开始
				else
					USART_RX_STA2 |= 0x8000; // 接收完成了
			}
			else // 还没收到0X0D
			{
				if (aRxBuffer2[0] == 0x0d)
					USART_RX_STA2 |= 0x4000;
				else
				{
					USART_RX_BUF2[USART_RX_STA2 & 0X3FFF] = aRxBuffer2[0];
					USART_RX_STA2++;
					/* 数据溢出 */
					if (USART_RX_STA2 > (USART_REC_LEN - 1))
						USART_RX_STA2 = 0; // 接收数据错误,重新开始接收
				}
			}
		}
	}
}

// 串口1中断服务程序
void USART1_IRQHandler(void)
{
	u32 timeout = 0;
#if SYSTEM_SUPPORT_OS // 使用OS
	OSIntEnter();
#endif

	HAL_UART_IRQHandler(&UART1_Handler); // 调用HAL库中断处理公用函数

	timeout = 0;
	while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY) // 等待就绪
	{
		timeout++; ////超时处理
		if (timeout > HAL_MAX_DELAY)
			break;
	}
	timeout = 0;
	while (HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer1, RXBUFFERSIZE) != HAL_OK) // 一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
		timeout++; // 超时处理
		if (timeout > HAL_MAX_DELAY)
			break;
	}
#if SYSTEM_SUPPORT_OS // 使用OS
	OSIntExit();
#endif
}

// 串口2中断服务程序
void USART2_IRQHandler(void)
{
	u32 timeout = 0;
#if SYSTEM_SUPPORT_OS // 使用OS
	OSIntEnter();
#endif

	HAL_UART_IRQHandler(&UART2_Handler); // 调用HAL库中断处理公用函数

	timeout = 0;
	while (HAL_UART_GetState(&UART2_Handler) != HAL_UART_STATE_READY) // 等待就绪
	{
		timeout++; ////超时处理
		if (timeout > HAL_MAX_DELAY)
			break;
	}

	timeout = 0;
	while (HAL_UART_Receive_IT(&UART2_Handler, (u8 *)aRxBuffer2, RXBUFFERSIZE) != HAL_OK) // 一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
		timeout++; // 超时处理
		if (timeout > HAL_MAX_DELAY)
			break;
	}
#if SYSTEM_SUPPORT_OS // 使用OS
	OSIntExit();
#endif
}

// 串口6中断服务程序
void USART6_IRQHandler(void)
{
	u32 timeout = 0;
#if SYSTEM_SUPPORT_OS // 使用OS
	OSIntEnter();
#endif

	HAL_UART_IRQHandler(&UART6_Handler); // 调用HAL库中断处理公用函数

	timeout = 0;
	while (HAL_UART_GetState(&UART6_Handler) != HAL_UART_STATE_READY) // 等待就绪
	{
		timeout++; ////超时处理
		if (timeout > HAL_MAX_DELAY)
			break;
	}

	timeout = 0;
	while (HAL_UART_Receive_IT(&UART6_Handler, (u8 *)aRxBuffer6, RXBUFFERSIZE) != HAL_OK) // 一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
		timeout++; // 超时处理
		if (timeout > HAL_MAX_DELAY)
			break;
	}
#if SYSTEM_SUPPORT_OS // 使用OS
	OSIntExit();
#endif
}
#endif
