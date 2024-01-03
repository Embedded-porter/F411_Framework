#include "bsp_command.h"
#include "bsp_config.h"
#include "usart.h"

/* uart ���ͻ��� */
uint8_t send_buff[1024];

// �������
const FBCmdSt_t cmd_tbl[] = 
{
   {CMD01,           "[RESET]",                         (1<<0), "��λ",                                             CMD01_Proc},
   {CMD02,           "[STA_?]",                         (1<<0), "״̬��ѯ",                                          CMD02_Proc},
   {CMD03,           "[DAC_**_**]",                     (1<<2), "��ȡADC��ֵ",                                         CMD03_Proc},

    /*ʼ�շ�������棬��λ����յ�*/
   {CMDHELP,         "<HELP_?>",                        (1<<0),"help document",                                     CmdHelp_Proc},  
   {CMDHELPALL,      "<HELPALL_?>",                     (1<<2),"�����ڲ������ѯ",                                    CmdHelpAll_Proc},  
   {CMDOVER,             "",                             0,      "",                                                 NULL},
};
static uint16_t CMD01_Proc(uint8_t *rec_ptr, uint8_t *send_ptr,uint32_t *send_len)
{
    sprintf(send_ptr,"[RESET_OK1111]");
    *send_len +=strlen(send_ptr) ;
    return 0;
}

static uint16_t CMD02_Proc(uint8_t *rec_ptr, uint8_t *send_ptr,uint32_t *send_len)
{
     sprintf(send_ptr,"[STA_?1123]");
    *send_len +=strlen(send_ptr) ;
    return 0;
}

static uint16_t CMD03_Proc(uint8_t *rec_ptr, uint8_t *send_ptr,uint32_t *send_len)
{
    char *result = strchr(rec_ptr, '_');
    uint16_t ADC_data[2];
    ADC_data[0] = (result[1] - '0') * 10 + (result[2] - '0');
    ADC_data[1] = (result[4] - '0') * 10 + (result[5] - '0');
    sprintf(send_ptr,"[DAC_%2d_%2d]",ADC_data[0],ADC_data[1]);
    *send_len +=strlen(send_ptr) ;
    return 0;
}

/* *******************************
��������
��������ӡ�����ĵ�����������������ڲ�ֱ�Ӵ�ӡ������װ��buff,��Ϊ���ݷ��࣬ռ��RAM
���룺
�����
���أ�
����������
***********************************/
static uint16_t CmdHelp_Proc(uint8_t *rec_ptr, uint8_t *send_ptr,uint32_t *send_len)
{
    uint8_t cmd_ID = 0;

    // ��ӡ����
    printf("HELP:\r\n");
    for (cmd_ID = 0; cmd_ID < (sizeof(cmd_tbl) / sizeof(FBCmdSt_t) - 1); cmd_ID++)
    {
        if (cmd_tbl[cmd_ID].ATTR & 0x0004 != 0) /*���������ӡ*/
        {
            printf("%s", (const char *)cmd_tbl[cmd_ID].st_cmdstr);
            // ��ӡð��
            printf(": ");
            printf("%s", (const char *)cmd_tbl[cmd_ID].help_str);
            printf("\r\n");
        }
        // ��ӡ������Ϣ
    }
    return 0;
}

/* *******************************
��������
��������ӡ�����ĵ�����������������ڲ�ֱ�Ӵ�ӡ������װ��buff,��Ϊ���ݷ��࣬ռ��RAM
    ��ӡ������������û������������
���룺
�����
���أ�
����������
********************************** */
static uint16_t CmdHelpAll_Proc(uint8_t *rec_ptr, uint8_t *send_ptr,uint32_t *send_len)
{
    uint8_t cmd_ID = 0;

    // ��ӡ����
    printf("HELP:\r\n");
    for (cmd_ID = 0; cmd_ID < (sizeof(cmd_tbl) / sizeof(FBCmdSt_t) - 1); cmd_ID++)
    {

        if (cmd_tbl[cmd_ID].ATTR) /*���������ӡ*/
        {
            printf("%s", (const char *)cmd_tbl[cmd_ID].st_cmdstr);
            // ��ӡð��
            printf(": ");
            printf("%s", (const char *)cmd_tbl[cmd_ID].help_str);
            printf("\r\n");
        }
        // ��ӡ������Ϣ
    }

    return 0;
}

/* *******************************
��������
����������ƥ�亯��
���룺rec_ptr���������ݰ�buff,send_ptr�������ݰ�buff,StartCharƥ������ַ������֣�
�����Ӧ���ַ�������ָ��
���أ�>=0,�����ַ���������  <0,ƥ��ʧ�ܣ�
����������
********************************** */
static int16_t FB_CmdRecognition(uint8_t *rec_ptr, uint8_t *send_ptr,uint32_t *send_len)
{
    uint8_t ID = 0;
    for (ID = 0; ID < (sizeof(cmd_tbl) / sizeof(FBCmdSt_t) - 1); ID++) // forѭ����������ṹ��
    {
        if (FB_StrCmp(rec_ptr, cmd_tbl[ID].st_cmdstr) > 0) // �ַ���ƥ��ɹ�
        {
            if (cmd_tbl[ID].Proc != NULL)
            {
                return cmd_tbl[ID].Proc(rec_ptr, send_ptr,send_len); // ����һ������ָ��
            }
        }
    }
    return -1;
}

/***********************************************************
 * �������ƣ�cmdcmp
 * ���룺�����Ƚ��ַ���
 * �����1:�ַ����Ƚϳɹ�  0���ַ����Ƚ�ʧ��
 ****************************************************************/
static int FB_StrCmp(char *buf1, char *buf)
{
    int i, ret;

    ret = 1;

    for (i = 0; i < 200; i++)
    {
        if (buf[i] == 0)
        { // �ַ��������˳��Ƚ�
            break;
        }
        else if (buf[i] == '*')
        { //* �ַ����Ƚ�
            continue;
        }
        else if (buf[i] != buf1[i])
        { // �Ƚ��ַ�����ȷ���0
            ret = 0;
            break;
        }
    }
    return ret;
}

/* *******************************
��������
������ƥ�䴦������������յ�����Ϣrec_ptr
���룺rec_ptr���������ݰ�buff,send_ptr�������ݰ�buff��send_len���͵��ַ�������ָ��
�����
    send_ptr ����õ��ַ���
    send_len�ַ������ȳ���
���أ���
����������
********************************** */
static void Cmd_all_receive(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len)
{
    int16_t i, j;
    int16_t count_len;

    send_ptr[0] = '<';                  
    memcpy(send_ptr + 1, rec_ptr + 1, 4); // ��"<C01_"�ĸ��ַ����Ƶ�send_ptr
    *send_len = 5;
    j = 5;
    while (rec_ptr[j] != '>' && rec_ptr[j] != 0) // ����0������'>'����
    {
        if (rec_ptr[j] != '[')
        {
            j++; // �����ж�rec_ptr[j]�Ƿ�����ַ�'['����������ڣ���j��1��������һ��ѭ����
        }
        else
        {
            count_len = FB_CmdRecognition(&rec_ptr[j], (send_ptr + *send_len),send_len); // �ӵ�������ʼ����
            // δƥ��ʱ
            if (count_len < 0)
            {
                count_len = 0;

                memcpy(send_ptr + *send_len, "[CMD_ERR]", 9);
                count_len = 9;
            }
            *send_len += count_len;
            j++;
        }
    }
    send_ptr[*send_len] = '>';
    *send_len += 1;
    send_ptr[*send_len] = 0;
}

/*********************************************************************************************************
** ��������: receive_command
** ��������: ���մ���I2C����
** �䡡��: 	rec_ptr:ָ���������ݶ���
**         	len:��Ч���ݳ���
** �䡡��:	send_ptr:ָ��������ݶ���
**			send_len��������ݳ���
**
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/

void receive_command(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len)
{
    uint8_t card_num;
    int16_t len;          /*��������ĺ���*/

    *send_len = 0;
    if ((rec_ptr[0] == '<') && (rec_ptr[1] == 'C') && (rec_ptr[4]) == '_')
    {
        card_num = (rec_ptr[2] - '0') * 10 + rec_ptr[3] - '0'; // �ж������ſ�

        if (card_num == 0x01)
        {
            Cmd_all_receive(rec_ptr, send_ptr, send_len);
        }
        else
        {
            *send_len = 0;
        }
    }
    else
    {
        len = FB_CmdRecognition(rec_ptr, send_ptr,send_len);
        // ƥ��ʧ��
        if (len < 0)
        {
            len = 0;
            memcpy(send_ptr, "<CMD_ERR>", 9);
            len = 9;
        }
        *send_len = len;
    }
}

void ReceivePolling(void)
{
    int send_len;
    uint16_t usart_len;  // ���ڽ������ݳ���
    if (USART_RX_STA1 & 0x8000) // �������
    {
        usart_len = USART_RX_STA1 & 0x3fff; // �õ��˴ν��յ������ݳ���

        memset(send_buff, 0, sizeof(send_buff));
        // ����ָ��
        receive_command(&USART_RX_BUF1[0], send_buff, &send_len);
        if(send_len!=0)
        {
            printf("%s\r\n", send_buff);
        }
        // Ӧ��ָ��
        USART_RX_STA1 = 0;
    }
}