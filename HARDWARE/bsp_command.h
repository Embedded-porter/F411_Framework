#ifndef _BSP_COMMAND_H
#define _BSP_COMMAND_H

#include "sys.h"
#include "string.h"

typedef enum Command_ID
{
    CMD01,
    CMD02,
    CMD03,
    /*ʼ�շ�������棬��λ����յ�*/
    CMDHELP,
    CMDHELPALL,
    CMDOVER,
} Command_ID;

/**********************
 * ����ƥ��ṹ��
 ***********************/
typedef struct FB_CmdSt
{
    int16_t id;
    uint8_t *st_cmdstr;                                    // �����ַ�
    uint16_t ATTR;                                         //���ԣ� 0����ʹ�ã�1�ͻ�ʹ��,2����ʹ��
    uint8_t *help_str;                                     // �����ַ�
    uint16_t (*Proc)(uint8_t *rec_ptr, uint8_t *send_ptr,uint32_t *send_len); // ����ָ�뺯��
} FBCmdSt_t;

/********************************
�������������
********************************** */
static uint16_t CMD01_Proc(uint8_t *rec_ptr, uint8_t *send_ptr,uint32_t *send_len);
static uint16_t CMD02_Proc(uint8_t *rec_ptr, uint8_t *send_ptr,uint32_t *send_len);
static uint16_t CMD03_Proc(uint8_t *rec_ptr, uint8_t *send_ptr,uint32_t *send_len);

/* *******************************
��������
��������ӡ�����ĵ�����������������ڲ�ֱ�Ӵ�ӡ������װ��buff,��Ϊ���ݷ��࣬ռ��RAM
********************************** */
static uint16_t CmdHelp_Proc(uint8_t *rec_ptr, uint8_t *send_ptr,uint32_t *send_len);
static uint16_t CmdHelpAll_Proc(uint8_t *rec_ptr, uint8_t *send_ptr,uint32_t *send_len);

/*******************************
�������ַ���������
***********************************/
/* �ַ����ԱȺ��� */
static int FB_StrCmp(char *buf1, char *buf);

/* ����ƥ�亯�� */
static int16_t FB_CmdRecognition(uint8_t *rec_ptr, uint8_t *send_ptr,uint32_t *send_len);

/* ƥ�䴦���� */
static void Cmd_all_receive(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len);

/* ����ָ���,�ṩ�ⲿʹ�� */
void receive_command(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len);
/* ָ����ѯ���� */
void ReceivePolling(void);
#endif // !_BSP_COMMAND_H