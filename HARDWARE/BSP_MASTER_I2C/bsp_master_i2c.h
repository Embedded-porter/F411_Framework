#ifndef _BSP_MASTER_I2C_H
#define _BSP_MASTER_I2C_H
#include "sys.h"
#include "bsp_myiic.h"

//////////////////////////////////////////////////////////////////////////////////	 
/*	@Module: OTP8 Master I2C Driver												*/
/*	@Author: Alan Liu															*/
////////////////////////////////////////////////////////////////////////////////// 	

#define I2C_OK		 0
#define	I2C_FAIL	-1


int Master_WriteLenByte(u32 I2C_Address, u8* pData, u8 Len);
int Master_ReadLenByte(u32 I2C_Address, u8* pData, u8 Len);
void Master_I2C_Init(void);

#endif
