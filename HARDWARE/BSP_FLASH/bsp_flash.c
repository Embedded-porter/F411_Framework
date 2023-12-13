#include "bsp_flash.h"
#include "delay.h"

/*
*********************************************************************************************************
*	函 数 名: STMFLASH_ReadWord
*	功能说明: 读取指定地址的字(32位数据)
*	形    参: faddr - 要读取的地址
*	返 回 值: 对应数据
*********************************************************************************************************
*/
u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(__IO uint32_t *)faddr;
}

/*
*********************************************************************************************************
*	函 数 名: STMFLASH_GetFlashSector
*	功能说明: 获取某个地址所在的flash扇区
*	形    参: faddr - 要读取的地址
*	返 回 值: 对应是扇区序号
*********************************************************************************************************
*/
u8 STMFLASH_GetFlashSector(u32 addr)
{
	if (addr < ADDR_FLASH_SECTOR_1)
		return FLASH_SECTOR_0;
	else if (addr < ADDR_FLASH_SECTOR_2)
		return FLASH_SECTOR_1;
	else if (addr < ADDR_FLASH_SECTOR_3)
		return FLASH_SECTOR_2;
	else if (addr < ADDR_FLASH_SECTOR_4)
		return FLASH_SECTOR_3;
	else if (addr < ADDR_FLASH_SECTOR_5)
		return FLASH_SECTOR_4;
	return FLASH_SECTOR_5;
}

/*
*********************************************************************************************************
*	函 数 名: STMFLASH_Write
*	功能说明: 从指定地址开始写入指定长度的数据
*			 特别注意:因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数
*        	 写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以
*       	 写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
*       	 没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写
*	形    参: WriteAddr:起始地址(此地址必须为4的倍数!!)  pBuffer:数据指针  NumToWrite:要写入数据的个数
*	返 回 值:无
*********************************************************************************************************
*/
void STMFLASH_Write(u32 WriteAddr, u32 *pBuffer, u32 NumToWrite)
{
	FLASH_EraseInitTypeDef FlashEraseInit;
	HAL_StatusTypeDef FlashStatus = HAL_OK;
	u32 SectorError = 0;
	u32 addrx = 0;
	u32 endaddr = 0;
	if (WriteAddr < STM32_FLASH_BASE || WriteAddr % 4)
		return; // 非法地址

	HAL_FLASH_Unlock();					  // 解锁
	addrx = WriteAddr;					  // 写入的起始地址
	endaddr = WriteAddr + NumToWrite * 4; // 写入的结束地址

	if (addrx < 0X1FFF0000)
	{
		while (addrx < endaddr) // 扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
		{
			if (STMFLASH_ReadWord(addrx) != 0XFFFFFFFF) // 有非0XFFFFFFFF的地方,要擦除这个扇区
			{
				FlashEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;		// 擦除类型，扇区擦除
				FlashEraseInit.Sector = STMFLASH_GetFlashSector(addrx); // 要擦除的扇区
				FlashEraseInit.NbSectors = 1;							// 一次只擦除一个扇区
				FlashEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;	// 电压范围，VCC=2.7~3.6V之间!!
				if (HAL_FLASHEx_Erase(&FlashEraseInit, &SectorError) != HAL_OK)
				{
					break; // 发生错误了
				}
			}
			else
				addrx += 4;
			FLASH_WaitForLastOperation(FLASH_WAITETIME); // 等待上次操作完成
		}
	}
	FlashStatus = FLASH_WaitForLastOperation(FLASH_WAITETIME); // 等待上次操作完成
	if (FlashStatus == HAL_OK)
	{
		while (WriteAddr < endaddr) // 写数据
		{
			if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, WriteAddr, *pBuffer) != HAL_OK) // 写入数据
			{
				break; // 写入异常
			}
			WriteAddr += 4;
			pBuffer++;
		}
	}
	HAL_FLASH_Lock(); // 上锁
}

/*
*********************************************************************************************************
*	函 数 名: STMFLASH_Read
*	功能说明: 从指定地址开始读出指定长度的数据
*	形    参: ReadAddr:读取起始地址  pBuffer:数据指针  NumToRead:要读取的地址
*	返 回 值: 无
*********************************************************************************************************
*/
void STMFLASH_Read(u32 ReadAddr, u32 *pBuffer, u32 NumToRead)
{
	u32 i;
	for (i = 0; i < NumToRead; i++)
	{
		pBuffer[i] = STMFLASH_ReadWord(ReadAddr); // 读取4个字节.
		ReadAddr += 4;							  // 偏移4个字节.
	}
}

/*
*********************************************************************************************************
*	函 数 名: Flash_Write_Word
*	功能说明: 指定地址写入一个数据
*	形    参: WriteAddr:写入数据的目的地址  WriteData:要写入的数据
*	返 回 值: 无
*********************************************************************************************************
*/
void Flash_Write_Word(u32 WriteAddr, u32 WriteData)
{
	STMFLASH_Write(WriteAddr, &WriteData, 1);
}
