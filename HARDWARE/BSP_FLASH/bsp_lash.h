#ifndef _BSP_FLASH_H
#define _BSP_FLASH_H
#include "sys.h"

// FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 // STM32 FLASH����ʼ��ַ
#define FLASH_WAITETIME 50000       // FLASH�ȴ���ʱʱ��

// FLASH ��������ʼ��ַ
#define ADDR_FLASH_SECTOR_0 ((u32)0x08000000) // ����0��ʼ��ַ, 16 Kbytes
#define ADDR_FLASH_SECTOR_1 ((u32)0x08004000) // ����1��ʼ��ַ, 16 Kbytes
#define ADDR_FLASH_SECTOR_2 ((u32)0x08008000) // ����2��ʼ��ַ, 16 Kbytes
#define ADDR_FLASH_SECTOR_3 ((u32)0x0800C000) // ����3��ʼ��ַ, 16 Kbytes
#define ADDR_FLASH_SECTOR_4 ((u32)0x08010000) // ����4��ʼ��ַ, 64 Kbytes
#define ADDR_FLASH_SECTOR_5 ((u32)0x08020000) // ����5��ʼ��ַ, 128 Kbytes

u32 STMFLASH_ReadWord(u32 faddr);                                 // ������
void STMFLASH_Write(u32 WriteAddr, u32 *pBuffer, u32 NumToWrite); // ��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr, u32 *pBuffer, u32 NumToRead);    // ��ָ����ַ��ʼ����ָ�����ȵ�����
void Flash_Write_Word(u32 WriteAddr, u32 WriteData);

#endif
