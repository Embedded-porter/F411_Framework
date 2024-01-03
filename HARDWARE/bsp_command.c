#include "bsp_command.h"
#include "bsp_config.h"
#include "usart.h"

/* uart 发送缓存 */
uint8_t send_buff[1024];

// 命令定义区
const FBCmdSt_t cmd_tbl[] = 
{
   {CMD01,           "[RESET]",                         (1<<0), "复位",                                             CMD01_Proc},
   {CMD02,           "[STA_?]",                         (1<<0), "状态查询",                                          CMD02_Proc},
   {CMD03,           "[DAC_**_**]",                     (1<<2), "读取ADC的值",                                         CMD03_Proc},

    /*始终放于最后面，定位表格终点*/
   {CMDHELP,         "<HELP_?>",                        (1<<0),"help document",                                     CmdHelp_Proc},  
   {CMDHELPALL,      "<HELPALL_?>",                     (1<<2),"厂家内部命令查询",                                    CmdHelpAll_Proc},  
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
函数名：
描述：打印帮助文档，单独输出函数，内部直接打印，无需装入buff,因为数据繁多，占用RAM
输入：
输出：
返回：
错误描述：
***********************************/
static uint16_t CmdHelp_Proc(uint8_t *rec_ptr, uint8_t *send_ptr,uint32_t *send_len)
{
    uint8_t cmd_ID = 0;

    // 打印命令
    printf("HELP:\r\n");
    for (cmd_ID = 0; cmd_ID < (sizeof(cmd_tbl) / sizeof(FBCmdSt_t) - 1); cmd_ID++)
    {
        if (cmd_tbl[cmd_ID].ATTR & 0x0004 != 0) /*调试命令不打印*/
        {
            printf("%s", (const char *)cmd_tbl[cmd_ID].st_cmdstr);
            // 打印冒号
            printf(": ");
            printf("%s", (const char *)cmd_tbl[cmd_ID].help_str);
            printf("\r\n");
        }
        // 打印帮助信息
    }
    return 0;
}

/* *******************************
函数名：
描述：打印帮助文档，单独输出函数，内部直接打印，无需装入buff,因为数据繁多，占用RAM
    打印所有命令，包括用户命令，厂家命令
输入：
输出：
返回：
错误描述：
********************************** */
static uint16_t CmdHelpAll_Proc(uint8_t *rec_ptr, uint8_t *send_ptr,uint32_t *send_len)
{
    uint8_t cmd_ID = 0;

    // 打印命令
    printf("HELP:\r\n");
    for (cmd_ID = 0; cmd_ID < (sizeof(cmd_tbl) / sizeof(FBCmdSt_t) - 1); cmd_ID++)
    {

        if (cmd_tbl[cmd_ID].ATTR) /*调试命令不打印*/
        {
            printf("%s", (const char *)cmd_tbl[cmd_ID].st_cmdstr);
            // 打印冒号
            printf(": ");
            printf("%s", (const char *)cmd_tbl[cmd_ID].help_str);
            printf("\r\n");
        }
        // 打印帮助信息
    }

    return 0;
}

/* *******************************
函数名：
描述：命令匹配函数
输入：rec_ptr：接收数据包buff,send_ptr发送数据包buff,StartChar匹配符首字符，区分；
输出：应答字符串函数指针
返回：>=0,返回字符串的数据  <0,匹配失败；
错误描述：
********************************** */
static int16_t FB_CmdRecognition(uint8_t *rec_ptr, uint8_t *send_ptr,uint32_t *send_len)
{
    uint8_t ID = 0;
    for (ID = 0; ID < (sizeof(cmd_tbl) / sizeof(FBCmdSt_t) - 1); ID++) // for循环遍历命令结构体
    {
        if (FB_StrCmp(rec_ptr, cmd_tbl[ID].st_cmdstr) > 0) // 字符串匹配成功
        {
            if (cmd_tbl[ID].Proc != NULL)
            {
                return cmd_tbl[ID].Proc(rec_ptr, send_ptr,send_len); // 返回一个函数指针
            }
        }
    }
    return -1;
}

/***********************************************************
 * 函数名称：cmdcmp
 * 输入：两个比较字符串
 * 输出：1:字符串比较成功  0：字符串比较失败
 ****************************************************************/
static int FB_StrCmp(char *buf1, char *buf)
{
    int i, ret;

    ret = 1;

    for (i = 0; i < 200; i++)
    {
        if (buf[i] == 0)
        { // 字符串结束退出比较
            break;
        }
        else if (buf[i] == '*')
        { //* 字符不比较
            continue;
        }
        else if (buf[i] != buf1[i])
        { // 比较字符不相等返回0
            ret = 0;
            break;
        }
    }
    return ret;
}

/* *******************************
函数名：
描述：匹配处理函数，处理接收到的信息rec_ptr
输入：rec_ptr：接收数据包buff,send_ptr发送数据包buff，send_len发送的字符串长度指针
输出：
    send_ptr 打包好的字符串
    send_len字符串长度长度
返回：无
错误描述：
********************************** */
static void Cmd_all_receive(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len)
{
    int16_t i, j;
    int16_t count_len;

    send_ptr[0] = '<';                  
    memcpy(send_ptr + 1, rec_ptr + 1, 4); // 将"<C01_"四个字符复制到send_ptr
    *send_len = 5;
    j = 5;
    while (rec_ptr[j] != '>' && rec_ptr[j] != 0) // 遇到0，或者'>'结束
    {
        if (rec_ptr[j] != '[')
        {
            j++; // 首先判断rec_ptr[j]是否等于字符'['，如果不等于，则将j加1，继续下一次循环。
        }
        else
        {
            count_len = FB_CmdRecognition(&rec_ptr[j], (send_ptr + *send_len),send_len); // 从第六个开始处理
            // 未匹配时
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
** 函数名称: receive_command
** 功能描述: 接收处理I2C命令
** 输　入: 	rec_ptr:指向输入数据队列
**         	len:有效数据长度
** 输　出:	send_ptr:指向输出数据队列
**			send_len：输出数据长度
**
** 全局变量: 无
** 调用模块: 无
********************************************************************************************************/

void receive_command(uint8_t *rec_ptr, uint8_t *send_ptr, uint32_t *send_len)
{
    uint8_t card_num;
    int16_t len;          /*兼容下面的函数*/

    *send_len = 0;
    if ((rec_ptr[0] == '<') && (rec_ptr[1] == 'C') && (rec_ptr[4]) == '_')
    {
        card_num = (rec_ptr[2] - '0') * 10 + rec_ptr[3] - '0'; // 判断是那张卡

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
        // 匹配失败
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
    uint16_t usart_len;  // 串口接收数据长度
    if (USART_RX_STA1 & 0x8000) // 接收完成
    {
        usart_len = USART_RX_STA1 & 0x3fff; // 得到此次接收到的数据长度

        memset(send_buff, 0, sizeof(send_buff));
        // 解析指令
        receive_command(&USART_RX_BUF1[0], send_buff, &send_len);
        if(send_len!=0)
        {
            printf("%s\r\n", send_buff);
        }
        // 应答指令
        USART_RX_STA1 = 0;
    }
}