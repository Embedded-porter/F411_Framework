#include "bsp_master_i2c.h"
#include "delay.h"

/*
*********************************************************************************************************
*   函 数 名: Master_I2C_Init
*   功能说明: IC2通信模块初始化
*   形    参: 无
*   返 回 值: 无
*********************************************************************************************************
*/
void Master_I2C_Init(void)
{
	IIC_Init();
}

/*
*********************************************************************************************************
*   函 数 名: Master_WriteLenByte
*   功能说明: 写数据
*   形    参: I2C_Address:器件地址 pData:数据指针 Len:数据长度
*   返 回 值: 无
*********************************************************************************************************
*/
int Master_WriteLenByte(u32 I2C_Address, u8 *pData, u8 Len)
{
	u8 t, ack = 0;

	IIC_Start();

	IIC_Send_Byte(I2C_Address); // 发送器件地址,写数据
	ack = IIC_Wait_Ack();		// 等待从机应答
	/*从机应答了，开始传输数据*/
	if (ack == 0)
	{
		for (t = 0; t < Len; t++)
		{
			IIC_Send_Byte(*pData); // 发送字节
			pData++;
			ack = IIC_Wait_Ack();
			/* 应答失败，产生停止标志 */
			if (ack == 1)
			{
				IIC_Stop(); // 产生一个停止条件
				delay_ms(10);
				return I2C_FAIL; // 退出传输
			}
			/*应答成功，继续传输*/
			else
			{
			}
		}
		/*到这里数据已经发送完毕*/
		IIC_Stop(); // 产生一个停止条件
		delay_ms(10);
		return I2C_OK; // 通讯成功
	}
	/*从机未应答*/
	else
	{
		IIC_Stop(); // 产生一个停止条件
		delay_ms(10);
		printf("[ERR]: Slaver ADDR AF\r\n");
		return I2C_FAIL; // 退出传输
	}
}

/*
*********************************************************************************************************
*   函 数 名: Master_ReadLenByte
*   功能说明: 读数据
*   形    参: I2C_Address:器件地址 pData:数据指针 Len:数据长度
*   返 回 值: 无
*********************************************************************************************************
*/
int Master_ReadLenByte(u32 I2C_Address, u8 *pData, u8 Len)
{
	u8 t;
	u32 temp = 0;
	u8 ack = 0;
	u8 data_length = 0;
	/*起始信号*/
	IIC_Start();
	/*发送器件地址,读数据*/
	IIC_Send_Byte(I2C_Address | 0x01);
	/*等待从机应答*/
	ack = IIC_Wait_Ack();
	/*从机应答了，开始接收数据*/
	if (ack == 0)
	{
		/* 先接收前两个byte 以确定包长度 */

		/* 根据协议规定，第一个byte最高位若为1，那么数据包格式是不同的，要重新设置剩余接收长度 */
		/* --------------------------------- */
		/* [0X80+CMD]  [ERR CODE]  [CRC8]	 */
		/* --------------------------------- */

		/*接收第一个byte数据，并发送应答*/
		temp = IIC_Read_One_Byte();
		IIC_Ack();
		*pData = temp;
		pData++;
		/*开始判断第一个byte最高位是否为1*/
		if ((temp & 0x80) != 0)
		{
			/*这个包固定长度为3byte，重新设置剩余接收长度，跳过中间的代码*/
			Len = 2;
			goto loop_rec;
		}

		/*接收第二个byte数据，在下面根据data_length决定是否发送应答*/
		temp = IIC_Read_One_Byte();

		*pData = temp;
		/*读取CMD包设定的I2C接收长度*/
		data_length = *pData;

		pData++;

		/*如果解析出来的包长度为>=128,那么停止通信*/
		if (data_length >= 128)
		{
			/*长度不正确，发送NACK，并且发送STOP*/
			IIC_NAck();
			IIC_Stop(); // 产生一个停止条件
			delay_ms(10);
			return I2C_FAIL;
		}
		/*长度符合要求，已经收下2byte，重新设置I2C剩余接收长度*/
		else
		{
			/*长度正确，发送应答*/
			IIC_Ack();
			/* CRC(1) */
			Len = data_length + 1;
		}

	loop_rec:
		/* 开始接收剩余数据 */
		for (t = 0; t < Len; t++)
		{
			/*接收一个byte数据*/
			temp = IIC_Read_One_Byte();

			/*判断是否为最后一个byte，是则发送 NACK */
			if (t == Len - 1)
				IIC_NAck(); // 发送NACK
			else
				IIC_Ack(); // 发送NACK

			*pData = temp;
			pData++;
		}

		/*到这里数据已经发送完毕*/
		IIC_Stop(); // 产生一个停止条件
		delay_ms(10);
	}
	/* 从机未应答 */
	else
	{
		IIC_Stop(); // 产生一个停止条件
		delay_ms(10);
		return I2C_FAIL;
	}
	return I2C_OK;
}
/**************************************************************/
