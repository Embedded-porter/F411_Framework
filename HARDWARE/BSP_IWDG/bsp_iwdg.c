#include "bsp_iwdg.h"
#include "bsp_config.h"

HAL_StatusTypeDef HAL_IWDG_Start(IWDG_HandleTypeDef *hiwdg)
{
	__HAL_IWDG_START(hiwdg);//���� IWDG����
	
	__HAL_IWDG_RELOAD_COUNTER(hiwdg);//��RLR�Ĵ����ж����ֵ���¼���IWDG������
	
	 return HAL_OK;
}

IWDG_HandleTypeDef IWDG_Handler; //�������Ź����

//��ʼ���������Ź�
//prer:��Ƶ��:IWDG_PRESCALER_4~IWDG_PRESCALER_256
//rlr:�Զ���װ��ֵ,0~0XFFF.
//ʱ�����(���):Tout=((4*2^prer)*rlr)/32 (ms).
void bsp_IwdgInit(u8 prer,u16 rlr)
{
    IWDG_Handler.Instance=IWDG;
    IWDG_Handler.Init.Prescaler=prer;	//����IWDG��Ƶϵ��
    IWDG_Handler.Init.Reload=rlr;		//��װ��ֵ
    HAL_IWDG_Init(&IWDG_Handler);		//��ʼ��IWDG,Ĭ�ϻῪ���������Ź�	

	  HAL_IWDG_Start(&IWDG_Handler);		//�����������Ź�
}
   
//ι�������Ź�
void bsp_IwdgFeed(void)
{   
    HAL_IWDG_Refresh(&IWDG_Handler); 	//ι��
}