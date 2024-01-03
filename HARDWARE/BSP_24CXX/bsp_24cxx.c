#include "bsp_24cxx.h"
#include "delay.h"

/*
*********************************************************************************************************
*   函 数 名: AT24CXX_Init
*   功能说明: AT24CXX模块初始化
*   形    参: 无
*   返 回 值: 无
*********************************************************************************************************
*/
void AT24CXX_Init(void)
{
	IIC_Init(); // IIC初始化
}
/*
*********************************************************************************************************
*   函 数 名: AT24CXX_ReadOneByte
*   功能说明: 指定地址读取一个字节数据
*   形    参: ReadAddr:读取地址
*   返 回 值: u8:读到的数据
*********************************************************************************************************
*/
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
	u8 temp = 0;
	IIC_Start();
	if (EE_TYPE > AT24C16)
	{
		IIC_Send_Byte(0XA0); // 发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr >> 8); // 发送高地址
	}
	else
		IIC_Send_Byte(0XA0 + ((ReadAddr / 256) << 1)); // 发送器件地址0XA0,写数据
	IIC_Wait_Ack();
	IIC_Send_Byte(ReadAddr % 256); // 发送低地址
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(0XA1); // 进入接收模式
	IIC_Wait_Ack();
	temp = IIC_Read_Byte(0);
	IIC_Stop(); // 产生一个停止条件
	return temp;
}

/*
*********************************************************************************************************
 *   函 数 名: AT24CXX_WriteOneByte
 *   功能说明: 指定地址写入一个字节数据
 *   形    参: WriteAddr:读取地址 DataToWrite:写入数据
 *   返 回 值: 无
 *********************************************************************************************************
 */
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite)
{
	IIC_Start();
	if (EE_TYPE > AT24C16)
	{
		IIC_Send_Byte(0XA0); // 发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr >> 8); // 发送高地址
	}
	else
		IIC_Send_Byte(0XA0 + ((WriteAddr / 256) << 1)); // 发送器件地址0XA0,写数据
	IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr % 256); // 发送低地址
	IIC_Wait_Ack();
	IIC_Send_Byte(DataToWrite); // 发送字节
	IIC_Wait_Ack();
	IIC_Stop(); // 产生一个停止条件
	delay_ms(10);
}

/*
*********************************************************************************************************
*   函 数 名: AT24CXX_WriteLenByte
*   功能说明: 在AT24CXX里面的指定地址开始写入长度为Len的数据
*   形    参: WriteAddr:开始写入的地址 DataToWrite:数据数组首地址 Len:要写入数据的长度
*   返 回 值: 无
*********************************************************************************************************
*/
void AT24CXX_WriteLenByte(u16 WriteAddr, u32 DataToWrite, u8 Len)
{
	u8 t;
	for (t = 0; t < Len; t++)
	{
		AT24CXX_WriteOneByte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xff);
	}
}

/*
*********************************************************************************************************
*   函 数 名: AT24CXX_ReadLenByte
*   功能说明: 在AT24CXX里面的指定地址开始读出长度为Len的数据,该函数用于读出16bit或者32bit的数据
*   形    参: ReadAddr:开始读出的地址 Len:要读出数据的长度2,4
*   返 回 值: 读取到的数据
*********************************************************************************************************
*/
u32 AT24CXX_ReadLenByte(u16 ReadAddr, u8 Len)
{
	u8 t;
	u32 temp = 0;
	for (t = 0; t < Len; t++)
	{
		temp <<= 8;
		temp += AT24CXX_ReadOneByte(ReadAddr + Len - t - 1);
	}
	return temp;
}

/*
*********************************************************************************************************
*   函 数 名: AT24CXX_Check
*   功能说明: 检查AT24CXX是否正常,通过写入最后一个地址的内容来判断(使用其他的型号记得修改)
*   形    参: 无
*   返 回 值: 1:检测失败 0:检测成功
*********************************************************************************************************
*/
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp = AT24CXX_ReadOneByte(16383); // 避免每次开机都写AT24CXX
	if (temp == 0X55)
		return 0;
	else // 排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(16383, 0X55);
		temp = AT24CXX_ReadOneByte(16383);
		if (temp == 0X55)
			return 0;
	}
	return 1;
}

/*
*********************************************************************************************************
*   函 数 名: AT24CXX_Read
*   功能说明: 在AT24CXX里面的指定地址开始读出指定个数的数据
*   形    参: ReadAddr:开始读出的地址 pBuffer:数据数组首地址 NumToRead:要读出数据的个数
*   返 回 值: 无
*********************************************************************************************************
*/
void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead)
{
	while (NumToRead)
	{
		*pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);
		NumToRead--;
	}
}

/*
*********************************************************************************************************
*   函 数 名: AT24CXX_Write
*   功能说明: 在AT24CXX里面的指定地址开始写入指定个数的数据
*   形    参: ReadAddr:开始写入的地址 pBuffer:数据数组首地址 NumToRead:要写入数据的个数
*   返 回 值: 无
*********************************************************************************************************
*/
void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite)
{
	while (NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr, *pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}

/*
*********************************************************************************************************
*   函 数 名: AT24CXX_ReadString
*   功能说明: 在AT24CXX里面的指定地址开始读取指定个数的字符串数据
*   形    参: ReadAddr:开始读取的地址 pBuffer:数据数组首地址 NumToRead:要读取数据的个数
*   返 回 值: 无
*********************************************************************************************************
*/
void AT24CXX_ReadString(uint16_t ReadAddr, char *buffer, uint16_t maxLength)
{
	uint16_t i = 0;
	while (i < maxLength - 1)
	{
		uint8_t data = AT24CXX_ReadOneByte(ReadAddr + i);
		if (data == '\0')
		{
			break; // 遇到字符串结束符，停止读取
		}
		buffer[i] = data;
		i++;
	}
	buffer[i] = '\0'; // 添加字符串结束符
}

/*
*********************************************************************************************************
*   函 数 名: AT24CXX_WriteString
*   功能说明: 在AT24CXX里面的指定地址开始写入字符串数据
*   形    参: ReadAddr:开始读取的地址 str:要写入的字符串
*   返 回 值: 无
*********************************************************************************************************
*/
void AT24CXX_WriteString(uint16_t WriteAddr, const char *str)
{
	uint16_t len = strlen(str);
	for (uint16_t i = 0; i < len; i++)
	{
		AT24CXX_WriteOneByte(WriteAddr + i, str[i]);
	}
}

/*
*********************************************************************************************************
*   函 数 名: AT24CXX_Write_Float_Num
*   功能说明: 在AT24CXX里面的指定地址开始写入指定的浮点数据
*   形    参: ReadAddr:开始读取的地址 data:要写入的浮点数据
*   返 回 值: 无
*********************************************************************************************************
*/
void AT24CXX_Write_Float_Num(uint16_t WriteAddr, float data)
{
	float_data_t data1;
	data1.data = data;
	AT24CXX_Write(WriteAddr, data1.byte, 4);
}

/*
*********************************************************************************************************
*   函 数 名: AT24CXX_Read_Float_Num
*   功能说明: 在AT24CXX里面的指定地址开始读取指定的浮点数据
*   形    参: ReadAddr:开始读取的地址
*   返 回 值: 无
*********************************************************************************************************
*/
float AT24CXX_Read_Float_Num(uint16_t ReadAddr)
{
	float_data_t data1;
	AT24CXX_Read(ReadAddr, data1.byte, 4);
	return data1.data;
}

/*
*********************************************************************************************************
*   函 数 名: AT24CXX_Write_Int_Num
*   功能说明: 在AT24CXX里面的指定地址开始写入指定的整型数据
*   形    参: ReadAddr:开始读取的地址 data:要写入的整型数据
*   返 回 值: 无
*********************************************************************************************************
*/
void AT24CXX_Write_Int_Num(uint16_t WriteAddr, int data)
{
	int_data_t data1;
	data1.data = data;
	AT24CXX_Write(WriteAddr, data1.byte, 4);
}

/*
*********************************************************************************************************
*   函 数 名: AT24CXX_Read_Int_Num
*   功能说明: 在AT24CXX里面的指定地址开始读取指定的整型数据
*   形    参: ReadAddr:开始读取的地址
*   返 回 值: 无
*********************************************************************************************************
*/
int AT24CXX_Read_Int_Num(uint16_t ReadAddr)
{
	int_data_t data1;
	AT24CXX_Read(ReadAddr, data1.byte, 4);
	return data1.data;
}

/*
*********************************************************************************************************
*   函 数 名: AT24CXX_Write_Short_Int_Num
*   功能说明: 在AT24CXX里面的指定地址开始写入指定的短整型数据
*   形    参: ReadAddr:开始读取的地址 data:要写入的短整型数据
*   返 回 值: 无
*********************************************************************************************************
*/
void AT24CXX_Write_Short_Int_Num(uint16_t WriteAddr, int16_t data)
{
	uint8_t byteData[2];
	byteData[0] = (uint8_t)(data >> 8); // 提取高位字节
	byteData[1] = (uint8_t)data;		// 提取低位字节

	AT24CXX_Write(WriteAddr, byteData, 2);
}

/*
*********************************************************************************************************
*   函 数 名: AT24CXX_Read_Short_Int_Num
*   功能说明: 在AT24CXX里面的指定地址开始读取指定的短整型数据
*   形    参: ReadAddr:开始读取的地址
*   返 回 值: 无
*********************************************************************************************************
*/
int16_t AT24CXX_Read_Short_Int_Num(uint16_t ReadAddr)
{
	uint8_t byteData[2];

	AT24CXX_Read(ReadAddr, byteData, 2);

	int16_t data = (int16_t)((byteData[0] << 8) | byteData[1]);

	return data;
}
