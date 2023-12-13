#include "bsp_gpio.h"

// ö�����е�IO
static GPIO_TypeDef *GPIO_PORT[] = {
	G_LED1_GPIO_Port,
	G_LED2_GPIO_Port,
	G_LED3_GPIO_Port,
	G_KEY1_GPIO_Port,
	G_KEY2_GPIO_Port,
	G_KEY3_GPIO_Port,
	G_KEY4_GPIO_Port,
	G_BEEP_GPIO_Port,
};
// ö�����е�IO
static const uint16_t GPIO_PIN[] = {
	G_LED1_Pin,
	G_LED2_Pin,
	G_LED3_Pin,
	G_KEY1_Pin,
	G_KEY2_Pin,
	G_KEY3_Pin,
	G_KEY4_Pin,
	G_BEEP_Pin,
};

/**
 * @brief ��ȡIO��״̬
 * @param  xGPIO ��Ҫ��ȡ��io
 * @return uint8_t IO��״̬
 */
uint8_t GPIO_R(IO_TypeDef xGPIO)
{
	return HAL_GPIO_ReadPin(GPIO_PORT[xGPIO], GPIO_PIN[xGPIO]);
}
/**
 * @brief IO����ߵ�ƽ
 * @param  xGPIO  �����õ�IO��
 */
void BSP_IO_H(IO_TypeDef xGPIO)
{
	HAL_GPIO_WritePin(GPIO_PORT[xGPIO], GPIO_PIN[xGPIO], GPIO_PIN_SET);
}
/**
 * @brief IO����͵�ƽ
 * @param  xGPIO    �����õ�IO��
 */
void BSP_IO_L(IO_TypeDef xGPIO)
{
	HAL_GPIO_WritePin(GPIO_PORT[xGPIO], GPIO_PIN[xGPIO], GPIO_PIN_RESET);
}
/**
 * @brief IO��ת
 * @param  xGPIO   �����õ�IO��
 */
void BSP_IO_T(IO_TypeDef xGPIO) // ��ƽ��ת
{
	HAL_GPIO_TogglePin(GPIO_PORT[xGPIO], GPIO_PIN[xGPIO]);
}

/**
 * @brief �ⲿ�ж�
 * @param  GPIO_Pin  �ж�IO��
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
}

void bsp_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*LED��ʼ��*/
	GPIO_InitStruct.Pin = G_LED1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(G_LED1_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = G_LED2_Pin;
	HAL_GPIO_Init(G_LED2_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = G_LED3_Pin;
	HAL_GPIO_Init(G_LED3_GPIO_Port, &GPIO_InitStruct);

	/*BEEP��ʼ��*/
	GPIO_InitStruct.Pin = G_BEEP_Pin;
	HAL_GPIO_Init(G_BEEP_GPIO_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(G_BEEP_GPIO_Port, G_BEEP_Pin, GPIO_PIN_SET);

	/*����IO��ʼ��,ע��ߵ͵�ƽ*/
	GPIO_InitStruct.Pin = G_KEY1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(G_KEY1_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = G_KEY2_Pin;
	HAL_GPIO_Init(G_KEY2_GPIO_Port, &GPIO_InitStruct);

	/*����IO��ʼ��,ע��ߵ͵�ƽ*/
	GPIO_InitStruct.Pin = G_KEY3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(G_KEY3_GPIO_Port, &GPIO_InitStruct);

	/*����IO��ʼ��,ע��ߵ͵�ƽ*/
	GPIO_InitStruct.Pin = G_KEY4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(G_KEY4_GPIO_Port, &GPIO_InitStruct);
}
