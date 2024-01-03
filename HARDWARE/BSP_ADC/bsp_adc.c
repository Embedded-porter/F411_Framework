#include "bsp_adc.h"
#include "delay.h"

ADC_HandleTypeDef ADC1_Handler; // ADC���

/*
*********************************************************************************************************
*   �� �� ��: bsp_AdcInit
*   ����˵��: ADC1���ó�ʼ��
*   ��    ��: ��
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_AdcInit(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};
  ADC1_Handler.Instance = ADC1;                                           /* ADC1 */
  ADC1_Handler.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;            /* ADCԤ��Ƶ */
  ADC1_Handler.Init.Resolution = ADC_RESOLUTION_12B;                      /* �ֱ���Ϊ12λ */
  ADC1_Handler.Init.ScanConvMode = ENABLE;                                /* ɨ��ģʽ */
  ADC1_Handler.Init.ContinuousConvMode = DISABLE;                         /* ѭ��ת��ģʽ */
  ADC1_Handler.Init.DiscontinuousConvMode = ENABLE;                       /* ������ת��ģʽ */
  ADC1_Handler.Init.NbrOfDiscConversion = 1;                              /* 1��������ת�� */
  ADC1_Handler.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; /* ���ⲿ���� */
  ADC1_Handler.Init.ExternalTrigConv = ADC_SOFTWARE_START;                /* �ֶ����� */
  ADC1_Handler.Init.DataAlign = ADC_DATAALIGN_RIGHT;                      /* �����Ҷ��� */
  ADC1_Handler.Init.NbrOfConversion = 10;                                 /* ����10��ͨ�� */
  ADC1_Handler.Init.DMAContinuousRequests = DISABLE;                      /* DMA���������� */
  ADC1_Handler.Init.EOCSelection = ADC_EOC_SINGLE_CONV;                   /* �������EOC���� */
  if (HAL_ADC_Init(&ADC1_Handler) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
   */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;
  if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
   */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
   */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
   */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
   */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
   */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = 6;
  if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
   */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 7;
  if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
   */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = 8;
  if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
   */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = 9;
  if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
   */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = 10;
  if (HAL_ADC_ConfigChannel(&ADC1_Handler, &sConfig) != HAL_OK)
  {
    Error_Handler(__FILE__, __LINE__);
  }
}
/*
*********************************************************************************************************
*   �� �� ��: HAL_ADC_MspInit
*   ����˵��: ADC�ײ��������������ã�ʱ��ʹ��
*   ��    ��: hadc��ADC���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (hadc->Instance == ADC1)
  {
    /* USER CODE BEGIN ADC1_MspInit 0 */

    /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**ADC1 GPIO Configuration
    PC0     ------> ADC1_IN10
    PC1     ------> ADC1_IN11
    PC2     ------> ADC1_IN12
    PC3     ------> ADC1_IN13
    PA0-WKUP------> ADC1_IN0
    PA1     ------> ADC1_IN1
    PA2     ------> ADC1_IN2
    PA3     ------> ADC1_IN3
    PB0     ------> ADC1_IN8
    PB1     ------> ADC1_IN9
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }
}
/*
*********************************************************************************************************
*   �� �� ��: HAL_ADC_MspDeInit
*   ����˵��: ADC�ײ���������Ϊ���״̬��Ҳ���ǲ����κ�����
*   ��    ��: hadc��ADC���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{

  if (hadc->Instance == ADC1)
  {
    /* USER CODE BEGIN ADC1_MspDeInit 0 */

    /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

#ifdef _BSP_ADC_DMA_ENABLE
    
#endif // 

    /**ADC1 GPIO Configuration
    PC0     ------> ADC1_IN10
    PC1     ------> ADC1_IN11
    PC2     ------> ADC1_IN12
    PC3     ------> ADC1_IN13
    PA0-WKUP     ------> ADC1_IN0
    PA1     ------> ADC1_IN1
    PA2     ------> ADC1_IN2
    PA3     ------> ADC1_IN3
    PB0     ------> ADC1_IN8
    PB1     ------> ADC1_IN9
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0 | GPIO_PIN_1);
  }
}
/*
*********************************************************************************************************
*   �� �� ��: Get_Adc
*   ����˵��: ���ADCֵԭʼֵ
*   ��    ��: ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
*   �� �� ֵ: ת�����
*********************************************************************************************************
*/
static u16 Get_Adc(u32 ch)
{
  ADC_ChannelConfTypeDef ADC1_ChanConf;

  ADC1_ChanConf.Channel = ch;                            // ͨ��
  ADC1_ChanConf.Rank = 1;                                // ��1�����У�����1
  ADC1_ChanConf.SamplingTime = ADC_SAMPLETIME_480CYCLES; // ����ʱ��
  ADC1_ChanConf.Offset = 0;
  HAL_ADC_ConfigChannel(&ADC1_Handler, &ADC1_ChanConf); // ͨ������

  HAL_ADC_Start(&ADC1_Handler); // ����ADC

  HAL_ADC_PollForConversion(&ADC1_Handler, 10); // ��ѯת��

  return (u16)HAL_ADC_GetValue(&ADC1_Handler); // �������һ��ADC1�������ת�����
}
/*
*********************************************************************************************************
*   �� �� ��: Get_Adc_Average
*   ����˵��: ��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ��
*   ��    ��: ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16   times:��ȡ����
*   �� �� ֵ: ͨ��ch��times��ת�����ƽ��ֵ
*********************************************************************************************************
*/
u16 Get_Adc_Average(u32 ch, u8 times)
{
  u32 temp_val = 0;
  u8 t;
  for (t = 0; t < times; t++)
  {
    temp_val += Get_Adc(ch);
  }
  return temp_val / times;
}
