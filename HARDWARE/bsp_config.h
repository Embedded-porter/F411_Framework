#ifndef _BSP_CONFIG_H
#define _BSP_CONFIG_H

/**************************Ӳ��ԭ��ͼ**********************************
SPIͨ��:  SPI_CS:PA4
          SPI_SCK:PA5
          SPI_MISO:PA6
          SPI_MOSI:PA7
          ����Ҫ��:DMA+SPI ����ģʽ ͨ���ٶ�:10M
AT24CXX:  SCL:PB8
          SDA:PB9
          оƬ�ͺ�:AT24C32 �洢�ռ�:4096�ֽ�

10·ADC:  PD1:PA0
          PD2:PA1
          PD3:PA2
          PD4:PA3
          PD5:PB0
          PD6:PB1
          PD7:PC0
          PD8:PC1
          PD9:PC2
          PD10:PC3

SP LOS1: ��·�����⹦�ʼ������
SP LOS2: ��·�����⹦�ʼ������   

CSX:VOA��Ƭѡ����
I2C3 SCL:VOA����
I2C3 SDA:VOA����



**********************************************************************/
#include <stdio.h>

//#define DBG_PRINTF(...)
#define DBG_PRINTF printf

#endif // !_BSP_CONFIG_H